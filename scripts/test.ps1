# SPDX-License-Identifier: Apache-2.0

param(
    [switch]$SkipDevice,
    [switch]$SkipHost
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot

$mainPath = Join-Path $projectRoot 'device\src\main.c'
$corePath = Join-Path $projectRoot 'device\src\usb_cdc_pcm_core.c'
$hostPath = Join-Path $projectRoot 'host\src\Program.cs'
$bridgeProject = Join-Path `
    $projectRoot `
    'host\bridge\Usb9588MicBridge.csproj'
$installerScript = Join-Path `
    $projectRoot `
    'installer\9588-usb-mic.iss'

$main = Get-Content -LiteralPath $mainPath -Raw
$core = Get-Content -LiteralPath $corePath -Raw
$hostSource = Get-Content -LiteralPath $hostPath -Raw

if ($main -notmatch '#define USB_PCM_CDC_C3_PROBE 1') {
    throw 'Device main.c does not select the C3 profile.'
}
if ($main -notmatch '#define USB_PCM_RELEASE_UI 1') {
    throw 'Device main.c does not select the release recording UI.'
}
if (
    -not $core.Contains(
        '#define PROBE_LOG_PATH       APP_DATA_DIR "\\9588usbmic.log"'
    ) -or
    -not $core.Contains('"STOP & EXIT"') -or
    -not $core.Contains('#define UI_WAVE_GAIN           2') -or
    -not $core.Contains('static void log_release_summary(void)') -or
    -not $core.Contains(
        'USB storage is unavailable until restart.'
    )
) {
    throw 'Release BDA UI, log path, or restart warning is incomplete.'
}

$audioStart = $core.IndexOf('static void service_audio_in(void) {')
$audioEnd = $core.IndexOf(
    'static void service_cdc_out(void) {',
    $audioStart
)
if ($audioStart -lt 0 -or $audioEnd -lt 0) {
    throw 'CDC audio service source block was not found.'
}
$audio = $core.Substring($audioStart, $audioEnd - $audioStart)
$cdcStart = $audio.IndexOf('#if USB_PCM_CDC_PROBE')
$cdcEnd = $audio.IndexOf('#elif USB_PCM_HID_PROBE', $cdcStart)
if ($cdcStart -lt 0 -or $cdcEnd -lt 0) {
    throw 'CDC audio conditional block was not found.'
}
$cdcAudio = $audio.Substring($cdcStart, $cdcEnd - $cdcStart)

$confirmCount = [regex]::Matches(
    $cdcAudio,
    [regex]::Escape('REG32(USB_AUDIO_IN_CONFIRM) = 0u;')
).Count
$packetCount = [regex]::Matches(
    $cdcAudio,
    [regex]::Escape('++g_ep1_packet_count;')
).Count
$byteCount = [regex]::Matches(
    $cdcAudio,
    [regex]::Escape('g_ep1_byte_count += CDC_FRAME_BYTES;')
).Count
if ($confirmCount -ne 1 -or $packetCount -ne 1 -or $byteCount -ne 1) {
    throw (
        'CDC frame finalization must occur exactly once: ' +
        "confirm=$confirmCount packet=$packetCount bytes=$byteCount"
    )
}

if (
    $hostSource.Contains('HidConnection') -or
    $hostSource.Contains('"--transport"')
) {
    throw 'The standalone host tool must remain CDC-only.'
}
if (
    -not $hostSource.Contains('private const uint Magic = 0x38383539;') -or
    -not $hostSource.Contains('private const int PayloadBytes = 64;') -or
    -not $hostSource.Contains('private const int SampleRate = 16000;')
) {
    throw 'Host protocol constants do not match the documented baseline.'
}

$bridgeProjectText = Get-Content -LiteralPath $bridgeProject -Raw
$bridgeSinkText = Get-Content -LiteralPath (
    Join-Path $projectRoot 'host\bridge\VirtualCableSink.cs'
) -Raw
$installerText = Get-Content -LiteralPath $installerScript -Raw
$endpointInstallerText = Get-Content -LiteralPath (
    Join-Path $projectRoot 'host\bridge\AudioEndpointInstaller.cs'
) -Raw
if (
    -not $bridgeProjectText.Contains(
        '<TargetFramework>net10.0-windows</TargetFramework>'
    ) -or
    -not $bridgeProjectText.Contains(
        '<PackageReference Include="NAudio" Version="2.3.0" />'
    )
) {
    throw 'Virtual microphone bridge runtime or NAudio pin changed.'
}
if (
    -not $bridgeSinkText.Contains('WdlResamplingSampleProvider') -or
    $bridgeSinkText.Contains('MediaFoundationResampler')
) {
    throw 'Virtual microphone bridge must use streaming WDL resampling.'
}
if (
    -not $installerText.Contains(
        'B950E39F01AF1D04EA623C8F6D8EB9B6EA5C477C637295FABF20631C85116BFB'
    ) -or
    -not $installerText.Contains(
        'https://download.vb-audio.com/Download_CABLE/'
    )
) {
    throw 'Installer VB-CABLE source or integrity pin changed.'
}
if (
    -not $endpointInstallerText.Contains('SetDefaultEndpoint') -or
    -not $endpointInstallerText.Contains('CABLE Output') -or
    -not $endpointInstallerText.Contains(
        'Environment.SpecialFolder.LocalApplicationData'
    ) -or
    -not $installerText.Contains('ExecAsOriginalUser') -or
    -not $installerText.Contains('CloseApplications=force') -or
    -not $installerText.Contains('--save-default-playback') -or
    -not $installerText.Contains('--restore-default-playback') -or
    -not $installerText.Contains('--set-default-cable-microphone')
) {
    throw 'Installer default audio endpoint preservation is incomplete.'
}

if (-not $SkipDevice) {
    & (Join-Path $projectRoot 'device\build.ps1')
    if ($LASTEXITCODE -ne 0) {
        throw "Device verification failed with code $LASTEXITCODE"
    }
}
if (-not $SkipHost) {
    & (Join-Path $projectRoot 'host\build.ps1')
    if ($LASTEXITCODE -ne 0) {
        throw "Host verification failed with code $LASTEXITCODE"
    }
    $hostExe = Join-Path $projectRoot 'out\host\UsbCdcPcmCapture.exe'
    $helpText = (& $hostExe --help | Out-String)
    if ($LASTEXITCODE -ne 0 -or -not $helpText.Contains('--port COMx')) {
        throw 'Host command-line smoke test failed.'
    }

    $dotnet10 = Join-Path $projectRoot 'out\tools\dotnet\dotnet.exe'
    if (Test-Path -LiteralPath $dotnet10) {
        $bridgeOutput = Join-Path $projectRoot 'out\bridge-test'
        & $dotnet10 publish $bridgeProject `
            --configuration Release `
            --runtime win-x64 `
            --self-contained true `
            --output $bridgeOutput `
            --nologo `
            -p:PublishSingleFile=true `
            -p:IncludeNativeLibrariesForSelfExtract=true `
            -p:DebugType=None `
            -p:DebugSymbols=false
        if ($LASTEXITCODE -ne 0) {
            throw "Bridge verification failed with code $LASTEXITCODE"
        }
        $bridgeExe = Join-Path $bridgeOutput 'Usb9588MicBridge.exe'
        $bridgeProcess = Start-Process `
            -FilePath $bridgeExe `
            -ArgumentList '--smoke-test' `
            -Wait `
            -PassThru `
            -WindowStyle Hidden
        if ($bridgeProcess.ExitCode -ne 0) {
            throw (
                'Virtual microphone bridge smoke test failed with code ' +
                $bridgeProcess.ExitCode
            )
        }
    }
    else {
        Write-Warning (
            'Skipping virtual microphone bridge build because the .NET 10 ' +
            'SDK is not available at out\tools\dotnet\dotnet.exe.'
        )
    }
}

Write-Host 'usb-cdc-pcm offline verification passed.'
