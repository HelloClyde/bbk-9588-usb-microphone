# Hardware and Lifecycle

## Proven target

- BBK 9588/C200 family device.
- JZ4730 SoC.
- V3.30 JZ4730 payload matching the verified C200knl firmware.
- Full-speed USB device operation.

Additional exact recovery images are implemented as static candidates. See
`firmware-compatibility.md` for hashes and verification status. The code
remains tied to firmware entry points, IRQ layout, and direct MMIO addresses.

## Selected JZ4730 UDC map

The C3 profile uses UDC base `0xB3040000`.

| Function | Selected register/FIFO region |
| --- | --- |
| EP0 IN control/CSR | `+0x000` / `+0x004` |
| EP0 OUT CSR | `+0x204` |
| EP0 OUT/IN FIFO | `+0x800` / `+0x840` |
| CDC notification EP1 | control `+0x020`, CSR `+0x024` |
| CDC Bulk IN EP2 | control `+0x040`, CSR `+0x044`, FIFO `+0x8A0` |
| CDC Bulk OUT EP5 | control `+0x2A0`, CSR `+0x2A4`, FIFO `+0x800` |

This is the JZ4730 PCH-style endpoint layout established by the hardware
probes. It is not the JZ4740 MUSB indexed-register layout.

## Selected MUSB map

The JZ4720/JZ4740 backend uses the indexed MUSB registers at `0xB3040000`.

| Function | Endpoint/register |
| --- | --- |
| Control | EP0, FIFO `+0x20` |
| CDC Bulk IN/OUT | EP1, FIFO `+0x24` |
| CDC notification | EP2 IN, FIFO `+0x28` |
| Indexed endpoint control | `INDEX +0x0E`, `MAXP +0x10/+0x14`, `CSR +0x12/+0x16` |

The backend masks IRQ24 and polls the controller. It follows the original
firmware's `CLKGR +0x20` and USB clock `+0x24` sequence, but leaves the High
Speed enable clear so the 64-byte CDC Bulk descriptor remains valid.

## Runtime ownership

The JZ4730 BDA:

1. Resolves the SDK's exact audio profile, opens capture, and primes PCM
   buffers.
2. Opens its local UI.
3. Masks the stock USB IRQ path and installs the validated IRQ12 handler.
4. Initializes and connects the JZ4730 UDC as CDC ACM.
5. Services EP0 and EP2 from the IRQ path while capture/UI run in the main
   path.
6. Disconnects UDC and restores the previous IRQ handler on exit.

The current code does not restart the stock Mass Storage state machine.
Although the previous IRQ handler is restored, IRQ12 remains masked and the
device records `reboot_required=1`.

The MUSB BDA follows the same lifecycle but polls EP0/EP1, leaves IRQ24
masked, and gates the MUSB clock after disconnect. It also requires a restart.

## Required shutdown

Always:

1. Stop PC capture.
2. Physically disconnect USB.
3. Exit the BDA through its UI or Esc.
4. Reboot the device.

Register readback or `recovery_ok`-style internal checks are not evidence of
Mass Storage recovery. Only a complete Windows re-enumeration and successful
file transfer without reboot would establish that behavior.

## Capture behavior

The SDK supplies profile-gated 4096-byte PCM blocks and a nonblocking
`bda_audio_capture_ready()` poll. The device keeps a two-block ring and
sends one 32-byte PCM slice per 64-byte CDC frame. EP2 IRQ is enabled only
when buffered PCM is available, preventing an empty-token interrupt loop.

Large input levels can clip at `0x7FFF`/`0x8000`; gain control remains future
work and is independent of USB frame integrity.
