# Windows one-click installer

`9588UsbMicSetup.exe` installs the self-contained tray bridge, optionally
downloads the original signed VB-CABLE package, invokes its silent installer,
and registers the bridge to start at user sign-in. Before a first-time driver
installation it saves all three default playback roles and restores them after
VB-CABLE is installed. It then selects `CABLE Output` for all three default
recording roles, so installing the virtual microphone does not redirect normal
PC playback into `CABLE Input`.

The user-facing flow is one setup executable and one UAC elevation. Windows may
still show its own driver confirmation, and VB-CABLE requires a restart. Those
OS-level steps cannot be suppressed by this project.

## Build

Install Inno Setup 6.7 or later, then run:

```powershell
.\installer\build.ps1
```

The output is:

```text
out\installer\9588UsbMicSetup.exe
```

The bridge is published with the supported .NET 10 LTS runtime as a
self-contained `win-x64` single-file application, so the target PC does not
need a separate .NET runtime. Building requires the .NET 10 SDK; it can be
supplied explicitly with `-DotnetPath`.

## Distribution boundary

Development installers download VB-CABLE from VB-Audio during setup and verify
the pinned SHA-256 hash. They do not store the driver binary in this source
repository.

Before publicly distributing an offline installer with VB-CABLE embedded,
obtain written permission or an appropriate distribution license from
VB-Audio. Keep `vb-cable-notice.txt` visible in every build.

The outer `9588UsbMicSetup.exe` should also be Authenticode-signed before public
release. The VB-CABLE driver package is signed, but that does not sign this
project's installer or bridge executable.
