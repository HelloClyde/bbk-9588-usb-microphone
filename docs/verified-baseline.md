# Verified Baseline

## Real-device artifact

The original C3 artifact used for the successful hardware run:

```text
title: UsbCdcP3
SHA-256: 4CFFA3689AC1EA0792C3D49BFB7348B891EAE1A0A369C18AB92C5FD4E0AD9719
```

Observed Windows path:

```text
usbccgp parent: Code 0
usbser MI_00 child: Code 0
COM port: available
```

Host capture:

```text
duration: 15.000 s
frames: 15,000
samples: 240,000
format: pcm_s16le / 16000 Hz / mono / 16-bit
invalid frames: 0
sequence gaps: 0
checksum errors: 0
device underrun reports: 0
nonzero samples: 239,991
```

Device log:

```text
normal SC acknowledgements: 1
configuration fallback triggers: 0
valid EP2 PCM frames: 15,104 (0x3B00)
empty tokens: 0
spurious IRQs: 0
event queue overflows: 0
```

This proves the SDK capture to JZ4730 EP2 to Windows WAV data path.

## Maintained source delta

The verified artifact contained a duplicated frame trailer, endpoint confirm,
and diagnostic counter block. Hardware and host logs showed only one valid
64-byte frame per EP2 token, while two diagnostic counters were exactly
doubled.

The source in this project removes that duplicate block and enforces one
confirm and one wire-frame increment per CDC frame. Therefore a newly built
BDA does not have the same hash as the original artifact. The original hash
is retained as the evidence baseline; the maintained source requires one
combined acceptance test when the next product candidate is ready.

No claim is made that Mass Storage recovers after exit. The verified run
required disconnect, BDA exit, and device reboot.

## v0.1.3 public artifact

The public BDA was rebuilt and passed the offline packer, checksum, entry-point,
profile, and host compatibility checks:

```text
title: 9588UsbMic
size: 65,792 bytes
SHA-256: AE3782ACE756EAF02FCFC17FEF1342CB16DD47212C2DDB8F561F9F2554A98ADA
```

This hash includes the release UI and reduced logging changes. It is not a
claim that this exact artifact has completed a new real-device acceptance run;
the hardware evidence above establishes the underlying C3 CDC PCM path.
