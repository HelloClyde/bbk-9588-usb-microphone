# SPDX-License-Identifier: Apache-2.0

param(
    [string]$Version = '0.1.3',
    [string]$IsccPath,
    [string]$DotnetPath
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$bridgeProject = Join-Path `
    $projectRoot `
    'host\bridge\Usb9588MicBridge.csproj'
$stageDir = Join-Path $projectRoot 'out\installer-stage'
$appStageDir = Join-Path $stageDir 'app'
$outputDir = Join-Path $projectRoot 'out\installer'
$scriptPath = Join-Path $PSScriptRoot '9588-usb-mic.iss'

if (-not $DotnetPath) {
    $localDotnet = Join-Path $projectRoot 'out\tools\dotnet\dotnet.exe'
    $DotnetPath = if (Test-Path -LiteralPath $localDotnet) {
        $localDotnet
    }
    else {
        (Get-Command dotnet -ErrorAction Stop).Source
    }
}
$sdkList = (& $DotnetPath --list-sdks | Out-String)
if ($LASTEXITCODE -ne 0 -or $sdkList -notmatch '(?m)^10\.') {
    throw (
        '.NET 10 SDK is required to build the self-contained bridge. ' +
        'Install it or pass -DotnetPath C:\path\to\dotnet.exe.'
    )
}

if ($Version -notmatch '^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z.-]+)?$') {
    throw "Invalid installer version: $Version"
}
$fileVersion = [regex]::Match(
    $Version,
    '^[0-9]+\.[0-9]+\.[0-9]+'
).Value

if (Test-Path -LiteralPath $stageDir) {
    $resolvedStage = (Resolve-Path -LiteralPath $stageDir).Path
    $resolvedRoot = (Resolve-Path -LiteralPath $projectRoot).Path
    if (-not $resolvedStage.StartsWith(
        $resolvedRoot + [IO.Path]::DirectorySeparatorChar,
        [StringComparison]::OrdinalIgnoreCase
    )) {
        throw "Refusing to remove stage directory outside project: $resolvedStage"
    }
    Remove-Item -LiteralPath $resolvedStage -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $appStageDir | Out-Null
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null

& $DotnetPath publish $bridgeProject `
    --configuration Release `
    --runtime win-x64 `
    --self-contained true `
    --output $appStageDir `
    --nologo `
    -p:PublishSingleFile=true `
    -p:IncludeNativeLibrariesForSelfExtract=true `
    -p:DebugType=None `
    -p:DebugSymbols=false `
    -p:Version=$Version
if ($LASTEXITCODE -ne 0) {
    throw "Bridge publish failed with code $LASTEXITCODE"
}

Copy-Item -LiteralPath (Join-Path $projectRoot 'LICENSE') `
    -Destination (Join-Path $stageDir 'LICENSE.txt')
Copy-Item -LiteralPath (Join-Path $projectRoot 'NOTICE') `
    -Destination (Join-Path $stageDir 'NOTICE.txt')
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'vb-cable-notice.txt') `
    -Destination (Join-Path $stageDir 'VB-CABLE-NOTICE.txt')
Copy-Item -LiteralPath (
    Join-Path $projectRoot 'third-party\NAudio-LICENSE.txt'
) -Destination (Join-Path $stageDir 'NAudio-LICENSE.txt')

if (-not $IsccPath) {
    $candidates = @(
        (Join-Path ${env:ProgramFiles(x86)} 'Inno Setup 6\ISCC.exe')
        (Join-Path $env:ProgramFiles 'Inno Setup 6\ISCC.exe')
        (Join-Path $env:ProgramFiles 'Inno Setup 7\ISCC.exe')
        (Join-Path $projectRoot 'out\tools\inno-setup\ISCC.exe')
    )
    $IsccPath = $candidates |
        Where-Object { $_ -and (Test-Path -LiteralPath $_) } |
        Select-Object -First 1
}
if (-not $IsccPath -or -not (Test-Path -LiteralPath $IsccPath)) {
    throw (
        'Inno Setup compiler was not found. Install Inno Setup 6.7+ or ' +
        'pass -IsccPath C:\path\to\ISCC.exe.'
    )
}

& $IsccPath `
    "/DMyAppVersion=$Version" `
    "/DMyFileVersion=$fileVersion" `
    "/DProjectRoot=$projectRoot" `
    "/O$outputDir" `
    $scriptPath
if ($LASTEXITCODE -ne 0) {
    throw "Installer build failed with code $LASTEXITCODE"
}

$setupPath = Join-Path $outputDir '9588UsbMicSetup.exe'
if (-not (Test-Path -LiteralPath $setupPath)) {
    throw "Installer compiler did not produce $setupPath"
}
Get-FileHash -Algorithm SHA256 -LiteralPath $setupPath
Write-Host "Built: $setupPath"
