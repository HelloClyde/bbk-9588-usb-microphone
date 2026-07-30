# SPDX-License-Identifier: Apache-2.0

param(
    [switch]$SkipDevice,
    [switch]$SkipHost
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot

$mainPath = Join-Path $projectRoot 'device\src\main.c'
$corePath = Join-Path $projectRoot 'device\src\usb_cdc_pcm_core.c'
$musbCorePath = Join-Path `
    $projectRoot `
    'device\src\usb_cdc_pcm_musb.c'
$profilePath = Join-Path `
    $projectRoot `
    'device\include\bda_firmware_profile.h'
$audioPath = Join-Path `
    $projectRoot `
    'device\include\bda_firmware_audio.h'
$liveUiPath = Join-Path $projectRoot 'device\include\bda_live_ui.h'
$deviceBuildPath = Join-Path $projectRoot 'device\build.ps1'
$iconPath = Join-Path $projectRoot 'device\assets\9588-usb-mic.png'
$hostPath = Join-Path $projectRoot 'host\src\Program.cs'
$bridgeProject = Join-Path `
    $projectRoot `
    'host\bridge\Usb9588MicBridge.csproj'
$installerScript = Join-Path `
    $projectRoot `
    'installer\9588-usb-mic.iss'
$releaseWorkflowPath = Join-Path `
    $projectRoot `
    '.github\workflows\release.yml'
$hostWorkflowPath = Join-Path `
    $projectRoot `
    '.github\workflows\host-ci.yml'
$packageReleasePath = Join-Path `
    $projectRoot `
    'scripts\package-release.ps1'
$gitModulesPath = Join-Path $projectRoot '.gitmodules'
$sdkHardwarePath = Join-Path `
    $projectRoot `
    'sdk\sdk\include\bda_hardware.h'

$main = Get-Content -LiteralPath $mainPath -Raw
$core = Get-Content -LiteralPath $corePath -Raw
$musbCore = Get-Content -LiteralPath $musbCorePath -Raw
$profiles = Get-Content -LiteralPath $profilePath -Raw
$audio = Get-Content -LiteralPath $audioPath -Raw
$liveUi = Get-Content -LiteralPath $liveUiPath -Raw
$deviceBuild = Get-Content -LiteralPath $deviceBuildPath -Raw
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
        'Restart the device before reconnecting USB.'
    ) -or
    $core.Contains('Restart the 9588 before reconnecting USB.')
) {
    throw 'Release BDA UI, log path, or restart warning is incomplete.'
}
if (
    -not (Test-Path -LiteralPath $iconPath) -or
    -not $deviceBuild.Contains("'--icon-png', `$IconPath")
) {
    throw 'Release BDA icon asset is missing from the device build.'
}
if (
    -not $main.Contains('#include "usb_cdc_pcm_core.c"') -or
    -not $main.Contains('#include "usb_cdc_pcm_musb.c"') -or
    -not $main.Contains('profile->udc_kind == BDA_UDC_PCH_STYLE') -or
    -not $main.Contains('profile->udc_kind == BDA_UDC_MUSB') -or
    -not $main.Contains('usb_cdc_pcm_pch_run()') -or
    -not $main.Contains('usb_cdc_pcm_musb_run()') -or
    -not $core.Contains('static int usb_cdc_pcm_pch_run(void)') -or
    -not $musbCore.Contains('static int usb_cdc_pcm_musb_run(void)') -or
    $deviceBuild.Contains('src\main_musb.c')
) {
    throw 'Unified PCH/MUSB runtime dispatch is incomplete.'
}
if (
    ([regex]::Matches(
        $main,
        [regex]::Escape('int bda_main(void)')
    ).Count -ne 1) -or
    -not $deviceBuild.Contains("'9588UsbMic-MUSB.bda'") -or
    -not $deviceBuild.Contains("'UsbCdcPcm.bda'") -or
    -not $deviceBuild.Contains(
        'Remove-Item -LiteralPath $obsoleteOutput'
    )
) {
    throw 'The device build must publish exactly one BDA entry artifact.'
}
foreach ($profileName in @(
    'BDA_FIRMWARE_C200_JZ4720',
    'BDA_FIRMWARE_C200_JZ4730',
    'BDA_FIRMWARE_C200_JZ4740',
    'BDA_FIRMWARE_C100_JZ4730',
    'BDA_FIRMWARE_C100_JZ4740'
)) {
    if (-not $profiles.Contains($profileName)) {
        throw "Firmware profile is missing: $profileName"
    }
}
foreach ($stopAddress in @(
    '0x8018b0d8u',
    '0x80199a6cu',
    '0x801891e8u',
    '0x801a13bcu',
    '0x801925d8u'
)) {
    if (-not $profiles.Contains($stopAddress)) {
        throw "Firmware capture stop entry is missing: $stopAddress"
    }
}
if (
    -not $profiles.Contains('#include "bda_hardware.h"') -or
    -not $profiles.Contains('bda_detect_hardware(&hardware);') -or
    -not $profiles.Contains('BDA_DEVICE_MODEL_9588') -or
    -not $profiles.Contains('BDA_DEVICE_MODEL_9688') -or
    -not $profiles.Contains('hardware->chip_model == soc') -or
    -not $profiles.Contains('static const bda_firmware_profile_t *cached_profile')
) {
    throw 'Firmware profiles do not require SDK model and chip detection.'
}
if (
    -not $audio.Contains(
        'typedef void (*capture_init_format_fn_t)(u32, u32, u32);'
    ) -or
    $audio.Contains('BDA_CAPTURE_STOP_AIC') -or
    ([regex]::Matches(
        $audio,
        [regex]::Escape('bda_audio_capture_firmware()')
    ).Count -ne 2)
) {
    throw 'Capture ABI or one-time profile detection regressed.'
}
if (
    -not $musbCore.Contains(
        '#define MUSB_POWER_SOFT_CONNECT_FS 0x40u'
    ) -or
    -not $musbCore.Contains('#define CPM_CLKGR   0xb0000020u') -or
    -not $musbCore.Contains('#define CPM_UHCCDR  0xb0000024u') -or
    $musbCore.Contains('0xb0002000u') -or
    $musbCore.Contains('0xb0002024u') -or
    -not $musbCore.Contains('g_profile->udc_kind == BDA_UDC_MUSB') -or
    -not $musbCore.Contains('*fifo = ~sequence;') -or
    -not $musbCore.Contains("'9', 0x00, '6', 0x00, '8', 0x00, '8', 0x00") -or
    -not $liveUi.Contains('"BBK USB MICROPHONE"') -or
    -not $liveUi.Contains('"STOP & EXIT"')
) {
    throw 'MUSB Full-Speed guard, framing, or release UI is incomplete.'
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
$releaseWorkflowText = Get-Content -LiteralPath $releaseWorkflowPath -Raw
$hostWorkflowText = Get-Content -LiteralPath $hostWorkflowPath -Raw
$packageReleaseText = Get-Content -LiteralPath $packageReleasePath -Raw
$gitModulesText = Get-Content -LiteralPath $gitModulesPath -Raw
$sdkHardwareText = Get-Content -LiteralPath $sdkHardwarePath -Raw
$endpointInstallerText = Get-Content -LiteralPath (
    Join-Path $projectRoot 'host\bridge\AudioEndpointInstaller.cs'
) -Raw
if (
    -not $gitModulesText.Contains(
        'url = https://github.com/HelloClyde/bbk9588-bda-sdk.git'
    ) -or
    -not $sdkHardwareText.Contains('bda_detect_hardware') -or
    -not $deviceBuild.Contains("'sdk\include'") -or
    -not $releaseWorkflowText.Contains('submodules: recursive') -or
    -not $hostWorkflowText.Contains('submodules: recursive')
) {
    throw 'Pinned SDK submodule integration is incomplete.'
}
foreach ($copiedSdkHeader in @(
    'device\include\bda_types.h',
    'device\include\bda_graphics.h',
    'device\include\bda_input.h',
    'device\include\bda_window.h',
    'device\include\bda\detail\runtime.h'
)) {
    if (Test-Path -LiteralPath (Join-Path $projectRoot $copiedSdkHeader)) {
        throw "Public SDK header must come from submodule: $copiedSdkHeader"
    }
}
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
if (
    -not $installerText.Contains('VersionInfoVersion={#MyFileVersion}') -or
    -not $releaseWorkflowText.Contains('9588UsbMic.bda') -or
    $releaseWorkflowText.Contains('9588UsbMic-MUSB.bda') -or
    $packageReleaseText.Contains('9588UsbMic-MUSB.bda') -or
    -not $packageReleaseText.Contains(
        "Name = '9588UsbMic.bda'"
    ) -or
    -not $releaseWorkflowText.Contains('9588UsbMicSetup.exe') -or
    -not $releaseWorkflowText.Contains('SHA256SUMS.txt') -or
    -not $releaseWorkflowText.Contains('gh release')
) {
    throw 'Tagged release asset publication is incomplete.'
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
