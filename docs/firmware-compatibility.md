# Firmware Compatibility

## Status meanings

- **Hardware verified**: a real device completed CDC enumeration, PCM
  transfer, host capture, UI exit, and the required restart workflow.
- **Static candidate**: the recovery image hash, runtime signatures, capture
  ABI, UDC family, and offline BDA build have been checked. Real-device
  acceptance is still required.

A successful build alone is not a hardware compatibility claim.

## Recovery image matrix

All hashes below refer to the raw recovery payload loaded at `0x80004000`.
Firmware images are not included in this repository.

| Device | Firmware | SoC | SHA-256 | BDA | Status |
| --- | --- | --- | --- | --- | --- |
| 9588 / C200 | V3.30 | JZ4720 | `469A833E8984E8C4C531A411955EDC38F5FB57C6089B8694F522F995DBE66C49` | `9588UsbMic.bda` | Static candidate |
| 9588 / C200 | V3.30 | JZ4730 | `C7A4BA34D5A4C006F88B7E9E0C0A991B2B1E7A1EC8896859F0CC3DFB18E44270` | `9588UsbMic.bda` | Hardware verified backend and identical kernel payload |
| 9588 / C200 | V3.30 | JZ4740 | `02A16107B11A3281067871C6FE3D4C289C910D8DFA9924573DD87F00351D6525` | `9588UsbMic.bda` | Static candidate |
| 9688 / C100 | V2.32 | JZ4730 | `B18CE485A6CA6A8EA9EF5E3963D5970188FF7A2AEB4A6D4E58FE43625F49E514` | `9588UsbMic.bda` | Static candidate |
| 9688 / C100 | V2.32 | JZ4740 | `7EC707B69A1F4FA7016856B2EBA6900412D02D7DAEFC8B4D11654C63731ABCA0` | `9588UsbMic.bda` | Static candidate |

The verified C200/JZ4730 `C200knl.bin` has a 0x40-byte container header. Its
payload is byte-for-byte identical to the raw V3.30 JZ4730 `C200.bin` listed
above.

## Runtime selection

The public SDK `bda_audio.h` implementation uses `bda_hardware.h` to identify
9588/9688 and JZ4720/JZ4730/JZ4740, then checks exact instructions at the
capture init, read, ready, and stop entry points. The result, including an
unsupported result, is cached for the BDA session, so the 5 MiB OS-image scan
is not repeated while capturing PCM. The project maps the matched SDK audio
firmware ID to USB-only UDC and IRQ metadata before touching controller
registers. An SDK profile also fixes the capture calling convention:

| Profile | Capture init | Capture read | Capture ready | Capture stop | Init arguments |
| --- | ---: | ---: | ---: | ---: | --- |
| C200/JZ4720 | `0x801967F0` | `0x80195D84` | `0x80196CCC` | `0x8018B0D8` | `16000, 16, 1` |
| C200/JZ4730 | `0x80199D4C` | `0x80199290` | `0x8019A088` | `0x80199A6C` | none |
| C200/JZ4740 | `0x80194900` | `0x80193E94` | `0x80194DDC` | `0x801891E8` | `16000, 16, 1` |
| C100/JZ4730 | `0x801A169C` | `0x801A04D0` | `0x801A19D8` | `0x801A13BC` | none |
| C100/JZ4740 | `0x8019E400` | `0x8019D284` | `0x8019EB88` | `0x801925D8` | `16000, 16, 1` |

The JZ4730 profiles use the PCH-style UDC and IRQ12. The JZ4720/JZ4740
profiles use the indexed MUSB UDC and IRQ24. The MUSB backend follows the
firmware's own clock and endpoint reset sequence, forces Full Speed, and maps
CDC data to EP1 IN/OUT with notifications on EP2 IN.

The MUSB stop entries are the firmware wrappers used by its own recorder
cleanup paths. This avoids leaving the capture engine in a hand-written
partial AIC state.

The SDK is pinned as the `sdk` Git submodule and owns all private recording
addresses and signature checks. The single BDA dispatches to the PCH-style or
MUSB backend only after the SDK returns an exact audio profile and the local
USB metadata agrees with its device model and UDC family. Unknown
combinations are rejected before MMIO.

## Minimum acceptance matrix

The existing 9588/JZ4730 result does not need another exploratory probe.
Each remaining firmware needs one integrated run only:

1. Start `9588UsbMic.bda` with USB unplugged and confirm the live waveform.
2. Connect Windows and confirm the CDC COM device starts without a PnP error.
3. Capture 15 seconds and require valid PCM, no checksum errors, and a moving
   device waveform.
4. Unplug USB, exit with the on-screen button, and save
   `A:\应用\数据\9588usbmic.log`.
5. Restart the device before testing Mass Storage.

JZ4720 and JZ4740 cannot share one hardware result because their capture
initialization differs. C100 and C200 also use different private entry
addresses, so each static candidate remains unverified until its own single
integrated run succeeds.
