// SPDX-License-Identifier: Apache-2.0

using System.Text;

namespace Usb9588MicBridge;

internal static class BridgeLog
{
    private static readonly object Sync = new();

    internal static string LogPath { get; } = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "9588UsbMic",
        "bridge.log"
    );

    internal static void Initialize()
    {
        string directory = Path.GetDirectoryName(LogPath)!;
        Directory.CreateDirectory(directory);
        if (
            File.Exists(LogPath) &&
            new FileInfo(LogPath).Length > 2 * 1024 * 1024
        )
        {
            string oldPath = Path.Combine(directory, "bridge.old.log");
            File.Move(LogPath, oldPath, true);
        }
        EnsureFileExists();
    }

    internal static void EnsureFileExists()
    {
        lock (Sync)
        {
            string directory = Path.GetDirectoryName(LogPath)!;
            Directory.CreateDirectory(directory);
            if (!File.Exists(LogPath))
            {
                File.WriteAllText(LogPath, string.Empty, new UTF8Encoding(false));
            }
        }
    }

    internal static void Write(string message)
    {
        string line = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff} {message}";
        lock (Sync)
        {
            File.AppendAllText(
                LogPath,
                line + Environment.NewLine,
                new UTF8Encoding(false)
            );
        }
    }
}
