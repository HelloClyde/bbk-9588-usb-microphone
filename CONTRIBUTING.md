# Contributing

## Scope

Keep changes focused on the JZ4730 CDC PCM data path, host capture tool, build
reproducibility, and documented device lifecycle. Do not add firmware dumps,
NAND images, proprietary applications, or generated build output.

## Before submitting

Run:

```powershell
.\scripts\test.ps1
```

Update `docs/protocol.md` and `CHANGELOG.md` for any descriptor, endpoint,
frame, sample format, PID, or lifecycle change.

## Device testing

Real-device tests must state one concrete hypothesis and include all counters
needed to decide it in one run. Preserve:

- Exact BDA SHA-256.
- Device log.
- Host capture, PnP, and SetupAPI logs.
- WAV metadata and frame-integrity summary.
- Connection, disconnection, exit, and reboot sequence.

Do not describe USB Mass Storage as restored unless Windows successfully
re-enumerates and transfers data after BDA exit without reboot.

## Compatibility

The host parser must continue to reject malformed magic, sample count, and
sequence-complement fields. Protocol changes should introduce an explicit
version field or a new PID before changing the meaning of existing bytes.

## Source cleanup

The device core still contains historical compile-time probe branches. Remove
them incrementally only when the C3 build remains behaviorally equivalent and
the affected path has offline coverage. Avoid broad rewrites immediately
before a hardware test.
