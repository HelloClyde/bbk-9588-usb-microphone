# Provenance and Distribution Boundary

This project is an independent, unofficial reverse-engineering effort. It is
not affiliated with or endorsed by BBK, Ingenic, or device manufacturers.

The repository contains:

- independently written BDA device code;
- independently written Windows host code;
- USB descriptors and protocol documentation;
- reconstructed firmware ABI declarations required to build the BDA.

The implementation was informed by:

- USB 2.0 and CDC ACM descriptor/request conventions;
- observed behavior of the target firmware and JZ4730 UDC;
- public Linux JZ47xx UDC and USB gadget driver behavior used for
  cross-checking register and control-transfer ordering.

Contributions must not add copied firmware code or third-party source without
preserving its original license and notices.

The optional Windows bridge references NAudio 2.3.0 under the MIT License.
Its license is preserved at `third-party/NAudio-LICENSE.txt`.

VB-CABLE is not repository source and is not covered by Apache-2.0.
Development installers download the original package from VB-Audio and verify
its pinned SHA-256 hash. Preserve the VB-Audio attribution and Donationware
notice, and obtain appropriate permission before offline or commercial
redistribution.

Firmware images, NAND dumps, original applications, trademarks, and other
third-party material are not distributed and are not licensed by this
project's Apache License 2.0.
