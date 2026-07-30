/* SPDX-License-Identifier: Apache-2.0 */

/*
 * JZ4730 CDC ACM PCM transport for the BBK 9588.
 *
 * This selects the C3 profile that was validated on real hardware. The core
 * intentionally preserves the probe-derived implementation as the starting
 * point for later module extraction.
 */
#define USB_MIC_SILENCE_PROBE 1
#define USB_MIC_IAD_PROBE 1
#define USB_MIC_EP4_PROBE 1
#define USB_MIC_P12_PROBE 1
#define USB_MIC_P13_PROBE 1
#define USB_MIC_P14_PROBE 1
#define USB_MIC_P15_PROBE 1
#define USB_MIC_P16_PROBE 1
#define USB_MIC_P17_PROBE 1
#define USB_MIC_P18_PROBE 1
#define USB_MIC_P19_PROBE 1
#define USB_PCM_CDC_PROBE 1
#define USB_PCM_CDC_C3_PROBE 1
#define USB_PCM_RELEASE_UI 1

#include "usb_cdc_pcm_core.c"
