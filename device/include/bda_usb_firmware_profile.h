/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BDA_USB_FIRMWARE_PROFILE_H
#define BDA_USB_FIRMWARE_PROFILE_H

#include "bda_audio.h"
#include "bda_hardware.h"
#include "bda_types.h"

#define BDA_UDC_PCH_STYLE 1u
#define BDA_UDC_MUSB      2u

typedef struct bda_usb_firmware_profile {
    u32 audio_firmware;
    u32 device_model;
    u32 udc_kind;
    const char *name;
    u32 usb_init;
    u32 irq_register;
    u32 irq_unregister;
    u32 irq_vector_base;
    u32 stock_usb_irq;
    u32 udc_irq;
} bda_usb_firmware_profile_t;

/*
 * Capture addresses, calling conventions, and firmware signatures are owned
 * by the SDK audio API. This project-local table contains only USB transport
 * metadata selected by the SDK's exact matched capture firmware identifier.
 */
static const bda_usb_firmware_profile_t
bda_usb_profile_9588_jz4730 = {
    BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4730,
    BDA_DEVICE_MODEL_9588,
    BDA_UDC_PCH_STYLE,
    "9588/C200 JZ4730",
    0x801890e0u,
    0x80004a34u,
    0x80004b64u,
    0x80479634u,
    0x8018a04cu,
    12u
};

static const bda_usb_firmware_profile_t
bda_usb_profile_9688_jz4730 = {
    BDA_AUDIO_CAPTURE_FIRMWARE_9688_JZ4730,
    BDA_DEVICE_MODEL_9688,
    BDA_UDC_PCH_STYLE,
    "9688/C100 JZ4730",
    0x80190200u,
    0x80004a34u,
    0x80004b64u,
    0x804a9e64u,
    0x8019116cu,
    12u
};

static const bda_usb_firmware_profile_t
bda_usb_profile_9588_jz4720 = {
    BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4720,
    BDA_DEVICE_MODEL_9588,
    BDA_UDC_MUSB,
    "9588/C200 JZ4720",
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static const bda_usb_firmware_profile_t
bda_usb_profile_9588_jz4740 = {
    BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4740,
    BDA_DEVICE_MODEL_9588,
    BDA_UDC_MUSB,
    "9588/C200 JZ4740",
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static const bda_usb_firmware_profile_t
bda_usb_profile_9688_jz4740 = {
    BDA_AUDIO_CAPTURE_FIRMWARE_9688_JZ4740,
    BDA_DEVICE_MODEL_9688,
    BDA_UDC_MUSB,
    "9688/C100 JZ4740",
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static inline const bda_usb_firmware_profile_t *
bda_usb_firmware_profile_by_audio_firmware(u32 firmware) {
    switch (firmware) {
        case BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4730:
            return &bda_usb_profile_9588_jz4730;
        case BDA_AUDIO_CAPTURE_FIRMWARE_9688_JZ4730:
            return &bda_usb_profile_9688_jz4730;
        case BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4720:
            return &bda_usb_profile_9588_jz4720;
        case BDA_AUDIO_CAPTURE_FIRMWARE_9588_JZ4740:
            return &bda_usb_profile_9588_jz4740;
        case BDA_AUDIO_CAPTURE_FIRMWARE_9688_JZ4740:
            return &bda_usb_profile_9688_jz4740;
        default:
            return 0;
    }
}

static inline const bda_usb_firmware_profile_t *
bda_usb_firmware_profile_detect(void) {
    static const bda_usb_firmware_profile_t *cached_profile;
    static u32 detection_complete;
    const bda_audio_capture_profile_t *audio_profile;

    if (cached_profile != 0) {
        return cached_profile;
    }
    if (detection_complete != 0u) {
        return 0;
    }

    audio_profile = bda_audio_capture_profile();
    if (audio_profile != 0) {
        cached_profile = bda_usb_firmware_profile_by_audio_firmware(
            audio_profile->firmware
        );
        if (
            cached_profile != 0 &&
            cached_profile->device_model != audio_profile->device_model
        ) {
            cached_profile = 0;
        }
    }
    detection_complete = 1u;
    return cached_profile;
}

#endif
