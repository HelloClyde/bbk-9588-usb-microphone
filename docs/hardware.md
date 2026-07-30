# Hardware and Lifecycle

## Proven target

- BBK 9588 family device.
- JZ4730 SoC.
- C200knl-compatible firmware ABI used by the current BDA.
- Full-speed USB device operation.

The code is tied to firmware entry points, IRQ table layout, and direct MMIO
addresses. A successful build does not imply compatibility with another
firmware image.

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

## Runtime ownership

The BDA:

1. Opens SDK audio capture and primes PCM buffers.
2. Opens its local UI.
3. Masks the stock USB IRQ path and installs the validated IRQ12 handler.
4. Initializes and connects the JZ4730 UDC as CDC ACM.
5. Services EP0 and EP2 from the IRQ path while capture/UI run in the main
   path.
6. Disconnects UDC and restores the previous IRQ handler on exit.

The current code does not restart the stock Mass Storage state machine.
Although the previous IRQ handler is restored, IRQ12 remains masked and the
device records `reboot_required=1`.

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

The SDK supplies 4096-byte PCM blocks. The device keeps a two-block ring and
sends one 32-byte PCM slice per 64-byte CDC frame. EP2 IRQ is enabled only
when buffered PCM is available, preventing an empty-token interrupt loop.

Large input levels can clip at `0x7FFF`/`0x8000`; gain control remains future
work and is independent of USB frame integrity.
