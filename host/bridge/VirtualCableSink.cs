// SPDX-License-Identifier: Apache-2.0

using NAudio.CoreAudioApi;
using NAudio.Wave;
using NAudio.Wave.SampleProviders;

namespace Usb9588MicBridge;

internal sealed class VirtualCableSink : IDisposable
{
    private const int SourceSampleRate = 16000;
    private static readonly byte[] Silence = new byte[SourceSampleRate / 5 * 2];
    private readonly MMDevice endpoint;
    private readonly BufferedWaveProvider source;
    private readonly WasapiOut output;

    private VirtualCableSink(
        MMDevice endpoint,
        BufferedWaveProvider source,
        WasapiOut output,
        WaveFormat outputFormat
    )
    {
        this.endpoint = endpoint;
        this.source = source;
        this.output = output;
        EndpointName = endpoint.FriendlyName;
        OutputFormat = outputFormat.ToString();
    }

    internal string EndpointName { get; }
    internal string OutputFormat { get; }
    internal double BufferedMilliseconds => source.BufferedDuration.TotalMilliseconds;

    internal static VirtualCableSink? TryOpen()
    {
        using MMDeviceEnumerator enumerator = new();
        MMDevice? selected = enumerator
            .EnumerateAudioEndPoints(DataFlow.Render, DeviceState.Active)
            .Where(IsVirtualCableInput)
            .OrderBy(device => device.FriendlyName, StringComparer.OrdinalIgnoreCase)
            .FirstOrDefault();
        if (selected is null)
        {
            return null;
        }

        MMDevice endpoint = selected;
        try
        {
            WaveFormat outputFormat;
            using (AudioClient audioClient = endpoint.AudioClient)
            {
                outputFormat = audioClient.MixFormat;
            }
            BufferedWaveProvider source = new(
                new WaveFormat(SourceSampleRate, 16, 1)
            )
            {
                BufferDuration = TimeSpan.FromMilliseconds(250),
                DiscardOnBufferOverflow = true,
                ReadFully = true
            };
            source.AddSamples(Silence, 0, SourceSampleRate / 20 * 2);

            ISampleProvider resampled = new WdlResamplingSampleProvider(
                source.ToSampleProvider(),
                outputFormat.SampleRate
            );
            ISampleProvider channelMapped = outputFormat.Channels switch
            {
                1 => resampled,
                2 => new MonoToStereoSampleProvider(resampled),
                _ => throw new NotSupportedException(
                    $"VB-CABLE mix format has unsupported channel count " +
                    $"{outputFormat.Channels}."
                )
            };
            IWaveProvider outputProvider = new SampleToWaveProvider(
                channelMapped
            );
            WasapiOut output = new(
                endpoint,
                AudioClientShareMode.Shared,
                true,
                50
            );
            try
            {
                output.Init(outputProvider);
                output.Play();
                return new VirtualCableSink(
                    endpoint,
                    source,
                    output,
                    outputFormat
                );
            }
            catch
            {
                output.Dispose();
                throw;
            }
        }
        catch
        {
            endpoint.Dispose();
            throw;
        }
    }

    internal void AddPcm(byte[] pcm)
    {
        source.AddSamples(pcm, 0, pcm.Length);
    }

    internal void AddSilence(int sampleCount)
    {
        int remainingBytes = checked(sampleCount * 2);
        while (remainingBytes != 0)
        {
            int count = Math.Min(remainingBytes, Silence.Length);
            source.AddSamples(Silence, 0, count);
            remainingBytes -= count;
        }
    }

    public void Dispose()
    {
        output.Stop();
        output.Dispose();
        endpoint.Dispose();
    }

    private static bool IsVirtualCableInput(MMDevice device)
    {
        string name = device.FriendlyName;
        return
            name.IndexOf(
                "CABLE Input",
                StringComparison.OrdinalIgnoreCase
            ) >= 0 &&
            name.IndexOf(
                "VB-Audio",
                StringComparison.OrdinalIgnoreCase
            ) >= 0;
    }
}
