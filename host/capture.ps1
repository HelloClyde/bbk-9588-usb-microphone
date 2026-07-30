# SPDX-License-Identifier: Apache-2.0

param(
    [int]$WaitSeconds = 120,
    [int]$CaptureSeconds = 15,
    [string]$ProductId = '0556',
    [string]$ProfileName = 'C3',
    [switch]$NoWindow
)

$ErrorActionPreference = 'Stop'
if ($ProductId -notmatch '^[0-9A-Fa-f]{4}$') {
    throw 'ProductId must contain exactly four hexadecimal digits.'
}
$ProductId = $ProductId.ToUpperInvariant()
$projectRoot = Split-Path -Parent $PSScriptRoot
$outputDir = Join-Path $projectRoot 'out'
$projectDir = Join-Path $PSScriptRoot 'src'
$projectPath = Join-Path $projectDir 'UsbCdcPcmCapture.csproj'
$toolDir = Join-Path $outputDir 'host'
$toolPath = Join-Path $toolDir 'UsbCdcPcmCapture.exe'
$wavPath = Join-Path $outputDir 'capture.wav'
$pngPath = Join-Path $outputDir 'waveform.png'
$logPath = Join-Path $outputDir 'capture.log'
$pnpLogPath = Join-Path $outputDir 'pnp.log'
$setupApiLogPath = Join-Path $outputDir 'setupapi.log'
$hardwareIdPattern = "VID_A4A5&PID_$ProductId"

function Get-CdcDevices {
    @(
        Get-CimInstance Win32_PnPEntity |
            Where-Object {
                $_.PNPDeviceID -and
                $_.PNPDeviceID.IndexOf(
                    $hardwareIdPattern,
                    [StringComparison]::OrdinalIgnoreCase
                ) -ge 0
            }
    )
}

function Get-ComPortName {
    param([Parameter(Mandatory)]$Device)

    foreach ($text in @($Device.Name, $Device.Caption)) {
        if ($text -and $text -match '\((COM[0-9]+)\)') {
            return $Matches[1]
        }
    }

    $registryPath = Join-Path `
        'HKLM:\SYSTEM\CurrentControlSet\Enum' `
        ($Device.PNPDeviceID + '\Device Parameters')
    try {
        $portName = (Get-ItemProperty `
            -LiteralPath $registryPath `
            -Name PortName `
            -ErrorAction Stop
        ).PortName
        if ($portName -match '^COM[0-9]+$') {
            return $portName
        }
    }
    catch {
    }
    return $null
}

function Test-ComPortReady {
    param(
        [Parameter(Mandatory)][string]$PortName
    )

    $serialPort = @(
        Get-CimInstance Win32_SerialPort -ErrorAction SilentlyContinue |
            Where-Object {
                $_.DeviceID -eq $PortName -and
                $_.PNPDeviceID -and
                $_.PNPDeviceID.IndexOf(
                    $hardwareIdPattern,
                    [StringComparison]::OrdinalIgnoreCase
                ) -ge 0 -and
                [int]$_.ConfigManagerErrorCode -eq 0
            }
    ) | Select-Object -First 1
    if (-not $serialPort) {
        return $false
    }

    try {
        $serialMap = Get-ItemProperty `
            -LiteralPath 'HKLM:\HARDWARE\DEVICEMAP\SERIALCOMM' `
            -ErrorAction Stop
        return @(
            $serialMap.PSObject.Properties |
                Where-Object {
                    $_.Name -notlike 'PS*' -and
                    [string]$_.Value -eq $PortName
                }
        ).Count -gt 0
    }
    catch {
        return $false
    }
}

function Write-PnpSnapshot {
    param([object[]]$Devices)

    $lines = @(
        "timestamp=$(Get-Date -Format o)"
        "hardware_id_pattern=$hardwareIdPattern"
    )
    if ($Devices.Count -eq 0) {
        $lines += 'device_count=0'
    }
    else {
        $index = 0
        foreach ($device in $Devices) {
            $lines += "device[$index].name=$($device.Name)"
            $lines += "device[$index].pnp_id=$($device.PNPDeviceID)"
            $lines += "device[$index].status=$($device.Status)"
            $lines += "device[$index].problem_code=$($device.ConfigManagerErrorCode)"
            $lines += "device[$index].com_port=$(Get-ComPortName $device)"
            $lines += "device[$index].service=$($device.Service)"
            $lines += "device[$index].class_guid=$($device.ClassGuid)"
            $lines += "device[$index].hardware_ids=$(@($device.HardwareID) -join ';')"
            $lines += "device[$index].compatible_ids=$(@($device.CompatibleID) -join ';')"
            ++$index
        }
        $lines += "device_count=$index"
    }
    Set-Content -LiteralPath $pnpLogPath -Value $lines -Encoding UTF8
}

function Write-SetupApiSnapshot {
    $sourcePath = Join-Path $env:WINDIR 'INF\setupapi.dev.log'
    $header = @(
        "timestamp=$(Get-Date -Format o)"
        "hardware_id_pattern=$hardwareIdPattern"
        "source=$sourcePath"
    )
    if (-not (Test-Path -LiteralPath $sourcePath)) {
        Set-Content -LiteralPath $setupApiLogPath `
            -Value ($header + 'setupapi_source_missing=1') `
            -Encoding UTF8
        return
    }

    try {
        $matchingBlocks = New-Object 'System.Collections.Generic.List[object]'
        $current = $null
        foreach ($line in Get-Content -LiteralPath $sourcePath) {
            if ($line.StartsWith('>>>  [')) {
                $current = New-Object 'System.Collections.Generic.List[string]'
            }
            if ($null -ne $current) {
                $current.Add($line)
            }
            if ($null -ne $current -and $line.StartsWith('<<<  [')) {
                $blockText = $current -join "`n"
                if (
                    $blockText.IndexOf(
                        $hardwareIdPattern,
                        [StringComparison]::OrdinalIgnoreCase
                    ) -ge 0
                ) {
                    $matchingBlocks.Add([string[]]$current.ToArray())
                }
                $current = $null
            }
        }

        $output = New-Object 'System.Collections.Generic.List[string]'
        foreach ($line in $header) {
            $output.Add($line)
        }
        if ($matchingBlocks.Count -eq 0) {
            $output.Add('matching_section_count=0')
        }
        else {
            $first = [Math]::Max(0, $matchingBlocks.Count - 4)
            $output.Add("matching_section_count=$($matchingBlocks.Count)")
            for ($i = $first; $i -lt $matchingBlocks.Count; ++$i) {
                $output.Add("----- setupapi section $i -----")
                foreach ($line in [string[]]$matchingBlocks[$i]) {
                    $output.Add($line)
                }
            }
        }
        Set-Content -LiteralPath $setupApiLogPath `
            -Value $output.ToArray() `
            -Encoding UTF8
    }
    catch {
        Set-Content -LiteralPath $setupApiLogPath `
            -Value ($header + "setupapi_read_error=$($_.Exception.Message)") `
            -Encoding UTF8
    }
}

if ($WaitSeconds -le 0 -or $CaptureSeconds -le 0) {
    throw 'WaitSeconds and CaptureSeconds must be positive.'
}

New-Item -ItemType Directory -Path $outputDir -Force | Out-Null
$latestSource = (
    Get-ChildItem -LiteralPath $projectDir -File |
        Measure-Object LastWriteTimeUtc -Maximum
).Maximum
if (
    -not (Test-Path -LiteralPath $toolPath) -or
    (Get-Item -LiteralPath $toolPath).LastWriteTimeUtc -lt $latestSource
) {
    Write-Host 'Building the CDC PCM capture diagnostic...'
    & dotnet build $projectPath `
        --configuration Release `
        --output $toolDir `
        --nologo
    if ($LASTEXITCODE -ne 0) {
        throw "CDC PCM diagnostic build failed with code $LASTEXITCODE"
    }
}

@($wavPath, $pngPath, $logPath, $pnpLogPath, $setupApiLogPath) |
    ForEach-Object {
    Remove-Item -LiteralPath $_ -Force -ErrorAction SilentlyContinue
}

Write-Host (
    "Waiting up to $WaitSeconds seconds for 9588 CDC PCM $ProfileName..."
)
$deadline = [DateTime]::UtcNow.AddSeconds($WaitSeconds)
$errorFirstSeen = $null
$portName = $null
do {
    $devices = @(Get-CdcDevices)
    foreach ($device in $devices) {
        $candidate = Get-ComPortName $device
        if (
            $candidate -and
            [int]$device.ConfigManagerErrorCode -eq 0 -and
            (Test-ComPortReady $candidate)
        ) {
            $portName = $candidate
            break
        }
    }
    if ($portName) {
        break
    }

    $failed = @(
        $devices |
            Where-Object { [int]$_.ConfigManagerErrorCode -ne 0 }
    )
    if ($failed.Count -gt 0) {
        if ($null -eq $errorFirstSeen) {
            $errorFirstSeen = [DateTime]::UtcNow
        }
        elseif (
            ([DateTime]::UtcNow - $errorFirstSeen).TotalSeconds -ge 3
        ) {
            Write-PnpSnapshot $devices
            Write-SetupApiSnapshot
            $codes = (
                $failed |
                    ForEach-Object { [string]$_.ConfigManagerErrorCode } |
                    Sort-Object -Unique
            ) -join ', '
            Write-Error (
                "9588 CDC enumerated but failed to start " +
                "(PnP code $codes). See $pnpLogPath and $setupApiLogPath"
            )
        }
    }
    else {
        $errorFirstSeen = $null
    }
    Start-Sleep -Milliseconds 500
} while ([DateTime]::UtcNow -lt $deadline)

if (-not $portName) {
    $devices = @(Get-CdcDevices)
    Write-PnpSnapshot $devices
    Write-SetupApiSnapshot
    Write-Error (
        "No working 9588 CDC COM port appeared. See $pnpLogPath " +
        "and $setupApiLogPath"
    )
}

$devices = @(Get-CdcDevices)
Write-PnpSnapshot $devices
Write-SetupApiSnapshot
Write-Host "Opening: $portName ($hardwareIdPattern)"
Write-Host "Capturing $CaptureSeconds seconds to: $wavPath"
$arguments = @(
    '--port', $portName,
    '--wait', '10',
    '--seconds', [string]$CaptureSeconds,
    '--wav', $wavPath,
    '--png', $pngPath,
    '--log', $logPath
)
if ($NoWindow) {
    $arguments += '--no-window'
}

& $toolPath @arguments
$toolExitCode = $LASTEXITCODE
if ($toolExitCode -ne 0) {
    Write-Error "CDC PCM capture failed with code $toolExitCode. See $logPath"
}

Write-Host "Capture complete: $wavPath"
Write-Host "Waveform: $pngPath"
Write-Host "Transport log: $logPath"
Write-Host "PnP snapshot: $pnpLogPath"
Write-Host "SetupAPI snapshot: $setupApiLogPath"
