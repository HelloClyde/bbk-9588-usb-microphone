/* SPDX-License-Identifier: Apache-2.0 */

/* Unified 9588/9688 CDC PCM entry point. */
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
#include "usb_cdc_pcm_musb.c"

__attribute__((section(".text.bda_main")))
int bda_main(void) {
    const bda_firmware_profile_t *profile = bda_firmware_profile_detect();

    if (!profile) {
        bda_msgbox(
            "BBK USB Mic",
            "Unsupported device, chip, or firmware."
        );
        return 1;
    }
    if (profile->udc_kind == BDA_UDC_PCH_STYLE) {
        return usb_cdc_pcm_pch_run();
    }
    if (profile->udc_kind == BDA_UDC_MUSB) {
        return usb_cdc_pcm_musb_run();
    }
    bda_msgbox("BBK USB Mic", "Unsupported USB controller.");
    return 1;
}
