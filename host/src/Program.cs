// SPDX-License-Identifier: Apache-2.0

using System.ComponentModel;
using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Win32.SafeHandles;

namespace UsbCdcPcmCapture;

internal static class Program
{
    private const uint Magic = 0x38383539;
    private const int PayloadBytes = 64;
    private const int SamplesPerReport = 16;
    private const int SampleRate = 16000;
    private static StreamWriter? logWriter;

    [STAThread]
    private static async Task<int> Main(string[] args)
    {
        if (args.Length == 1 && args[0] is "-h" or "--help")
        {
            Console.WriteLine(Options.HelpText);
            return 0;
        }

        Options options;
        try
        {
            options = Options.Parse(args);
        }
        catch (ArgumentException error)
        {
            Console.Error.WriteLine(error.Message);
            return 2;
        }

        Directory.CreateDirectory(Path.GetDirectoryName(options.WavPath) ?? ".");
        Directory.CreateDirectory(Path.GetDirectoryName(options.PngPath) ?? ".");
        Directory.CreateDirectory(Path.GetDirectoryName(options.LogPath) ?? ".");
        using StreamWriter writer = new(options.LogPath, false, new UTF8Encoding(false))
        {
            AutoFlush = true
        };
        logWriter = writer;

        WaveformHost? waveform = null;
        try
        {
            Log($"opening transport=cdc port={options.Port}");
            using IReportConnection connection =
                await SerialConnection.WaitForAsync(
                    options.Port,
                    TimeSpan.FromSeconds(options.WaitSeconds)
                );
            Log("transport=cdc");
            Log($"device product={connection.Product}");
            Log($"device path={connection.Path}");
            Log($"frame_bytes={connection.InputReportBytes}");

            if (!options.NoWindow)
            {
                waveform = new WaveformHost();
                waveform.Start();
            }

            CaptureResult result = await CaptureAsync(
                connection,
                options.CaptureSeconds,
                waveform
            );
            WriteWaveFile(options.WavPath, result.Samples);
            WriteWaveformPng(
                options.PngPath,
                result.Samples,
                result
            );

            Log($"capture_seconds={result.Elapsed.TotalSeconds:F3}");
            Log($"valid_reports={result.ValidReports}");
            Log($"invalid_reports={result.InvalidReports}");
            Log($"sequence_gaps={result.SequenceGaps}");
            Log($"checksum_errors={result.ChecksumErrors}");
            Log($"device_underrun_reports={result.DeviceUnderrunReports}");
            Log($"samples={result.Samples.Count}");
            Log($"nonzero_samples={result.NonzeroSamples}");
            Log($"peak={result.Peak}");
            Log($"wav={options.WavPath}");
            Log($"waveform={options.PngPath}");

            int minimumReports = Math.Max(100, options.CaptureSeconds * 700);
            if (result.ValidReports < minimumReports)
            {
                Log($"capture_failed reason=too_few_reports minimum={minimumReports}");
                return 4;
            }
            if (result.ChecksumErrors != 0)
            {
                Log("capture_failed reason=report_integrity");
                return 5;
            }

            Log("capture_ok");
            return 0;
        }
        catch (TimeoutException error)
        {
            Log($"capture_failed reason=timeout message={error.Message}");
            return 3;
        }
        catch (Exception error)
        {
            Log($"capture_failed type={error.GetType().Name} message={error.Message}");
            Log(error.ToString());
            return 1;
        }
        finally
        {
            waveform?.Dispose();
            logWriter = null;
        }
    }

    private static async Task<CaptureResult> CaptureAsync(
        IReportConnection connection,
        int seconds,
        WaveformHost? waveform
    )
    {
        byte[] report = new byte[connection.InputReportBytes];
        List<short> samples = new(seconds * SampleRate);
        List<short> uiSamples = new(1024);
        Stopwatch elapsed = Stopwatch.StartNew();
        Stopwatch progress = Stopwatch.StartNew();
        Stopwatch uiUpdate = Stopwatch.StartNew();
        uint expectedSequence = 0;
        bool haveSequence = false;
        int validReports = 0;
        int invalidReports = 0;
        long sequenceGaps = 0;
        int checksumErrors = 0;
        int deviceUnderrunReports = 0;
        long nonzeroSamples = 0;
        int peak = 0;
        int lastProgressSecond = 0;
        int targetSamples = checked(seconds * SampleRate);

        Log($"capture_begin seconds={seconds} target_samples={targetSamples}");
        while (samples.Count < targetSamples)
        {
            int count = await connection.ReadReportAsync(
                report,
                TimeSpan.FromSeconds(2)
            );
            if (count < PayloadBytes)
            {
                ++invalidReports;
                continue;
            }

            uint magic = BitConverter.ToUInt32(report, 0);
            uint sequence = BitConverter.ToUInt32(report, 4);
            uint complement = BitConverter.ToUInt32(report, 60);
            ushort sampleCount = BitConverter.ToUInt16(report, 12);
            ushort flags = BitConverter.ToUInt16(report, 14);
            if (magic != Magic || sampleCount != SamplesPerReport)
            {
                ++invalidReports;
                continue;
            }
            if (complement != ~sequence)
            {
                ++checksumErrors;
                ++invalidReports;
                continue;
            }

            if (haveSequence && sequence != expectedSequence)
            {
                uint distance = unchecked(sequence - expectedSequence);
                if (distance < 10000)
                {
                    sequenceGaps += distance;
                    int missingSamples = checked((int)distance * SamplesPerReport);
                    for (int i = 0; i < missingSamples; ++i)
                    {
                        samples.Add(0);
                        uiSamples.Add(0);
                    }
                }
            }
            expectedSequence = unchecked(sequence + 1);
            haveSequence = true;
            ++validReports;
            if ((flags & 2) != 0)
            {
                ++deviceUnderrunReports;
            }

            for (int i = 0; i < SamplesPerReport; ++i)
            {
                short sample = BitConverter.ToInt16(
                    report,
                    16 + i * 2
                );
                samples.Add(sample);
                uiSamples.Add(sample);
                if (sample != 0)
                {
                    ++nonzeroSamples;
                }
                int magnitude = sample == short.MinValue
                    ? 32768
                    : Math.Abs(sample);
                peak = Math.Max(peak, magnitude);
            }

            if (waveform is not null && uiUpdate.ElapsedMilliseconds >= 40)
            {
                waveform.Post(
                    uiSamples.ToArray(),
                    validReports,
                    sequenceGaps,
                    peak
                );
                uiSamples.Clear();
                uiUpdate.Restart();
            }

            int currentSecond = (int)progress.Elapsed.TotalSeconds;
            if (currentSecond > lastProgressSecond)
            {
                lastProgressSecond = currentSecond;
                Log(
                    $"progress seconds={currentSecond} reports={validReports} " +
                    $"gaps={sequenceGaps} underruns={deviceUnderrunReports} " +
                    $"peak={peak}"
                );
            }
        }

        if (waveform is not null && uiSamples.Count != 0)
        {
            waveform.Post(uiSamples.ToArray(), validReports, sequenceGaps, peak);
        }
        return new CaptureResult(
            samples,
            elapsed.Elapsed,
            validReports,
            invalidReports,
            sequenceGaps,
            checksumErrors,
            deviceUnderrunReports,
            nonzeroSamples,
            peak
        );
    }

    private static void WriteWaveFile(string path, IReadOnlyList<short> samples)
    {
        using FileStream stream = File.Create(path);
        using BinaryWriter writer = new(stream, Encoding.ASCII, false);
        int dataBytes = checked(samples.Count * sizeof(short));
        writer.Write(Encoding.ASCII.GetBytes("RIFF"));
        writer.Write(36 + dataBytes);
        writer.Write(Encoding.ASCII.GetBytes("WAVE"));
        writer.Write(Encoding.ASCII.GetBytes("fmt "));
        writer.Write(16);
        writer.Write((short)1);
        writer.Write((short)1);
        writer.Write(SampleRate);
        writer.Write(SampleRate * sizeof(short));
        writer.Write((short)sizeof(short));
        writer.Write((short)16);
        writer.Write(Encoding.ASCII.GetBytes("data"));
        writer.Write(dataBytes);
        foreach (short sample in samples)
        {
            writer.Write(sample);
        }
    }

    private static void WriteWaveformPng(
        string path,
        IReadOnlyList<short> samples,
        CaptureResult result
    )
    {
        const int width = 1280;
        const int height = 420;
        using Bitmap bitmap = new(width, height);
        using Graphics graphics = Graphics.FromImage(bitmap);
        graphics.SmoothingMode = SmoothingMode.AntiAlias;
        graphics.Clear(Color.FromArgb(15, 19, 22));
        using Pen gridPen = new(Color.FromArgb(42, 51, 57), 1);
        using Pen centerPen = new(Color.FromArgb(83, 96, 104), 1);
        using Pen wavePen = new(Color.FromArgb(67, 213, 166), 1.2f);
        using Brush textBrush = new SolidBrush(Color.FromArgb(222, 229, 232));
        using Font font = new("Segoe UI", 12);

        for (int x = 0; x < width; x += 80)
        {
            graphics.DrawLine(gridPen, x, 48, x, height);
        }
        for (int y = 48; y < height; y += 62)
        {
            graphics.DrawLine(gridPen, 0, y, width, y);
        }
        int center = (height + 48) / 2;
        graphics.DrawLine(centerPen, 0, center, width, center);

        if (samples.Count != 0)
        {
            float scale = (height - 70) / 2f / 32768f;
            for (int x = 0; x < width; ++x)
            {
                int begin = (int)((long)x * samples.Count / width);
                int end = Math.Max(
                    begin + 1,
                    (int)((long)(x + 1) * samples.Count / width)
                );
                end = Math.Min(end, samples.Count);
                short minimum = short.MaxValue;
                short maximum = short.MinValue;
                for (int i = begin; i < end; ++i)
                {
                    minimum = Math.Min(minimum, samples[i]);
                    maximum = Math.Max(maximum, samples[i]);
                }
                graphics.DrawLine(
                    wavePen,
                    x,
                    center - maximum * scale,
                    x,
                    center - minimum * scale
                );
            }
        }

        string title =
            "9588 CDC PCM  |  " +
            $"reports {result.ValidReports}  |  " +
            $"gaps {result.SequenceGaps}  |  peak {result.Peak}";
        graphics.DrawString(title, font, textBrush, 16, 15);
        bitmap.Save(path, System.Drawing.Imaging.ImageFormat.Png);
    }

    private static void Log(string text)
    {
        string line = $"{DateTime.Now:HH:mm:ss.fff} {text}";
        Console.WriteLine(line);
        logWriter?.WriteLine(line);
    }
}

internal sealed record CaptureResult(
    List<short> Samples,
    TimeSpan Elapsed,
    int ValidReports,
    int InvalidReports,
    long SequenceGaps,
    int ChecksumErrors,
    int DeviceUnderrunReports,
    long NonzeroSamples,
    int Peak
);

internal sealed record Options(
    int WaitSeconds,
    int CaptureSeconds,
    string WavPath,
    string PngPath,
    string LogPath,
    string Port,
    bool NoWindow
)
{
    internal const string HelpText =
        "UsbCdcPcmCapture --port COMx [options]\n" +
        "\n" +
        "Options:\n" +
        "  --port COMx       CDC ACM serial port (required)\n" +
        "  --wait N          Port-open timeout in seconds (default: 120)\n" +
        "  --seconds N       Capture duration in seconds (default: 15)\n" +
        "  --wav PATH        Output PCM WAV path\n" +
        "  --png PATH        Output waveform PNG path\n" +
        "  --log PATH        Output transport log path\n" +
        "  --no-window       Disable the live waveform window\n" +
        "  -h, --help        Show this help";

    public static Options Parse(string[] args)
    {
        int wait = 120;
        int seconds = 15;
        string wav = Path.GetFullPath("usb_cdc_pcm_capture.wav");
        string png = Path.GetFullPath("usb_cdc_pcm_waveform.png");
        string log = Path.GetFullPath("usb_cdc_pcm_capture.log");
        string? port = null;
        bool noWindow = false;

        for (int i = 0; i < args.Length; ++i)
        {
            string value = args[i];
            string Next()
            {
                if (++i >= args.Length)
                {
                    throw new ArgumentException($"Missing value after {value}");
                }
                return args[i];
            }

            switch (value)
            {
                case "--wait":
                    wait = int.Parse(Next());
                    break;
                case "--seconds":
                    seconds = int.Parse(Next());
                    break;
                case "--wav":
                    wav = Path.GetFullPath(Next());
                    break;
                case "--png":
                    png = Path.GetFullPath(Next());
                    break;
                case "--log":
                    log = Path.GetFullPath(Next());
                    break;
                case "--port":
                    port = Next();
                    break;
                case "--no-window":
                    noWindow = true;
                    break;
                default:
                    throw new ArgumentException($"Unknown argument: {value}");
            }
        }
        if (wait <= 0 || seconds <= 0)
        {
            throw new ArgumentException("Wait and capture durations must be positive.");
        }
        if (string.IsNullOrWhiteSpace(port))
        {
            throw new ArgumentException("--port is required.");
        }
        return new Options(
            wait,
            seconds,
            wav,
            png,
            log,
            port,
            noWindow
        );
    }
}

internal interface IReportConnection : IDisposable
{
    string Path { get; }
    string Product { get; }
    int InputReportBytes { get; }
    Task<int> ReadReportAsync(byte[] buffer, TimeSpan timeout);
}

internal sealed class SerialConnection : IReportConnection
{
    private const uint Magic = 0x38383539;
    private const int FrameBytes = 64;
    private const int SamplesPerFrame = 16;
    private readonly FileStream stream;
    private readonly List<byte> pending = new(4096);
    private readonly byte[] readBuffer = new byte[1024];

    private SerialConnection(string port, FileStream stream)
    {
        Path = port;
        Product = "9588 CDC PCM";
        this.stream = stream;
    }

    public string Path { get; }
    public string Product { get; }
    public int InputReportBytes => FrameBytes;

    public static SerialConnection Open(string port)
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
            FileStream stream = new(handle, FileAccess.ReadWrite, 4096, true);
            return new SerialConnection(port, stream);
        }
        catch
        {
            handle.Dispose();
            throw;
        }
    }

    public static async Task<SerialConnection> WaitForAsync(
        string port,
        TimeSpan timeout
    )
    {
        Stopwatch elapsed = Stopwatch.StartNew();
        Win32Exception? lastError = null;
        while (elapsed.Elapsed < timeout)
        {
            try
            {
                return Open(port);
            }
            catch (Win32Exception error)
                when (error.NativeErrorCode is 2 or 3 or 5)
            {
                lastError = error;
                await Task.Delay(250);
            }
        }
        throw new TimeoutException(
            $"CDC port {port} did not become openable within " +
            $"{timeout.TotalSeconds:F0} seconds. Last error: " +
            $"{lastError?.Message ?? "unknown"}"
        );
    }

    public async Task<int> ReadReportAsync(byte[] buffer, TimeSpan timeout)
    {
        if (buffer.Length < FrameBytes)
        {
            throw new ArgumentException("CDC frame buffer is too small.", nameof(buffer));
        }

        using CancellationTokenSource cancellation = new(timeout);
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
                        pending.CopyTo(0, buffer, 0, FrameBytes);
                        pending.RemoveRange(0, FrameBytes);
                        return FrameBytes;
                    }
                    if (pending.Count >= FrameBytes)
                    {
                        pending.RemoveAt(0);
                        continue;
                    }
                }

                int count = await stream.ReadAsync(
                    readBuffer.AsMemory(),
                    cancellation.Token
                );
                if (count == 0)
                {
                    throw new IOException("The CDC port closed while capturing.");
                }
                for (int i = 0; i < count; ++i)
                {
                    pending.Add(readBuffer[i]);
                }
                if (pending.Count > 16384)
                {
                    pending.RemoveRange(0, pending.Count - FrameBytes);
                }
            }
        }
        catch (OperationCanceledException)
        {
            throw new TimeoutException(
                "The CDC bulk IN endpoint produced no valid frame for 2 seconds."
            );
        }
    }

    public void Dispose()
    {
        stream.Dispose();
    }

    private int FindFrame()
    {
        for (int i = 0; i + 3 < pending.Count; ++i)
        {
            if (ReadUInt32(i) == Magic)
            {
                return i;
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
        ushort sampleCount = (ushort)(
            pending[offset + 12] |
            (pending[offset + 13] << 8)
        );
        return complement == ~sequence && sampleCount == SamplesPerFrame;
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

internal sealed class WaveformHost : IDisposable
{
    private readonly ManualResetEventSlim ready = new(false);
    private Thread? thread;
    private WaveformForm? form;

    public void Start()
    {
        thread = new Thread(() =>
        {
            ApplicationConfiguration.Initialize();
            form = new WaveformForm();
            ready.Set();
            Application.Run(form);
        })
        {
            IsBackground = true,
            Name = "9588 CDC PCM waveform"
        };
        thread.SetApartmentState(ApartmentState.STA);
        thread.Start();
        ready.Wait();
    }

    public void Post(short[] samples, int reports, long gaps, int peak)
    {
        WaveformForm? target = form;
        if (target is null || target.IsDisposed)
        {
            return;
        }
        try
        {
            target.BeginInvoke(
                new Action(() => target.Append(samples, reports, gaps, peak))
            );
        }
        catch (InvalidOperationException)
        {
        }
    }

    public void Dispose()
    {
        WaveformForm? target = form;
        if (target is not null && !target.IsDisposed)
        {
            try
            {
                target.BeginInvoke(new Action(target.Close));
            }
            catch (InvalidOperationException)
            {
            }
        }
        thread?.Join(1000);
        ready.Dispose();
    }
}

internal sealed class WaveformForm : Form
{
    private const int Capacity = 3200;
    private readonly short[] samples = new short[Capacity];
    private int sampleCount;
    private int reports;
    private long gaps;
    private int peak;

    public WaveformForm()
    {
        Text = "9588 CDC PCM";
        ClientSize = new Size(960, 360);
        MinimumSize = new Size(640, 280);
        BackColor = Color.FromArgb(15, 19, 22);
        DoubleBuffered = true;
        StartPosition = FormStartPosition.CenterScreen;
    }

    public void Append(short[] incoming, int reportCount, long gapCount, int peakValue)
    {
        if (incoming.Length >= Capacity)
        {
            Array.Copy(
                incoming,
                incoming.Length - Capacity,
                samples,
                0,
                Capacity
            );
            sampleCount = Capacity;
        }
        else
        {
            int overflow = Math.Max(0, sampleCount + incoming.Length - Capacity);
            if (overflow != 0)
            {
                Array.Copy(samples, overflow, samples, 0, sampleCount - overflow);
                sampleCount -= overflow;
            }
            Array.Copy(incoming, 0, samples, sampleCount, incoming.Length);
            sampleCount += incoming.Length;
        }
        reports = reportCount;
        gaps = gapCount;
        peak = peakValue;
        Invalidate();
    }

    protected override void OnPaint(PaintEventArgs eventArgs)
    {
        base.OnPaint(eventArgs);
        Graphics graphics = eventArgs.Graphics;
        graphics.SmoothingMode = SmoothingMode.AntiAlias;
        Rectangle plot = new(18, 54, ClientSize.Width - 36, ClientSize.Height - 76);
        using Pen grid = new(Color.FromArgb(42, 51, 57), 1);
        using Pen center = new(Color.FromArgb(83, 96, 104), 1);
        using Pen wave = new(Color.FromArgb(67, 213, 166), 1.4f);
        using Brush text = new SolidBrush(Color.FromArgb(222, 229, 232));
        using Font font = new("Segoe UI", 11);

        for (int i = 0; i <= 8; ++i)
        {
            int x = plot.Left + plot.Width * i / 8;
            graphics.DrawLine(grid, x, plot.Top, x, plot.Bottom);
        }
        for (int i = 0; i <= 4; ++i)
        {
            int y = plot.Top + plot.Height * i / 4;
            graphics.DrawLine(grid, plot.Left, y, plot.Right, y);
        }
        int centerY = plot.Top + plot.Height / 2;
        graphics.DrawLine(center, plot.Left, centerY, plot.Right, centerY);
        graphics.DrawString(
            $"reports {reports}    gaps {gaps}    peak {peak}",
            font,
            text,
            plot.Left,
            18
        );

        if (sampleCount < 2)
        {
            return;
        }
        PointF[] points = new PointF[Math.Min(plot.Width, sampleCount)];
        for (int x = 0; x < points.Length; ++x)
        {
            int index = x * (sampleCount - 1) / Math.Max(1, points.Length - 1);
            float y = centerY - samples[index] * (plot.Height / 2f) / 32768f;
            points[x] = new PointF(
                plot.Left + x * plot.Width / (float)Math.Max(1, points.Length - 1),
                y
            );
        }
        graphics.DrawLines(wave, points);
    }
}
