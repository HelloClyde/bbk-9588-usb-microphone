# Changelog

## Unreleased

- Added `bbk9588-bda-sdk` as a pinned Git submodule and made local and CI
  device builds consume its public headers and packer directly.
- Firmware selection now requires the SDK's model/SoC result and the existing
  exact firmware code signatures to agree before UDC MMIO.
- Cached the successful hardware/profile result for the BDA session so the
  SDK's OS-image scan never runs in the PCM capture hot path.
- Added exact firmware profiles for 9588/C200 JZ4720, JZ4730, and JZ4740 plus
  9688/C100 JZ4730 and JZ4740 recovery images.
- Added a separate Full-Speed indexed-MUSB CDC backend for JZ4720/JZ4740,
  preserving the existing VID/PID and 64-byte PCM wire protocol.
- Unified the PCH-style and MUSB backends behind one hardware-gated
  `9588UsbMic.bda`; builds and releases no longer publish a second BDA.
- Selects 9588/C200 or 9688/C100 USB product text from the exact runtime
  profile.
- Uses each firmware's own capture cleanup entry and honors the `void`
  convention of the JZ4720/JZ4740 format-taking capture initializer.
- Kept all four newly added firmware paths marked as static candidates until
  each completes one integrated real-device acceptance run.

## 0.1.4-rc.2 - 2026-07-30

- Normalized fixed-width PE version strings before tagged CI verifies the
  prerelease installer version.

## 0.1.4-rc.1 - 2026-07-30

- Added a custom microphone, waveform, and USB menu icon to replace the
  packer's diagnostic placeholder.
- Added a tagged GitHub Actions release pipeline that builds the BDA and
  versioned Windows installer on a clean runner, verifies both, and uploads
  them with `SHA256SUMS.txt` to GitHub Releases.
- Pinned the public BDA packer, MIPS toolchain, and official Inno Setup
  download used by release builds.

## 0.1.3 - 2026-07-30

- Prepared the first standalone public source release.
- Added Windows CI, deterministic release packaging, and SHA-256 manifests.
- Replaced the probe-style BDA screen with an immediate-recording interface,
  a higher-sensitivity envelope waveform, recording/link indicators, and a
  touch-responsive `STOP & EXIT` button.
- Removed the entry and confirmation dialogs; capture starts as soon as the
  BDA opens.
- Changed the exit message to state that a restart is required before USB
  storage can be used again.
- Replaced probe traces with a short per-session summary at
  `A:\应用\数据\9588usbmic.log`.
- Unified the device artifact name as `9588UsbMic.bda`.

## 0.1.2-dev - 2026-07-30

- Preserve the Console, Multimedia, and Communications default playback
  endpoints across the first VB-CABLE driver installation.
- Select `CABLE Output` for all three default recording roles after install.
- Stop the running tray bridge before upgrades so its executable can be
  replaced without an application-close prompt.

## 0.1.1-dev - 2026-07-30

- Replaced block-oriented Media Foundation resampling with streaming WDL
  resampling. This removes the observed 250 ms audio / 750 ms silence cycle.
- Skip VB-CABLE installation and reboot when upgrading a PC that already has
  the virtual audio driver.
- Verified a 9.98-second capture from `CABLE Output` with continuous nonzero
  audio and no silence interval longer than 100 ms.

## 0.1.0-dev - 2026-07-30

- Added the real-device validated C3 CDC ACM profile.
- Added the Windows CDC capture tool with live waveform, WAV, PNG, PnP, and
  SetupAPI diagnostics.
- Added a .NET 10 tray bridge that routes live CDC PCM through NAudio to the
  VB-CABLE playback endpoint.
- Added a one-click Windows installer build that downloads and verifies the
  original VB-CABLE package, installs the bridge, and configures sign-in
  startup.
- Documented the 64-byte PCM frame and selected JZ4730 endpoint map.
- Removed the host tool's obsolete HID transport.
- Fixed duplicate device-side frame trailer, confirm, and diagnostic counting
  in the maintained source.
- Kept Mass Storage hot restore explicitly unsupported; reboot remains
  required after each run.
