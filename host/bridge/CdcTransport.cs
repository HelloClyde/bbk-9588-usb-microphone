// SPDX-License-Identifier: Apache-2.0

using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using Microsoft.Win32.SafeHandles;

namespace Usb9588MicBridge;

internal static class CdcDeviceDiscovery
{
    private const string UsbEnumPath =
        @"SYSTEM\CurrentControlSet\Enum\USB";
    private const string SerialMapPath =
        @"HARDWARE\DEVICEMAP\SERIALCOMM";
    private const string HardwareIdNeedle = "VID_A4A5&PID_0556";

    internal static string? FindPort()
    {
        HashSet<string> activePorts = GetActivePorts();
        if (activePorts.Count == 0)
        {
            return null;
        }

        using RegistryKey? usb = Registry.LocalMachine.OpenSubKey(UsbEnumPath);
        if (usb is null)
        {
            return null;
        }

        List<string> ports = new();
        foreach (string hardwareKeyName in usb.GetSubKeyNames())
        {
            if (
                hardwareKeyName.IndexOf(
                    HardwareIdNeedle,
                    StringComparison.OrdinalIgnoreCase
                ) < 0
            )
            {
                continue;
            }

            using RegistryKey? hardwareKey = usb.OpenSubKey(hardwareKeyName);
            if (hardwareKey is null)
            {
                continue;
            }
            foreach (string instanceName in hardwareKey.GetSubKeyNames())
            {
                using RegistryKey? parameters = hardwareKey.OpenSubKey(
                    instanceName + @"\Device Parameters"
                );
                string? port = parameters?.GetValue("PortName") as string;
                if (
                    port is not null &&
                    port.StartsWith("COM", StringComparison.OrdinalIgnoreCase) &&
                    int.TryParse(port.AsSpan(3), out _) &&
                    activePorts.Contains(port)
                )
                {
                    ports.Add(port.ToUpperInvariant());
                }
            }
        }

        return ports
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(ParsePortNumber)
            .FirstOrDefault();
    }

    private static HashSet<string> GetActivePorts()
    {
        HashSet<string> ports = new(StringComparer.OrdinalIgnoreCase);
        using RegistryKey? serialMap = Registry.LocalMachine.OpenSubKey(
            SerialMapPath
        );
        if (serialMap is null)
        {
            return ports;
        }
        foreach (string valueName in serialMap.GetValueNames())
        {
            if (serialMap.GetValue(valueName) is string port)
            {
                ports.Add(port);
            }
        }
        return ports;
    }

    private static int ParsePortNumber(string port)
    {
        return int.TryParse(port.AsSpan(3), out int number)
            ? number
            : int.MaxValue;
    }
}

internal sealed record CdcFrame(uint Sequence, ushort Flags, byte[] Pcm)
{
    internal const int SampleCount = 16;

    internal int Peak
    {
        get
        {
            int peak = 0;
            for (int offset = 0; offset < Pcm.Length; offset += 2)
            {
                short sample = BitConverter.ToInt16(Pcm, offset);
                int magnitude = sample == short.MinValue
                    ? 32768
                    : Math.Abs(sample);
                peak = Math.Max(peak, magnitude);
            }
            return peak;
        }
    }
}

internal sealed class CdcConnection : IDisposable
{
    private const uint Magic = 0x38383539;
    private const int FrameBytes = 64;
    private readonly FileStream stream;
    private readonly List<byte> pending = new(4096);
    private readonly byte[] readBuffer = new byte[1024];

    private CdcConnection(string port, FileStream stream)
    {
        Port = port;
        this.stream = stream;
    }

    internal string Port { get; }

    internal static CdcConnection Open(string port)
    {
        string normalized = port.StartsWith(@"\\.\", StringComparison.Ordinal)
            ? port
            : $@"\\.\{port}";
        SafeFileHandle handle = NativeMethods.CreateFile(
            normalized,
            NativeMethods.GenericRead | NativeMethods.GenericWrite,
            0,
            IntPtr.Zero,
            NativeMethods.OpenExisting,
            NativeMethods.FileFlagOverlapped,
            IntPtr.Zero
        );
        if (handle.IsInvalid)
        {
            int error = Marshal.GetLastWin32Error();
            handle.Dispose();
            throw new Win32Exception(error, $"Unable to open CDC port {port}.");
        }

        try
        {
            NativeMethods.ConfigureSerial(handle);
            FileStream fileStream = new(
                handle,
                FileAccess.ReadWrite,
                4096,
                true
            );
            return new CdcConnection(port, fileStream);
        }
        catch
        {
            handle.Dispose();
            throw;
        }
    }

    internal static async Task<CdcConnection> WaitForAsync(
        string port,
        TimeSpan timeout,
        CancellationToken cancellationToken
    )
    {
        Stopwatch elapsed = Stopwatch.StartNew();
        Win32Exception? lastError = null;
        while (elapsed.Elapsed < timeout)
        {
            cancellationToken.ThrowIfCancellationRequested();
            try
            {
                return Open(port);
            }
            catch (Win32Exception error)
                when (error.NativeErrorCode is 2 or 3 or 5 or 32)
            {
                lastError = error;
                await Task.Delay(250, cancellationToken);
            }
        }
        throw new TimeoutException(
            $"CDC port {port} did not become openable within " +
            $"{timeout.TotalSeconds:F0} seconds. Last error: " +
            $"{lastError?.Message ?? "unknown"}"
        );
    }

    internal async Task<CdcFrame> ReadFrameAsync(
        TimeSpan timeout,
        CancellationToken cancellationToken
    )
    {
        using CancellationTokenSource deadline =
            CancellationTokenSource.CreateLinkedTokenSource(cancellationToken);
        deadline.CancelAfter(timeout);
        try
        {
            while (true)
            {
                int frameOffset = FindFrame();
                if (frameOffset >= 0)
                {
                    if (frameOffset != 0)
                    {
                        pending.RemoveRange(0, frameOffset);
                    }
                    if (pending.Count >= FrameBytes && FrameIsValid(0))
                    {
                        uint sequence = ReadUInt32(4);
                        ushort flags = ReadUInt16(14);
                        byte[] pcm = new byte[CdcFrame.SampleCount * 2];
                        pending.CopyTo(16, pcm, 0, pcm.Length);
                        pending.RemoveRange(0, FrameBytes);
                        return new CdcFrame(sequence, flags, pcm);
                    }
                    if (pending.Count >= FrameBytes)
                    {
                        pending.RemoveAt(0);
                        continue;
                    }
                }

                int count = await stream.ReadAsync(
                    readBuffer.AsMemory(),
                    deadline.Token
                );
                if (count == 0)
                {
                    throw new IOException(
                        "The CDC port closed while bridging audio."
                    );
                }
                for (int index = 0; index < count; ++index)
                {
                    pending.Add(readBuffer[index]);
                }
                if (pending.Count > 16384)
                {
                    pending.RemoveRange(0, pending.Count - FrameBytes);
                }
            }
        }
        catch (OperationCanceledException)
            when (
                deadline.IsCancellationRequested &&
                !cancellationToken.IsCancellationRequested
            )
        {
            throw new TimeoutException(
                "The CDC bulk IN endpoint produced no valid frame for " +
                $"{timeout.TotalSeconds:F0} seconds."
            );
        }
    }

    public void Dispose()
    {
        stream.Dispose();
    }

    private int FindFrame()
    {
        for (int index = 0; index + 3 < pending.Count; ++index)
        {
            if (ReadUInt32(index) == Magic)
            {
                return index;
            }
        }
        if (pending.Count > 3)
        {
            pending.RemoveRange(0, pending.Count - 3);
        }
        return -1;
    }

    private bool FrameIsValid(int offset)
    {
        uint sequence = ReadUInt32(offset + 4);
        uint complement = ReadUInt32(offset + 60);
        ushort sampleCount = ReadUInt16(offset + 12);
        return complement == ~sequence && sampleCount == CdcFrame.SampleCount;
    }

    private ushort ReadUInt16(int offset)
    {
        return (ushort)(
            pending[offset] |
            (pending[offset + 1] << 8)
        );
    }

    private uint ReadUInt32(int offset)
    {
        return (uint)(
            pending[offset] |
            (pending[offset + 1] << 8) |
            (pending[offset + 2] << 16) |
            (pending[offset + 3] << 24)
        );
    }
}

internal static class NativeMethods
{
    internal const uint GenericRead = 0x80000000;
    internal const uint GenericWrite = 0x40000000;
    internal const uint OpenExisting = 3;
    internal const uint FileFlagOverlapped = 0x40000000;

    [StructLayout(LayoutKind.Sequential)]
    private struct Dcb
    {
        internal int Length;
        internal uint BaudRate;
        internal uint Flags;
        internal ushort Reserved;
        internal ushort XonLimit;
        internal ushort XoffLimit;
        internal byte ByteSize;
        internal byte Parity;
        internal byte StopBits;
        internal byte XonChar;
        internal byte XoffChar;
        internal byte ErrorChar;
        internal byte EofChar;
        internal byte EventChar;
        internal ushort Reserved1;
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    internal static extern SafeFileHandle CreateFile(
        string fileName,
        uint desiredAccess,
        uint shareMode,
        IntPtr securityAttributes,
        uint creationDisposition,
        uint flagsAndAttributes,
        IntPtr templateFile
    );

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool GetCommState(
        SafeFileHandle file,
        ref Dcb dcb
    );

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool SetCommState(
        SafeFileHandle file,
        ref Dcb dcb
    );

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool PurgeComm(
        SafeFileHandle file,
        uint flags
    );

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool EscapeCommFunction(
        SafeFileHandle file,
        uint function
    );

    internal static void ConfigureSerial(SafeFileHandle handle)
    {
        Dcb dcb = new()
        {
            Length = Marshal.SizeOf<Dcb>()
        };
        if (!GetCommState(handle, ref dcb))
        {
            throw new Win32Exception(
                Marshal.GetLastWin32Error(),
                "GetCommState failed for the CDC port."
            );
        }

        dcb.BaudRate = 115200;
        dcb.Flags |= 0x00000001;
        dcb.Flags &= ~0x00000002u;
        dcb.Flags = (dcb.Flags & ~0x00000030u) | 0x00000010u;
        dcb.Flags = (dcb.Flags & ~0x00003000u) | 0x00001000u;
        dcb.ByteSize = 8;
        dcb.Parity = 0;
        dcb.StopBits = 0;
        if (!SetCommState(handle, ref dcb))
        {
            throw new Win32Exception(
                Marshal.GetLastWin32Error(),
                "SetCommState failed for the CDC port."
            );
        }

        _ = PurgeComm(handle, 0x0000000f);
        _ = EscapeCommFunction(handle, 5);
        _ = EscapeCommFunction(handle, 3);
    }
}
