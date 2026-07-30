/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BDA_FIRMWARE_PROFILE_H
#define BDA_FIRMWARE_PROFILE_H

#include "bda_hardware.h"
#include "bda_types.h"
#include "bda/detail/runtime.h"

#define BDA_FIRMWARE_NONE              0u
#define BDA_FIRMWARE_C200_JZ4730       1u
#define BDA_FIRMWARE_C100_JZ4730       2u
#define BDA_FIRMWARE_C200_JZ4720       3u
#define BDA_FIRMWARE_C200_JZ4740       4u
#define BDA_FIRMWARE_C100_JZ4740       5u

#define BDA_MACHINE_C200 200u
#define BDA_MACHINE_C100 100u

#define BDA_SOC_JZ4720 BDA_CHIP_MODEL_JZ4720
#define BDA_SOC_JZ4730 BDA_CHIP_MODEL_JZ4730
#define BDA_SOC_JZ4740 BDA_CHIP_MODEL_JZ4740

#define BDA_UDC_PCH_STYLE 1u
#define BDA_UDC_MUSB      2u

#define BDA_CAPTURE_STOP_PRIVATE 1u

typedef struct bda_firmware_profile {
    u32 id;
    u32 machine;
    u32 soc;
    u32 udc_kind;
    const char *name;

    u32 capture_init;
    u32 capture_read;
    u32 capture_ready;
    u32 capture_stop;
    u32 capture_init_takes_format;
    u32 capture_stop_kind;

    u32 usb_init;
    u32 irq_register;
    u32 irq_unregister;
    u32 irq_vector_base;
    u32 stock_usb_irq;
    u32 udc_irq;
} bda_firmware_profile_t;

/*
 * These profiles describe exact recovery images. The image hashes and
 * container rules are kept in docs/firmware-compatibility.md. Runtime
 * selection requires both the public SDK hardware result and several exact
 * code words before any SoC-specific MMIO access.
 */
static const bda_firmware_profile_t bda_profile_c200_jz4730 = {
    BDA_FIRMWARE_C200_JZ4730,
    BDA_MACHINE_C200,
    BDA_SOC_JZ4730,
    BDA_UDC_PCH_STYLE,
    "C200/JZ4730",
    0x80199d4cu,
    0x80199290u,
    0x8019a088u,
    0x80199a6cu,
    0u,
    BDA_CAPTURE_STOP_PRIVATE,
    0x801890e0u,
    0x80004a34u,
    0x80004b64u,
    0x80479634u,
    0x8018a04cu,
    12u
};

static const bda_firmware_profile_t bda_profile_c100_jz4730 = {
    BDA_FIRMWARE_C100_JZ4730,
    BDA_MACHINE_C100,
    BDA_SOC_JZ4730,
    BDA_UDC_PCH_STYLE,
    "C100/JZ4730",
    0x801a169cu,
    0x801a04d0u,
    0x801a19d8u,
    0x801a13bcu,
    0u,
    BDA_CAPTURE_STOP_PRIVATE,
    0x80190200u,
    0x80004a34u,
    0x80004b64u,
    0x804a9e64u,
    0x8019116cu,
    12u
};

static const bda_firmware_profile_t bda_profile_c200_jz4720 = {
    BDA_FIRMWARE_C200_JZ4720,
    BDA_MACHINE_C200,
    BDA_SOC_JZ4720,
    BDA_UDC_MUSB,
    "C200/JZ4720",
    0x801967f0u,
    0x80195d84u,
    0x80196cccu,
    0x8018b0d8u,
    1u,
    BDA_CAPTURE_STOP_PRIVATE,
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static const bda_firmware_profile_t bda_profile_c200_jz4740 = {
    BDA_FIRMWARE_C200_JZ4740,
    BDA_MACHINE_C200,
    BDA_SOC_JZ4740,
    BDA_UDC_MUSB,
    "C200/JZ4740",
    0x80194900u,
    0x80193e94u,
    0x80194ddcu,
    0x801891e8u,
    1u,
    BDA_CAPTURE_STOP_PRIVATE,
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static const bda_firmware_profile_t bda_profile_c100_jz4740 = {
    BDA_FIRMWARE_C100_JZ4740,
    BDA_MACHINE_C100,
    BDA_SOC_JZ4740,
    BDA_UDC_MUSB,
    "C100/JZ4740",
    0x8019e400u,
    0x8019d284u,
    0x8019eb88u,
    0x801925d8u,
    1u,
    BDA_CAPTURE_STOP_PRIVATE,
    0u,
    0u,
    0u,
    0u,
    0u,
    24u
};

static inline int bda_profile_words_match(
    u32 address,
    u32 word0,
    u32 word1,
    u32 word2,
    u32 word3
) {
    const volatile u32 *code = (const volatile u32 *)address;
    return code[0] == word0 &&
        code[1] == word1 &&
        code[2] == word2 &&
        code[3] == word3;
}

static inline int bda_profile_capture_read_matches(u32 address) {
    return bda_profile_words_match(
        address,
        0x27bdffb8u,
        0xafbe0040u,
        0xafb50034u,
        0xafb40030u
    );
}

static inline int bda_profile_jz4730_stop_matches(u32 address) {
    return bda_profile_words_match(
        address,
        0x3c03b001u,
        0x34630080u,
        0x8c620000u,
        0x2406fffbu
    );
}

static inline int bda_profile_musb_stop_matches(
    u32 address, u32 first_call
) {
    return bda_profile_words_match(
        address,
        0x27bdffe8u,
        0xafbf0010u,
        first_call,
        0x00000000u
    );
}

static inline int bda_profile_matches_c200_jz4730(void) {
    return bda_profile_words_match(
            0x80199d4cu,
            0x27bdffe0u,
            0xafbf001cu,
            0x0c0669f2u,
            0xafb00018u
        ) &&
        bda_profile_capture_read_matches(0x80199290u) &&
        bda_profile_jz4730_stop_matches(0x80199a6cu) &&
        bda_profile_words_match(
            0x8019a088u,
            0x27bdffe8u,
            0xafbf0014u,
            0x0c001356u,
            0xafb00010u
        );
}

static inline int bda_profile_matches_c100_jz4730(void) {
    return bda_profile_words_match(
            0x801a169cu,
            0x27bdffe0u,
            0xafbf001cu,
            0x0c068846u,
            0xafb00018u
        ) &&
        bda_profile_capture_read_matches(0x801a04d0u) &&
        bda_profile_jz4730_stop_matches(0x801a13bcu) &&
        bda_profile_words_match(
            0x801a19d8u,
            0x27bdffe8u,
            0xafbf0014u,
            0x0c001356u,
            0xafb00010u
        );
}

static inline int bda_profile_matches_c200_jz4720(void) {
    const volatile u32 *ready = (const volatile u32 *)0x80196cccu;
    return bda_profile_words_match(
            0x801967f0u,
            0x27bdffd0u,
            0x00052e00u,
            0x00063600u,
            0xafbf002cu
        ) &&
        bda_profile_capture_read_matches(0x80195d84u) &&
        bda_profile_musb_stop_matches(
            0x8018b0d8u, 0x0c065f28u
        ) &&
        ready[0] == 0x27bdffe8u &&
        ready[1] == 0xafbf0014u &&
        ready[4] == 0x3c108058u &&
        ready[5] == 0x8e102d98u;
}

static inline int bda_profile_matches_c200_jz4740(void) {
    const volatile u32 *ready = (const volatile u32 *)0x80194ddcu;
    return bda_profile_words_match(
            0x80194900u,
            0x27bdffd0u,
            0x00052e00u,
            0x00063600u,
            0xafbf002cu
        ) &&
        bda_profile_capture_read_matches(0x80193e94u) &&
        bda_profile_musb_stop_matches(
            0x801891e8u, 0x0c06576cu
        ) &&
        ready[0] == 0x27bdffe8u &&
        ready[1] == 0xafbf0014u &&
        ready[4] == 0x3c108058u &&
        ready[5] == 0x8e100748u;
}

static inline int bda_profile_matches_c100_jz4740(void) {
    const volatile u32 *ready = (const volatile u32 *)0x8019eb88u;
    return bda_profile_words_match(
            0x8019e400u,
            0x27bdffd0u,
            0x00052e00u,
            0x00063600u,
            0xafbf002cu
        ) &&
        bda_profile_capture_read_matches(0x8019d284u) &&
        bda_profile_musb_stop_matches(
            0x801925d8u, 0x0c067ed8u
        ) &&
        ready[0] == 0x27bdffe8u &&
        ready[1] == 0xafbf0014u &&
        ready[4] == 0x3c10805bu &&
        ready[5] == 0x8e106358u;
}

static inline int bda_profile_hardware_matches(
    const bda_hardware_info_t *hardware,
    u32 machine,
    u32 soc
) {
    u32 device_model;

    if (machine == BDA_MACHINE_C200) {
        device_model = BDA_DEVICE_MODEL_9588;
    } else if (machine == BDA_MACHINE_C100) {
        device_model = BDA_DEVICE_MODEL_9688;
    } else {
        return 0;
    }

    return hardware->device_model == device_model &&
        hardware->chip_model == soc;
}

static inline const bda_firmware_profile_t *bda_firmware_profile_detect(void) {
    static const bda_firmware_profile_t *cached_profile;
    bda_hardware_info_t hardware;

    if (cached_profile) {
        return cached_profile;
    }
    if (!bda_sdk_internal_sys()) {
        return 0;
    }
    bda_detect_hardware(&hardware);

    if (
        bda_profile_hardware_matches(
            &hardware, BDA_MACHINE_C200, BDA_SOC_JZ4730
        ) &&
        bda_profile_matches_c200_jz4730()
    ) {
        cached_profile = &bda_profile_c200_jz4730;
    }
    else if (
        bda_profile_hardware_matches(
            &hardware, BDA_MACHINE_C100, BDA_SOC_JZ4730
        ) &&
        bda_profile_matches_c100_jz4730()
    ) {
        cached_profile = &bda_profile_c100_jz4730;
    }
    else if (
        bda_profile_hardware_matches(
            &hardware, BDA_MACHINE_C200, BDA_SOC_JZ4720
        ) &&
        bda_profile_matches_c200_jz4720()
    ) {
        cached_profile = &bda_profile_c200_jz4720;
    }
    else if (
        bda_profile_hardware_matches(
            &hardware, BDA_MACHINE_C200, BDA_SOC_JZ4740
        ) &&
        bda_profile_matches_c200_jz4740()
    ) {
        cached_profile = &bda_profile_c200_jz4740;
    }
    else if (
        bda_profile_hardware_matches(
            &hardware, BDA_MACHINE_C100, BDA_SOC_JZ4740
        ) &&
        bda_profile_matches_c100_jz4740()
    ) {
        cached_profile = &bda_profile_c100_jz4740;
    }
    return cached_profile;
}

static inline const bda_firmware_profile_t *bda_firmware_profile_by_id(u32 id) {
    switch (id) {
        case BDA_FIRMWARE_C200_JZ4730:
            return &bda_profile_c200_jz4730;
        case BDA_FIRMWARE_C100_JZ4730:
            return &bda_profile_c100_jz4730;
        case BDA_FIRMWARE_C200_JZ4720:
            return &bda_profile_c200_jz4720;
        case BDA_FIRMWARE_C200_JZ4740:
            return &bda_profile_c200_jz4740;
        case BDA_FIRMWARE_C100_JZ4740:
            return &bda_profile_c100_jz4740;
        default:
            return 0;
    }
}

#endif
