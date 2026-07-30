// SPDX-License-Identifier: Apache-2.0

using System.Diagnostics;
using System.Drawing;
using System.Threading;

namespace Usb9588MicBridge;

internal static class Program
{
    private const string MutexName = @"Local\Usb9588MicBridge";

    [STAThread]
    private static int Main(string[] args)
    {
        if (AudioEndpointInstaller.TryRun(args, out int installerExitCode))
        {
            return installerExitCode;
        }

        ApplicationConfiguration.Initialize();
        BridgeLog.Initialize();
        if (args.Length == 1 && args[0] == "--smoke-test")
        {
            string? port = CdcDeviceDiscovery.FindPort();
            Version? naudioVersion = typeof(NAudio.Wave.WasapiOut)
                .Assembly
                .GetName()
                .Version;
            BridgeLog.Write(
                $"smoke_test_ok port={port ?? "none"} " +
                $"naudio={naudioVersion}"
            );
            return 0;
        }

        using Mutex instanceMutex = new(true, MutexName, out bool firstInstance);
        if (!firstInstance)
        {
            return 0;
        }

        BridgeLog.Write("application_start");
        BridgeApplicationContext context = new();
        Application.Run(context);
        BridgeLog.Write("application_stop");
        return 0;
    }
}

internal sealed class BridgeApplicationContext : ApplicationContext
{
    private readonly CancellationTokenSource cancellation = new();
    private readonly NotifyIcon trayIcon;
    private readonly ToolStripMenuItem statusItem;
    private readonly System.Windows.Forms.Timer statusTimer;
    private readonly Task workerTask;
    private string status = "Starting";
    private bool stopping;

    internal BridgeApplicationContext()
    {
        statusItem = new ToolStripMenuItem("Starting")
        {
            Enabled = false
        };
        ContextMenuStrip menu = new();
        menu.Items.Add(statusItem);
        menu.Items.Add(new ToolStripSeparator());
        menu.Items.Add("Open log", null, (_, _) => OpenLog());
        menu.Items.Add(
            "VB-CABLE website",
            null,
            (_, _) => OpenUrl("https://vb-audio.com/Cable/index.htm")
        );
        menu.Items.Add(new ToolStripSeparator());
        menu.Items.Add("Exit", null, (_, _) => ExitThread());

        trayIcon = new NotifyIcon
        {
            ContextMenuStrip = menu,
            Icon = SystemIcons.Application,
            Text = "9588 USB Microphone",
            Visible = true
        };
        trayIcon.DoubleClick += (_, _) => OpenLog();

        statusTimer = new System.Windows.Forms.Timer
        {
            Interval = 500
        };
        statusTimer.Tick += (_, _) =>
        {
            string current = Volatile.Read(ref status);
            statusItem.Text = current;
            trayIcon.Text = current.Length <= 63
                ? current
                : current[..63];
        };
        statusTimer.Start();

        workerTask = Task.Run(() => RunBridgeAsync(cancellation.Token));
    }

    protected override void ExitThreadCore()
    {
        if (stopping)
        {
            return;
        }
        stopping = true;
        statusTimer.Stop();
        cancellation.Cancel();
        try
        {
            workerTask.Wait(TimeSpan.FromSeconds(3));
        }
        catch (AggregateException error)
        {
            BridgeLog.Write($"worker_stop_error={error.GetBaseException().Message}");
        }
        trayIcon.Visible = false;
        trayIcon.Dispose();
        statusTimer.Dispose();
        cancellation.Dispose();
        base.ExitThreadCore();
    }

    private async Task RunBridgeAsync(CancellationToken cancellationToken)
    {
        bool notifiedMissingCable = false;
        while (!cancellationToken.IsCancellationRequested)
        {
            try
            {
                string? port = CdcDeviceDiscovery.FindPort();
                if (port is null)
                {
                    SetStatus("9588 USB Microphone: waiting for device");
                    await Task.Delay(1000, cancellationToken);
                    continue;
                }

                VirtualCableSink? sink = VirtualCableSink.TryOpen();
                if (sink is null)
                {
                    SetStatus("9588 USB Microphone: VB-CABLE not found");
                    if (!notifiedMissingCable)
                    {
                        notifiedMissingCable = true;
                        ShowBalloon(
                            "VB-CABLE is not installed",
                            "Install VB-CABLE, then restart Windows."
                        );
                    }
                    await Task.Delay(3000, cancellationToken);
                    continue;
                }

                notifiedMissingCable = false;
                using (sink)
                using (CdcConnection connection =
                    await CdcConnection.WaitForAsync(
                        port,
                        TimeSpan.FromSeconds(10),
                        cancellationToken
                    ))
                {
                    SetStatus($"9588 USB Microphone: {port} connected");
                    BridgeLog.Write(
                        $"bridge_connected port={port} endpoint={sink.EndpointName} " +
                        $"output_format={sink.OutputFormat}"
                    );
                    await PumpAsync(
                        connection,
                        sink,
                        port,
                        cancellationToken
                    );
                }
            }
            catch (OperationCanceledException)
                when (cancellationToken.IsCancellationRequested)
            {
                break;
            }
            catch (Exception error)
            {
                SetStatus("9588 USB Microphone: reconnecting");
                BridgeLog.Write(
                    $"bridge_error type={error.GetType().Name} " +
                    $"message={error.Message}"
                );
                await DelayAfterFailureAsync(cancellationToken);
            }
        }
    }

    private async Task PumpAsync(
        CdcConnection connection,
        VirtualCableSink sink,
        string port,
        CancellationToken cancellationToken
    )
    {
        uint expectedSequence = 0;
        bool haveSequence = false;
        long reports = 0;
        long sequenceGaps = 0;
        long underruns = 0;
        int peak = 0;
        Stopwatch progress = Stopwatch.StartNew();

        while (!cancellationToken.IsCancellationRequested)
        {
            CdcFrame frame = await connection.ReadFrameAsync(
                TimeSpan.FromSeconds(3),
                cancellationToken
            );
            if (haveSequence && frame.Sequence != expectedSequence)
            {
                uint distance = unchecked(frame.Sequence - expectedSequence);
                if (distance is > 0 and < 1000)
                {
                    sequenceGaps += distance;
                    sink.AddSilence(checked((int)distance * CdcFrame.SampleCount));
                }
            }
            expectedSequence = unchecked(frame.Sequence + 1);
            haveSequence = true;

            sink.AddPcm(frame.Pcm);
            ++reports;
            if ((frame.Flags & 2) != 0)
            {
                ++underruns;
            }
            peak = Math.Max(peak, frame.Peak);

            if (progress.Elapsed >= TimeSpan.FromSeconds(5))
            {
                SetStatus(
                    $"9588 USB Microphone: {port}, peak {peak}"
                );
                BridgeLog.Write(
                    $"bridge_progress port={port} reports={reports} " +
                    $"gaps={sequenceGaps} underruns={underruns} peak={peak} " +
                    $"buffer_ms={sink.BufferedMilliseconds:F0}"
                );
                progress.Restart();
                peak = 0;
            }
        }
    }

    private static async Task DelayAfterFailureAsync(
        CancellationToken cancellationToken
    )
    {
        try
        {
            await Task.Delay(1000, cancellationToken);
        }
        catch (OperationCanceledException)
        {
        }
    }

    private void SetStatus(string value)
    {
        Volatile.Write(ref status, value);
    }

    private void ShowBalloon(string title, string message)
    {
        try
        {
            trayIcon.ShowBalloonTip(
                5000,
                title,
                message,
                ToolTipIcon.Warning
            );
        }
        catch (InvalidOperationException)
        {
        }
    }

    private static void OpenLog()
    {
        BridgeLog.EnsureFileExists();
        Process.Start(new ProcessStartInfo
        {
            FileName = BridgeLog.LogPath,
            UseShellExecute = true
        });
    }

    private static void OpenUrl(string url)
    {
        Process.Start(new ProcessStartInfo
        {
            FileName = url,
            UseShellExecute = true
        });
    }
}
