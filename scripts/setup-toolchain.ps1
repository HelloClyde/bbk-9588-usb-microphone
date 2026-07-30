# SPDX-License-Identifier: Apache-2.0

param(
    [string]$Url = (
        'https://static.grumpycoder.net/pixel/mips/' +
        'g++-mipsel-none-elf-15.2.0.zip'
    ),
    [string]$ExpectedSha256 = (
        '8BA866E25C9826EE04AB4310365D264E3E73769E3738BB58AE38FD6740B7EE8D'
    )
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$toolchainDir = Join-Path $projectRoot '.toolchain'
$archivePath = Join-Path $toolchainDir 'g++-mipsel-none-elf-15.2.0.zip'

function Find-ToolchainPrefix {
    $roots = @(
        $toolchainDir
        (Join-Path $toolchainDir 'g++-mipsel-none-elf-15.2.0')
    )
    foreach ($root in $roots) {
        $prefix = Join-Path $root 'bin\mipsel-none-elf-'
        $gcc = $prefix + 'gcc.exe'
        $objcopy = $prefix + 'objcopy.exe'
        $cc1 = Join-Path `
            $root `
            'libexec\gcc\mipsel-none-elf\15.2.0\cc1.exe'
        if (
            (Test-Path -LiteralPath $gcc -PathType Leaf) -and
            (Test-Path -LiteralPath $objcopy -PathType Leaf) -and
            (Test-Path -LiteralPath $cc1 -PathType Leaf)
        ) {
            return $prefix
        }
    }
    return $null
}

$prefix = Find-ToolchainPrefix
if ($prefix) {
    Write-Host "Toolchain already installed: $prefix"
    Write-Output $prefix
    return
}

New-Item -ItemType Directory -Force -Path $toolchainDir | Out-Null
if (-not (Test-Path -LiteralPath $archivePath -PathType Leaf)) {
    Write-Host "Downloading verified MIPS toolchain..."
    Invoke-WebRequest -UseBasicParsing -Uri $Url -OutFile $archivePath
}

$actualSha256 = (
    Get-FileHash -Algorithm SHA256 -LiteralPath $archivePath
).Hash
if ($actualSha256 -ne $ExpectedSha256.ToUpperInvariant()) {
    throw (
        "Toolchain archive SHA-256 mismatch. Expected $ExpectedSha256, " +
        "got $actualSha256."
    )
}

Write-Host "Extracting verified MIPS toolchain..."
Expand-Archive `
    -LiteralPath $archivePath `
    -DestinationPath $toolchainDir `
    -Force

$prefix = Find-ToolchainPrefix
if (-not $prefix) {
    throw 'MIPS toolchain extraction completed but gcc/objcopy/cc1 are missing.'
}
Write-Output $prefix
