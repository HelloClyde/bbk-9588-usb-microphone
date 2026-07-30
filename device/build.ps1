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
    $candidate = Split-Path -Parent $projectRoot
    if (Test-Path -LiteralPath (Join-Path $candidate 'bda_packer')) {
        $SdkRoot = $candidate
    }
}
if ($SdkRoot) {
    $SdkRoot = (Resolve-Path -LiteralPath $SdkRoot).Path
}

if (-not $OutputPath) {
    $OutputPath = Join-Path $projectRoot 'out\9588UsbMic.bda'
}
$outputFullPath = [IO.Path]::GetFullPath($OutputPath)
New-Item -ItemType Directory `
    -Path (Split-Path -Parent $outputFullPath) `
    -Force | Out-Null

if (-not $ToolchainPrefix) {
    $ToolchainPrefix = $env:JZ4730_TOOLCHAIN_PREFIX
}
if (-not $ToolchainPrefix -and $SdkRoot) {
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

$sourcePath = Join-Path $deviceRoot 'src\main.c'
$includePath = Join-Path $deviceRoot 'include'
$buildArgs = @(
    $sourcePath,
    '--prefix', $ToolchainPrefix,
    '--title', $Title,
    '--category', '9',
    '--icon-png', $IconPath,
    '-I', $includePath,
    '-o', $outputFullPath
)

if ($SdkRoot) {
    $builder = Join-Path $SdkRoot 'reverse\bda_compile_c.py'
    $validator = Join-Path $SdkRoot 'reverse\bda_validate.py'
    if (-not (Test-Path -LiteralPath $builder)) {
        throw "BDA builder not found under SDK root: $builder"
    }
    & python $builder @buildArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Device build failed with code $LASTEXITCODE"
    }
    & python $validator $outputFullPath
    if ($LASTEXITCODE -ne 0) {
        throw "BDA validation failed with code $LASTEXITCODE"
    }
}
else {
    & bda-pack @buildArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Device build failed with code $LASTEXITCODE"
    }
    & bda-validate $outputFullPath
    if ($LASTEXITCODE -ne 0) {
        throw "BDA validation failed with code $LASTEXITCODE"
    }
}

$hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $outputFullPath).Hash
Write-Host "Built: $outputFullPath"
Write-Host "SHA-256: $hash"
