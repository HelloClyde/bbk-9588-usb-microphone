# SPDX-License-Identifier: Apache-2.0

param(
    [string]$Version = '6.7.3',
    [string]$Url = (
        'https://github.com/jrsoftware/issrc/releases/download/' +
        'is-6_7_3/innosetup-6.7.3.exe'
    ),
    [string]$ExpectedSha256 = (
        '9C73C3BAE7ED48D44112A0F48E66742C00090BDB5BEF71D9D3C056C66E97B732'
    )
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$toolsDir = Join-Path $projectRoot 'out\tools'
$installDir = Join-Path $toolsDir 'inno-setup'
$installerPath = Join-Path $toolsDir "innosetup-$Version.exe"
$isccPath = Join-Path $installDir 'ISCC.exe'

if (Test-Path -LiteralPath $isccPath -PathType Leaf) {
    Write-Host "Inno Setup already installed: $isccPath"
    Write-Output $isccPath
    return
}

New-Item -ItemType Directory -Force -Path $toolsDir | Out-Null
if (-not (Test-Path -LiteralPath $installerPath -PathType Leaf)) {
    Write-Host "Downloading verified Inno Setup $Version..."
    Invoke-WebRequest -UseBasicParsing -Uri $Url -OutFile $installerPath
}

$actualSha256 = (
    Get-FileHash -Algorithm SHA256 -LiteralPath $installerPath
).Hash
if ($actualSha256 -ne $ExpectedSha256.ToUpperInvariant()) {
    throw (
        "Inno Setup SHA-256 mismatch. Expected $ExpectedSha256, " +
        "got $actualSha256."
    )
}

$process = Start-Process `
    -FilePath $installerPath `
    -ArgumentList @(
        '/VERYSILENT',
        '/SUPPRESSMSGBOXES',
        '/NORESTART',
        '/SP-',
        '/CURRENTUSER',
        '/NOICONS',
        "/DIR=$installDir"
    ) `
    -Wait `
    -PassThru `
    -WindowStyle Hidden
if ($process.ExitCode -ne 0) {
    throw "Inno Setup installation failed with code $($process.ExitCode)."
}
if (-not (Test-Path -LiteralPath $isccPath -PathType Leaf)) {
    throw "Inno Setup compiler was not installed at $isccPath"
}
Write-Output $isccPath
