# SPDX-License-Identifier: Apache-2.0

param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$projectPath = Join-Path $PSScriptRoot 'src\UsbCdcPcmCapture.csproj'
$outputDir = Join-Path $projectRoot 'out\host'

New-Item -ItemType Directory -Path $outputDir -Force | Out-Null
& dotnet build $projectPath `
    --configuration $Configuration `
    --output $outputDir `
    --nologo
if ($LASTEXITCODE -ne 0) {
    throw "Host build failed with code $LASTEXITCODE"
}

Write-Host "Built: $(Join-Path $outputDir 'UsbCdcPcmCapture.exe')"
