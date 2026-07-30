# SPDX-License-Identifier: Apache-2.0

param(
    [string]$SdkRoot = '',
    [string]$ToolchainPrefix = '',
    [string]$OutputPath = '',
    [string]$Title = '9588UsbMic',
    [string]$IconPath = ''
)

$ErrorActionPreference = 'Stop'
$deviceRoot = $PSScriptRoot
$projectRoot = Split-Path -Parent $deviceRoot

if (-not $SdkRoot) {
    $SdkRoot = $env:BBK9588_SDK_ROOT
}
if (-not $SdkRoot) {
    $candidate = Join-Path $projectRoot 'sdk'
    if (Test-Path -LiteralPath (Join-Path $candidate 'reverse\bda_compile_c.py')) {
        $SdkRoot = $candidate
    }
}
if (-not $SdkRoot) {
    throw (
        'SDK submodule is not initialized. Run: ' +
        'git submodule update --init --recursive'
    )
}
$SdkRoot = (Resolve-Path -LiteralPath $SdkRoot).Path

if (-not $OutputPath) {
    $OutputPath = Join-Path $projectRoot 'out\9588UsbMic.bda'
}
$outputFullPath = [IO.Path]::GetFullPath($OutputPath)

if (-not $ToolchainPrefix) {
    $ToolchainPrefix = $env:JZ4730_TOOLCHAIN_PREFIX
}
if (-not $ToolchainPrefix) {
    $projectPrefix = Join-Path `
        $projectRoot `
        '.toolchain\bin\mipsel-none-elf-'
    if (Test-Path -LiteralPath ($projectPrefix + 'gcc.exe')) {
        $ToolchainPrefix = $projectPrefix
    }
}
if (-not $ToolchainPrefix) {
    $bundledPrefix = Join-Path `
        $SdkRoot `
        '.toolchain\bin\mipsel-none-elf-'
    if (Test-Path -LiteralPath ($bundledPrefix + 'gcc.exe')) {
        $ToolchainPrefix = $bundledPrefix
    }
}
if (-not $ToolchainPrefix) {
    $ToolchainPrefix = 'mipsel-none-elf-'
}

if (-not $IconPath) {
    $IconPath = Join-Path $deviceRoot 'assets\9588-usb-mic.png'
}
if (-not (Test-Path -LiteralPath $IconPath)) {
    throw "BDA icon not found: $IconPath"
}
$IconPath = (Resolve-Path -LiteralPath $IconPath).Path

$includePath = Join-Path $deviceRoot 'include'
$sdkIncludePath = Join-Path $SdkRoot 'sdk\include'
$hardwareHeader = Join-Path $sdkIncludePath 'bda_hardware.h'
$audioHeader = Join-Path $sdkIncludePath 'bda_audio.h'
$builder = Join-Path $SdkRoot 'reverse\bda_compile_c.py'
$validator = Join-Path $SdkRoot 'reverse\bda_validate.py'
if (-not (Test-Path -LiteralPath $builder)) {
    throw "BDA builder not found under SDK root: $builder"
}
if (
    -not (Test-Path -LiteralPath $hardwareHeader) -or
    -not (Test-Path -LiteralPath $audioHeader)
) {
    throw (
        'SDK hardware or multi-firmware audio API is missing. ' +
        'Update the submodule: ' +
        'git submodule update --init --recursive'
    )
}

function Build-Bda {
    param(
        [Parameter(Mandatory)]
        [string]$SourcePath,
        [Parameter(Mandatory)]
        [string]$TargetPath,
        [Parameter(Mandatory)]
        [string]$TargetTitle
    )

    New-Item -ItemType Directory `
        -Path (Split-Path -Parent $TargetPath) `
        -Force | Out-Null
    $buildArgs = @(
        $SourcePath,
        '--prefix', $ToolchainPrefix,
        '--title', $TargetTitle,
        '--category', '9',
        '--icon-png', $IconPath,
        '-I', $includePath,
        '-I', $sdkIncludePath,
        '-o', $TargetPath
    )

    & python $builder @buildArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Device build failed with code $LASTEXITCODE"
    }
    & python $validator $TargetPath
    if ($LASTEXITCODE -ne 0) {
        throw "BDA validation failed with code $LASTEXITCODE"
    }

    $hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $TargetPath).Hash
    Write-Host "Built: $TargetPath"
    Write-Host "SHA-256: $hash"
}

Build-Bda `
    -SourcePath (Join-Path $deviceRoot 'src\main.c') `
    -TargetPath $outputFullPath `
    -TargetTitle $Title

foreach ($obsoleteName in @(
    '9588UsbMic-MUSB.bda',
    'UsbCdcPcm.bda'
)) {
    $obsoleteOutput = [IO.Path]::GetFullPath(
        (Join-Path $projectRoot "out\$obsoleteName")
    )
    if (
        $obsoleteOutput -ne $outputFullPath -and
        (Test-Path -LiteralPath $obsoleteOutput)
    ) {
        Remove-Item -LiteralPath $obsoleteOutput -Force
        Write-Host "Removed obsolete artifact: $obsoleteOutput"
    }
}
