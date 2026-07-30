; SPDX-License-Identifier: Apache-2.0

#ifndef MyAppVersion
  #define MyAppVersion "0.1.3"
#endif
#ifndef ProjectRoot
  #error ProjectRoot must be passed by installer/build.ps1
#endif

#define MyAppName "9588 USB Microphone"
#define MyAppPublisher "9588 USB PCM project"
#define MyAppExeName "Usb9588MicBridge.exe"
#define StageDir ProjectRoot + "\out\installer-stage"
#define VbCableUrl "https://download.vb-audio.com/Download_CABLE/VBCABLE_Driver_Pack45.zip"
#define VbCableSha256 "B950E39F01AF1D04EA623C8F6D8EB9B6EA5C477C637295FABF20631C85116BFB"

[Setup]
AppId={{4BF3A9AB-86F1-4F97-BC34-A6EB4BD19392}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\9588 USB Microphone
DefaultGroupName=9588 USB Microphone
DisableProgramGroupPage=yes
InfoBeforeFile={#StageDir}\VB-CABLE-NOTICE.txt
LicenseFile={#StageDir}\LICENSE.txt
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0.17763
OutputBaseFilename=9588UsbMicSetup
Compression=lzma2
SolidCompression=yes
ArchiveExtraction=full
WizardStyle=modern
SetupLogging=yes
RestartIfNeededByRun=yes
CloseApplications=force
RestartApplications=no
UninstallDisplayIcon={app}\{#MyAppExeName}
VersionInfoVersion={#MyAppVersion}
VersionInfoDescription={#MyAppName} installer
VersionInfoCompany={#MyAppPublisher}
VersionInfoCopyright=Apache-2.0 project; VB-CABLE remains copyright V. Burel

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "vbcable"; Description: "Install VB-CABLE virtual audio driver"; \
  GroupDescription: "Virtual microphone driver:"; Flags: checkedonce; \
  Check: ShouldOfferVbCable
Name: "autostart"; Description: "Start the 9588 microphone bridge at sign-in"; \
  GroupDescription: "Startup:"; Flags: checkedonce

[Files]
Source: "{#StageDir}\app\{#MyAppExeName}"; DestDir: "{app}"; \
  Flags: ignoreversion
Source: "{#StageDir}\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#StageDir}\NOTICE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#StageDir}\VB-CABLE-NOTICE.txt"; DestDir: "{app}"; \
  Flags: ignoreversion
Source: "{#StageDir}\NAudio-LICENSE.txt"; DestDir: "{app}"; \
  Flags: ignoreversion
Source: "{#VbCableUrl}"; \
  DestDir: "{app}\third-party\VBCABLE"; \
  DestName: "VBCABLE_Driver_Pack45.zip"; \
  ExternalSize: 3_467_579; \
  Hash: "{#VbCableSha256}"; \
  Flags: external download extractarchive ignoreversion; \
  Tasks: vbcable

[Registry]
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; \
  ValueType: string; ValueName: "9588UsbMicBridge"; \
  ValueData: """{app}\{#MyAppExeName}"""; \
  Flags: uninsdeletevalue; Tasks: autostart

[Icons]
Name: "{group}\9588 USB Microphone"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\VB-CABLE license and source"; \
  Filename: "{app}\VB-CABLE-NOTICE.txt"
Name: "{group}\NAudio license"; Filename: "{app}\NAudio-LICENSE.txt"
Name: "{group}\Remove VB-CABLE (requires restart)"; \
  Filename: "{app}\third-party\VBCABLE\VBCABLE_Setup_x64.exe"; \
  Parameters: "-u"; WorkingDir: "{app}\third-party\VBCABLE"; Tasks: vbcable
Name: "{group}\Uninstall 9588 USB Microphone"; \
  Filename: "{uninstallexe}"

[Run]
Filename: "{app}\third-party\VBCABLE\VBCABLE_Setup_x64.exe"; \
  Parameters: "-i -h"; \
  StatusMsg: "Installing the signed VB-CABLE audio driver..."; \
  Flags: runhidden waituntilterminated; Tasks: vbcable; \
  BeforeInstall: SaveDefaultPlayback; \
  AfterInstall: RestoreDefaultPlaybackAndSelectMicrophone
Filename: "{app}\{#MyAppExeName}"; \
  Description: "Start the 9588 USB Microphone bridge"; \
  Flags: nowait postinstall skipifsilent runasoriginaluser

[UninstallRun]
Filename: "{sys}\taskkill.exe"; Parameters: "/IM {#MyAppExeName} /F"; \
  Flags: runhidden waituntilterminated; RunOnceId: "StopBridge"

[Code]
var
  VbCablePresentAtStart: Boolean;

function InitializeSetup(): Boolean;
begin
  VbCablePresentAtStart :=
    RegKeyExists(
      HKLM,
      'SYSTEM\CurrentControlSet\Services\VBAudioVACMME'
    );
  Result := True;
end;

function ShouldOfferVbCable(): Boolean;
begin
  Result := not VbCablePresentAtStart;
end;

function RunBridgeHelper(Parameters: String): Boolean;
var
  ResultCode: Integer;
begin
  Result :=
    ExecAsOriginalUser(
      ExpandConstant('{app}\{#MyAppExeName}'),
      Parameters,
      ExpandConstant('{app}'),
      SW_HIDE,
      ewWaitUntilTerminated,
      ResultCode
    ) and
    (ResultCode = 0);
end;

procedure SaveDefaultPlayback;
begin
  if not RunBridgeHelper('--save-default-playback') then
  begin
    RaiseException(
      'Unable to save the current default playback device.'
    );
  end;
end;

procedure RestoreDefaultPlaybackAndSelectMicrophone;
begin
  if not RunBridgeHelper('--restore-default-playback') then
  begin
    RaiseException(
      'VB-CABLE was installed, but the previous playback device could ' +
      'not be restored.'
    );
  end;
  if not RunBridgeHelper('--set-default-cable-microphone') then
  begin
    RaiseException(
      'VB-CABLE was installed, but CABLE Output could not be selected ' +
      'as the default microphone.'
    );
  end;
end;

procedure ConfigureExistingVbCable;
begin
  if not RunBridgeHelper('--set-default-cable-microphone') then
  begin
    RaiseException(
      'CABLE Output could not be selected as the default microphone.'
    );
  end;
end;

function PrepareToInstall(var NeedsRestart: Boolean): String;
var
  ResultCode: Integer;
begin
  Exec(
    ExpandConstant('{sys}\taskkill.exe'),
    '/IM {#MyAppExeName} /F',
    '',
    SW_HIDE,
    ewWaitUntilTerminated,
    ResultCode
  );
  Sleep(300);
  Result := '';
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep = ssPostInstall) and VbCablePresentAtStart then
  begin
    ConfigureExistingVbCable;
  end;
end;

function NeedRestart(): Boolean;
begin
  Result :=
    (not VbCablePresentAtStart) and
    WizardIsTaskSelected('vbcable');
end;
