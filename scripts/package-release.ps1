# SPDX-License-Identifier: Apache-2.0

param(
    [string]$Version = '0.1.3'
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$outDir = Join-Path $projectRoot 'out'
$releaseDir = Join-Path $outDir "release\v$Version"

if ($Version -notmatch '^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z.-]+)?$') {
    throw "Invalid release version: $Version"
}

$assets = @(
    @{
        Source = Join-Path $outDir '9588UsbMic.bda'
        Name = '9588UsbMic.bda'
    },
    @{
        Source = Join-Path $outDir 'installer\9588UsbMicSetup.exe'
        Name = '9588UsbMicSetup.exe'
    }
)

foreach ($asset in $assets) {
    if (-not (Test-Path -LiteralPath $asset.Source)) {
        throw "Release asset is missing: $($asset.Source)"
    }
}

if (Test-Path -LiteralPath $releaseDir) {
    $resolvedRelease = (Resolve-Path -LiteralPath $releaseDir).Path
    $resolvedOut = (Resolve-Path -LiteralPath $outDir).Path
    if (-not $resolvedRelease.StartsWith(
        $resolvedOut + [IO.Path]::DirectorySeparatorChar,
        [StringComparison]::OrdinalIgnoreCase
    )) {
        throw "Refusing to remove release directory outside out: $resolvedRelease"
    }
    Remove-Item -LiteralPath $resolvedRelease -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $releaseDir | Out-Null

foreach ($asset in $assets) {
    Copy-Item -LiteralPath $asset.Source -Destination (
        Join-Path $releaseDir $asset.Name
    )
}

$hashLines = Get-ChildItem -LiteralPath $releaseDir -File |
    Sort-Object Name |
    ForEach-Object {
        $hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $_.FullName).Hash
        "$hash *$($_.Name)"
    }
$manifestPath = Join-Path $releaseDir 'SHA256SUMS.txt'
[IO.File]::WriteAllLines(
    $manifestPath,
    $hashLines,
    [Text.UTF8Encoding]::new($false)
)

Write-Host "Release assets: $releaseDir"
Get-Content -LiteralPath $manifestPath
