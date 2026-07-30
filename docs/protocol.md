# CDC PCM Protocol

## USB identity

| Field | Value |
| --- | --- |
| VID | `0xA4A5` |
| PID | `0x0556` |
| Product | `9588 CDC PCM C3` |
| Device class | `EF/02/01` |
| EP0 max packet | 32 bytes |
| Configuration bytes | 75 |
| Interfaces | CDC control 0, CDC data 1 |

The configuration starts with an Interface Association Descriptor:

```text
08 0B 00 02 02 02 01 00
```

Windows binds the composite parent to `usbccgp` and the CDC function to
`usbser`. The IAD is required for the proven Windows path.

## Endpoints

| Address | Type | Max packet | Purpose |
| --- | --- | ---: | --- |
| `0x81` | Interrupt IN | 8 | CDC notification |
| `0x82` | Bulk IN | 64 | PCM frames |
| `0x05` | Bulk OUT | 64 | Reserved CDC host-to-device data |

The current host only consumes `0x82`. Baud rate is not used to pace USB Bulk
traffic; the Windows host configures 115200/8N1 only to complete normal
`usbser` setup.

## Audio format

| Property | Value |
| --- | --- |
| Sample rate | 16,000 Hz |
| Channels | 1 |
| Encoding | signed PCM |
| Sample width | 16 bits |
| Byte order | little-endian |
| Samples per frame | 16 |
| PCM bytes per frame | 32 |

At 1,000 frames per second this carries exactly 16,000 samples per second.

## 64-byte frame

All integer fields are little-endian.

| Offset | Size | Field |
| ---: | ---: | --- |
| `0x00` | 4 | Magic `0x38383539` (`"9588"` on wire) |
| `0x04` | 4 | Monotonic sequence |
| `0x08` | 4 | Device loop counter |
| `0x0C` | 2 | Sample count, currently 16 |
| `0x0E` | 2 | Flags |
| `0x10` | 32 | 16 signed PCM samples |
| `0x30` | 4 | Device packet ordinal |
| `0x34` | 4 | SDK capture read counter |
| `0x38` | 4 | Device underrun counter |
| `0x3C` | 4 | Bitwise complement of sequence |

Flags:

| Bit | Meaning |
| ---: | --- |
| 0 | PCM payload is present |
| 1 | Device reported an underrun/silence frame |

The host accepts a frame only when:

- magic is correct;
- sample count is 16;
- the final word equals `~sequence`.

Sequence gaps are filled with zero samples in the output WAV and recorded in
the capture log.

## Control requests

The device supports standard enumeration requests and the CDC ACM requests
used by Windows:

- `SET_LINE_CODING`
- `GET_LINE_CODING`
- `SET_CONTROL_LINE_STATE`
- `SEND_BREAK`

The proven run observed 34 setup requests, including 18 CDC class requests,
with no unsupported request.

## Compatibility rules

Do not change existing field meaning under PID `0x0556`. A breaking frame or
descriptor change requires a new PID or an explicit protocol version that old
hosts can reject deterministically.
