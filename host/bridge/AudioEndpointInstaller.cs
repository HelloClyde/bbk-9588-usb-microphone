// SPDX-License-Identifier: Apache-2.0

using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;
using NAudio.CoreAudioApi;

namespace Usb9588MicBridge;

internal static class AudioEndpointInstaller
{
    private static readonly Role[] EndpointRoles =
    [
        Role.Console,
        Role.Multimedia,
        Role.Communications
    ];

    internal static bool TryRun(string[] args, out int exitCode)
    {
        bool isSave =
            args.Length is 1 or 2 &&
            args[0].Equals(
                "--save-default-playback",
                StringComparison.OrdinalIgnoreCase
            );
        bool isRestore =
            args.Length is 1 or 2 &&
            args[0].Equals(
                "--restore-default-playback",
                StringComparison.OrdinalIgnoreCase
            );
        bool isSelectMicrophone =
            args.Length == 1 &&
            args[0].Equals(
                "--set-default-cable-microphone",
                StringComparison.OrdinalIgnoreCase
            );
        if (!isSave && !isRestore && !isSelectMicrophone)
        {
            exitCode = 0;
            return false;
        }

        BridgeLog.Initialize();
        try
        {
            if (isSave)
            {
                SaveDefaultPlayback(GetSnapshotPath(args));
            }
            else if (isRestore)
            {
                RestoreDefaultPlayback(GetSnapshotPath(args));
            }
            else
            {
                SetDefaultCableMicrophone();
            }
            exitCode = 0;
        }
        catch (Exception error)
        {
            BridgeLog.Write(
                $"installer_audio_error command={args[0]} " +
                $"type={error.GetType().Name} message={error.Message}"
            );
            exitCode = 1;
        }
        return true;
    }

    private static string GetSnapshotPath(string[] args)
    {
        if (args.Length == 2)
        {
            return args[1];
        }
        return Path.Combine(
            Environment.GetFolderPath(
                Environment.SpecialFolder.LocalApplicationData
            ),
            "9588UsbMic",
            "preinstall-default-playback.json"
        );
    }

    private static void SaveDefaultPlayback(string snapshotPath)
    {
        using MMDeviceEnumerator enumerator = new();
        DefaultPlaybackSnapshot snapshot = new();
        foreach (Role role in EndpointRoles)
        {
            using MMDevice endpoint = enumerator.GetDefaultAudioEndpoint(
                DataFlow.Render,
                role
            );
            snapshot.Set(
                role,
                new EndpointReference(endpoint.ID, endpoint.FriendlyName)
            );
            BridgeLog.Write(
                $"installer_playback_saved role={role} " +
                $"endpoint={endpoint.FriendlyName} id={endpoint.ID}"
            );
        }

        string fullPath = Path.GetFullPath(snapshotPath);
        string? directory = Path.GetDirectoryName(fullPath);
        if (!string.IsNullOrEmpty(directory))
        {
            Directory.CreateDirectory(directory);
        }
        string json = JsonSerializer.Serialize(
            snapshot,
            new JsonSerializerOptions { WriteIndented = true }
        );
        File.WriteAllText(fullPath, json, new UTF8Encoding(false));
    }

    private static void RestoreDefaultPlayback(string snapshotPath)
    {
        string json = File.ReadAllText(
            Path.GetFullPath(snapshotPath),
            Encoding.UTF8
        );
        DefaultPlaybackSnapshot snapshot =
            JsonSerializer.Deserialize<DefaultPlaybackSnapshot>(json) ??
            throw new InvalidDataException(
                "The default playback snapshot is empty."
            );

        using MMDeviceEnumerator enumerator = new();
        using AudioEndpointPolicy policy = new();
        foreach (Role role in EndpointRoles)
        {
            EndpointReference endpointReference =
                snapshot.Get(role) ??
                throw new InvalidDataException(
                    $"The default playback snapshot has no {role} endpoint."
                );
            using MMDevice endpoint = enumerator.GetDevice(
                endpointReference.Id
            );
            if ((endpoint.State & DeviceState.Active) == 0)
            {
                throw new InvalidOperationException(
                    $"The saved {role} playback endpoint is not active: " +
                    endpointReference.Name
                );
            }
            policy.SetDefaultEndpoint(endpointReference.Id, role);
            BridgeLog.Write(
                $"installer_playback_restored role={role} " +
                $"endpoint={endpoint.FriendlyName} id={endpoint.ID}"
            );
        }
        File.Delete(Path.GetFullPath(snapshotPath));
    }

    private static void SetDefaultCableMicrophone()
    {
        DateTime deadline = DateTime.UtcNow.AddSeconds(30);
        do
        {
            using MMDeviceEnumerator enumerator = new();
            MMDevice? endpoint = enumerator
                .EnumerateAudioEndPoints(
                    DataFlow.Capture,
                    DeviceState.Active
                )
                .FirstOrDefault(IsCableOutput);
            if (endpoint is not null)
            {
                using (endpoint)
                using (AudioEndpointPolicy policy = new())
                {
                    foreach (Role role in EndpointRoles)
                    {
                        policy.SetDefaultEndpoint(endpoint.ID, role);
                        BridgeLog.Write(
                            $"installer_microphone_selected role={role} " +
                            $"endpoint={endpoint.FriendlyName} id={endpoint.ID}"
                        );
                    }
                }
                return;
            }
            Thread.Sleep(500);
        }
        while (DateTime.UtcNow < deadline);

        throw new InvalidOperationException(
            "CABLE Output did not become available within 30 seconds."
        );
    }

    private static bool IsCableOutput(MMDevice endpoint)
    {
        string name = endpoint.FriendlyName;
        return
            name.IndexOf(
                "CABLE Output",
                StringComparison.OrdinalIgnoreCase
            ) >= 0 &&
            name.IndexOf(
                "VB-Audio Virtual Cable",
                StringComparison.OrdinalIgnoreCase
            ) >= 0;
    }

    private sealed class DefaultPlaybackSnapshot
    {
        public EndpointReference? Console { get; set; }
        public EndpointReference? Multimedia { get; set; }
        public EndpointReference? Communications { get; set; }

        internal EndpointReference? Get(Role role)
        {
            return role switch
            {
                Role.Console => Console,
                Role.Multimedia => Multimedia,
                Role.Communications => Communications,
                _ => throw new ArgumentOutOfRangeException(nameof(role))
            };
        }

        internal void Set(Role role, EndpointReference endpoint)
        {
            switch (role)
            {
                case Role.Console:
                    Console = endpoint;
                    break;
                case Role.Multimedia:
                    Multimedia = endpoint;
                    break;
                case Role.Communications:
                    Communications = endpoint;
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(role));
            }
        }
    }

    private sealed record EndpointReference(string Id, string Name);
}

internal sealed class AudioEndpointPolicy : IDisposable
{
    private object? client = new PolicyConfigClient();
    private IPolicyConfig Policy =>
        (IPolicyConfig)(
            client ??
            throw new ObjectDisposedException(nameof(AudioEndpointPolicy))
        );

    internal void SetDefaultEndpoint(string endpointId, Role role)
    {
        int result = Policy.SetDefaultEndpoint(endpointId, role);
        Marshal.ThrowExceptionForHR(result);
    }

    public void Dispose()
    {
        object? current = Interlocked.Exchange(ref client, null);
        if (current is not null && Marshal.IsComObject(current))
        {
            Marshal.FinalReleaseComObject(current);
        }
    }

    [ComImport]
    [Guid("870AF99C-171D-4F9E-AF0D-E63DF40C2BC9")]
    private sealed class PolicyConfigClient;

    [ComImport]
    [Guid("F8679F50-850A-41CF-9C72-430F290290C8")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    private interface IPolicyConfig
    {
        [PreserveSig]
        int GetMixFormat(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr format
        );

        [PreserveSig]
        int GetDeviceFormat(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            int defaultFormat,
            IntPtr format
        );

        [PreserveSig]
        int ResetDeviceFormat(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId
        );

        [PreserveSig]
        int SetDeviceFormat(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr endpointFormat,
            IntPtr mixFormat
        );

        [PreserveSig]
        int GetProcessingPeriod(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            int defaultPeriod,
            IntPtr period,
            IntPtr minimumPeriod
        );

        [PreserveSig]
        int SetProcessingPeriod(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr period
        );

        [PreserveSig]
        int GetShareMode(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr mode
        );

        [PreserveSig]
        int SetShareMode(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr mode
        );

        [PreserveSig]
        int GetPropertyValue(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr propertyKey,
            IntPtr propertyValue
        );

        [PreserveSig]
        int SetPropertyValue(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            IntPtr propertyKey,
            IntPtr propertyValue
        );

        [PreserveSig]
        int SetDefaultEndpoint(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            Role role
        );

        [PreserveSig]
        int SetEndpointVisibility(
            [MarshalAs(UnmanagedType.LPWStr)] string deviceId,
            int visible
        );
    }
}
