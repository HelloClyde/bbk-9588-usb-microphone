/* SPDX-License-Identifier: Apache-2.0 */

#include "../firmware_abi.h"
#include "bda_audio.h"
#include "bda_usb_firmware_profile.h"
#include "bda_live_ui.h"

/*
 * The release entry includes both USB backends in one translation unit.
 * Namespace every MUSB-local symbol so its state and helpers cannot alias the
 * probe-derived PCH backend.
 */
#define usb_setup_packet musb_backend_usb_setup_packet
#define usb_setup_packet_t musb_backend_usb_setup_packet_t
#define k_device_desc musb_backend_k_device_desc
#define k_config_desc musb_backend_k_config_desc
#define k_string0_desc musb_backend_k_string0_desc
#define k_string_mfr_desc musb_backend_k_string_mfr_desc
#define k_string_product_c200_desc \
    musb_backend_k_string_product_c200_desc
#define k_string_product_c100_desc \
    musb_backend_k_string_product_c100_desc
#define k_zero1 musb_backend_k_zero1
#define k_zero2 musb_backend_k_zero2
#define musb_device_desc_must_be_18 \
    musb_backend_device_desc_must_be_18
#define musb_config_desc_must_be_75 \
    musb_backend_config_desc_must_be_75
#define musb_c200_product_desc_must_be_32 \
    musb_backend_c200_product_desc_must_be_32
#define musb_c100_product_desc_must_be_32 \
    musb_backend_c100_product_desc_must_be_32
#define g_profile musb_backend_g_profile
#define g_capture musb_backend_g_capture
#define g_pcm_ring musb_backend_g_pcm_ring
#define g_pcm_read_slot musb_backend_g_pcm_read_slot
#define g_pcm_write_slot musb_backend_g_pcm_write_slot
#define g_pcm_block_count musb_backend_g_pcm_block_count
#define g_pcm_read_offset musb_backend_g_pcm_read_offset
#define g_ep0_tx_data musb_backend_g_ep0_tx_data
#define g_ep0_tx_len musb_backend_g_ep0_tx_len
#define g_ep0_tx_pos musb_backend_g_ep0_tx_pos
#define g_ep0_expect_out_len musb_backend_g_ep0_expect_out_len
#define g_ep0_out_action musb_backend_g_ep0_out_action
#define g_configured musb_backend_g_configured
#define g_address musb_backend_g_address
#define g_interface_alt musb_backend_g_interface_alt
#define g_line_coding musb_backend_g_line_coding
#define g_loop_count musb_backend_g_loop_count
#define g_original_intc_mask musb_backend_g_original_intc_mask
#define g_reset_count musb_backend_g_reset_count
#define g_setup_count musb_backend_g_setup_count
#define g_unsupported_count musb_backend_g_unsupported_count
#define g_configure_count musb_backend_g_configure_count
#define g_class_request_count musb_backend_g_class_request_count
#define g_capture_read_count musb_backend_g_capture_read_count
#define g_capture_error_count musb_backend_g_capture_error_count
#define g_capture_drop_count musb_backend_g_capture_drop_count
#define g_pcm_packet_count musb_backend_g_pcm_packet_count
#define g_pcm_byte_count musb_backend_g_pcm_byte_count
#define g_usb_byte_count musb_backend_g_usb_byte_count
#define g_sequence musb_backend_g_sequence
#define g_out_packet_count musb_backend_g_out_packet_count
#define g_out_byte_count musb_backend_g_out_byte_count
#define g_control_line_state musb_backend_g_control_line_state
#define g_break_value musb_backend_g_break_value
#define g_exit_reason musb_backend_g_exit_reason
#define min_u16 musb_backend_min_u16
#define tiny_delay musb_backend_tiny_delay
#define delay_ticks musb_backend_delay_ticks
#define append_text musb_backend_append_text
#define append_hex32 musb_backend_append_hex32
#define log_puts musb_backend_log_puts
#define log_reset musb_backend_log_reset
#define log_text musb_backend_log_text
#define log_value musb_backend_log_value
#define log_start musb_backend_log_start
#define log_summary musb_backend_log_summary
#define musb_index musb_backend_index
#define reset_control_state musb_backend_reset_control_state
#define musb_configure_endpoints musb_backend_configure_endpoints
#define musb_clock_enable musb_backend_clock_enable
#define musb_clock_disable musb_backend_clock_disable
#define musb_start musb_backend_start
#define musb_stop musb_backend_stop
#define fifo_write musb_backend_fifo_write
#define fifo_read musb_backend_fifo_read
#define fifo_discard musb_backend_fifo_discard
#define fifo_read_setup musb_backend_fifo_read_setup
#define ep0_stall musb_backend_ep0_stall
#define ep0_status_ack musb_backend_ep0_status_ack
#define ep0_continue_in musb_backend_ep0_continue_in
#define ep0_start_in musb_backend_ep0_start_in
#define ep0_start_out musb_backend_ep0_start_out
#define descriptor_for_request musb_backend_descriptor_for_request
#define handle_cdc_request musb_backend_handle_cdc_request
#define handle_standard_request musb_backend_handle_standard_request
#define handle_setup musb_backend_handle_setup
#define ep0_handle_out_payload musb_backend_ep0_handle_out_payload
#define service_ep0 musb_backend_service_ep0
#define service_data_out musb_backend_service_data_out
#define pcm_consume_frame musb_backend_pcm_consume_frame
#define service_data_in musb_backend_service_data_in
#define usb_service musb_backend_usb_service
#define capture_prepare musb_backend_capture_prepare
#define capture_drop_oldest musb_backend_capture_drop_oldest
#define capture_service musb_backend_capture_service
#define reset_counters musb_backend_reset_counters
#define musb_profile_detect musb_backend_profile_detect
#define run_loop musb_backend_run_loop

#ifdef REG8
#undef REG8
#endif
#ifdef REG32
#undef REG32
#endif
#ifdef EP0_MAX_PACKET
#undef EP0_MAX_PACKET
#endif
#ifdef PCM_BLOCK_WORDS
#undef PCM_BLOCK_WORDS
#endif

#define REG8(address)  (*(volatile u8 *)(address))
#define REG16(address) (*(volatile u16 *)(address))
#define REG32(address) (*(volatile u32 *)(address))

#define MUSB_BASE     0xb3040000u
#define MUSB_FADDR    (MUSB_BASE + 0x00u)
#define MUSB_POWER    (MUSB_BASE + 0x01u)
#define MUSB_INTRTX   (MUSB_BASE + 0x02u)
#define MUSB_INTRRX   (MUSB_BASE + 0x04u)
#define MUSB_INTRTXE  (MUSB_BASE + 0x06u)
#define MUSB_INTRRXE  (MUSB_BASE + 0x08u)
#define MUSB_INTRUSB  (MUSB_BASE + 0x0au)
#define MUSB_INTRUSBE (MUSB_BASE + 0x0bu)
#define MUSB_INDEX    (MUSB_BASE + 0x0eu)
#define MUSB_INMAXP   (MUSB_BASE + 0x10u)
#define MUSB_CSR0     (MUSB_BASE + 0x12u)
#define MUSB_INCSR    (MUSB_BASE + 0x12u)
#define MUSB_OUTMAXP  (MUSB_BASE + 0x14u)
#define MUSB_OUTCSR   (MUSB_BASE + 0x16u)
#define MUSB_COUNT0   (MUSB_BASE + 0x18u)
#define MUSB_FIFO0    (MUSB_BASE + 0x20u)
#define MUSB_FIFO1    (MUSB_BASE + 0x24u)

#define CPM_CLKGR   0xb0000020u
#define CPM_UHCCDR  0xb0000024u
#define INTC_MASK   0xb0001004u
#define INTC_IMSR   0xb0001008u
#define MUSB_IRQ    24u
#define MUSB_IRQ_BIT (1u << MUSB_IRQ)

#define MUSB_POWER_SOFT_CONNECT_FS 0x40u

#define MUSB_INTR_SUSPEND    0x01u
#define MUSB_INTR_RESUME     0x02u
#define MUSB_INTR_RESET      0x04u
#define MUSB_INTR_DISCONNECT 0x20u

#define CSR0_RXPKTRDY             0x0001u
#define CSR0_TXPKTRDY             0x0002u
#define CSR0_SENTSTALL            0x0004u
#define CSR0_DATAEND              0x0008u
#define CSR0_SETUPEND             0x0010u
#define CSR0_SENDSTALL            0x0020u
#define CSR0_SERVICED_RXPKTRDY    0x0040u
#define CSR0_SERVICED_SETUPEND    0x0080u

#define INCSR_INPKTRDY    0x0001u
#define INCSR_FLUSHFIFO   0x0008u
#define INCSR_CLRDATATOG  0x0040u
#define INCSR_MODE_IN     0x2000u
#define INCSR_RESET_VALUE \
    (INCSR_MODE_IN | INCSR_CLRDATATOG | INCSR_FLUSHFIFO)

#define OUTCSR_OUTPKTRDY   0x0001u
#define OUTCSR_FLUSHFIFO   0x0010u
#define OUTCSR_CLRDATATOG  0x0080u
#define OUTCSR_RESET_VALUE (OUTCSR_FLUSHFIFO | OUTCSR_CLRDATATOG)

#define USB_REQ_GET_STATUS        0u
#define USB_REQ_CLEAR_FEATURE     1u
#define USB_REQ_SET_FEATURE       3u
#define USB_REQ_SET_ADDRESS       5u
#define USB_REQ_GET_DESCRIPTOR    6u
#define USB_REQ_GET_CONFIGURATION 8u
#define USB_REQ_SET_CONFIGURATION 9u
#define USB_REQ_GET_INTERFACE     10u
#define USB_REQ_SET_INTERFACE     11u

#define USB_DESC_DEVICE        1u
#define USB_DESC_CONFIGURATION 2u
#define USB_DESC_STRING        3u

#define CDC_SET_LINE_CODING        0x20u
#define CDC_GET_LINE_CODING        0x21u
#define CDC_SET_CONTROL_LINE_STATE 0x22u
#define CDC_SEND_BREAK             0x23u

#define CDC_CONTROL_OUT_NONE        0u
#define CDC_CONTROL_OUT_LINE_CODING 1u
#define CDC_LINE_CODING_BYTES       7u

#define EP0_MAX_PACKET       64u
#define CDC_FRAME_BYTES      64u
#define CDC_PCM_BYTES        32u
#define CDC_PCM_WORDS        (CDC_PCM_BYTES / 4u)
#define CDC_PCM_SAMPLES      16u
#define CDC_PCM_MAGIC        0x38383539u
#define PCM_RING_BLOCKS      2u
#define PCM_BLOCK_BYTES      BDA_AUDIO_CAPTURE_BLOCK_BYTES
#define PCM_BLOCK_WORDS      (PCM_BLOCK_BYTES / 4u)

#define APP_DATA_DIR \
    "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd"
#define LOG_PATH APP_DATA_DIR "\\9588usbmic.log"
#define LOG_PREFIX "[UsbMic] "

typedef struct usb_setup_packet {
    u8 bm_request_type;
    u8 b_request;
    u16 w_value;
    u16 w_index;
    u16 w_length;
} usb_setup_packet_t;

static const u8 k_device_desc[] = {
    0x12, 0x01, 0x10, 0x01, 0xef, 0x02, 0x01, 0x40,
    0xa5, 0xa4, 0x56, 0x05, 0x00, 0x01, 0x01, 0x02,
    0x00, 0x01
};

/*
 * MUSB endpoint map:
 *   EP2 IN: CDC notification
 *   EP1 IN/OUT: CDC data
 */
static const u8 k_config_desc[] = {
    0x09, 0x02, 0x4b, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00,
    0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,
    0x05, 0x24, 0x00, 0x10, 0x01,
    0x05, 0x24, 0x01, 0x00, 0x01,
    0x04, 0x24, 0x02, 0x06,
    0x05, 0x24, 0x06, 0x00, 0x01,
    0x07, 0x05, 0x82, 0x03, 0x08, 0x00, 0x20,
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,
    0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00,
    0x07, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00
};

static const u8 k_string0_desc[] = {
    0x04, 0x03, 0x09, 0x04
};

static const u8 k_string_mfr_desc[] = {
    0x08, 0x03, 'B', 0x00, 'B', 0x00, 'K', 0x00
};

static const u8 k_string_product_c200_desc[] = {
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '3', 0x00
};

static const u8 k_string_product_c100_desc[] = {
    0x20, 0x03,
    '9', 0x00, '6', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '3', 0x00
};

static const u8 k_zero1[] = {0x00};
static const u8 k_zero2[] = {0x00, 0x00};

typedef char musb_device_desc_must_be_18[
    sizeof(k_device_desc) == 18u ? 1 : -1
];
typedef char musb_config_desc_must_be_75[
    sizeof(k_config_desc) == 75u ? 1 : -1
];
typedef char musb_c200_product_desc_must_be_32[
    sizeof(k_string_product_c200_desc) == 32u ? 1 : -1
];
typedef char musb_c100_product_desc_must_be_32[
    sizeof(k_string_product_c100_desc) == 32u ? 1 : -1
];

static const bda_usb_firmware_profile_t *g_profile;
static bda_audio_capture_t g_capture = BDA_AUDIO_CAPTURE_INITIALIZER;
static u32 g_pcm_ring[PCM_RING_BLOCKS][PCM_BLOCK_WORDS]
    __attribute__((aligned(4)));
static u32 g_pcm_read_slot;
static u32 g_pcm_write_slot;
static u32 g_pcm_block_count;
static u32 g_pcm_read_offset;

static const u8 *g_ep0_tx_data;
static u16 g_ep0_tx_len;
static u16 g_ep0_tx_pos;
static u16 g_ep0_expect_out_len;
static u8 g_ep0_out_action;
static u8 g_configured;
static u8 g_address;
static u8 g_interface_alt;
static u8 g_line_coding[CDC_LINE_CODING_BYTES] = {
    0x00, 0xc2, 0x01, 0x00, 0x00, 0x00, 0x08
};

static u32 g_loop_count;
static u32 g_original_intc_mask;
static u32 g_reset_count;
static u32 g_setup_count;
static u32 g_unsupported_count;
static u32 g_configure_count;
static u32 g_class_request_count;
static u32 g_capture_read_count;
static u32 g_capture_error_count;
static u32 g_capture_drop_count;
static u32 g_pcm_packet_count;
static u32 g_pcm_byte_count;
static u32 g_usb_byte_count;
static u32 g_sequence;
static u32 g_out_packet_count;
static u32 g_out_byte_count;
static u32 g_control_line_state;
static u32 g_break_value;
static u32 g_exit_reason;

static u16 min_u16(u16 left, u16 right) {
    return left < right ? left : right;
}

static void tiny_delay(void) {
    volatile u32 i;
    for (i = 0u; i < 2000u; ++i) {
    }
}

static void delay_ticks(u32 ticks) {
    u32 start = bda_gui_tick_count_25ms_like();
    while ((bda_gui_tick_count_25ms_like() - start) < ticks) {
        tiny_delay();
    }
}

static char *append_text(char *out, char *end, const char *text) {
    while (*text != 0 && out < end) {
        *out++ = *text++;
    }
    return out;
}

static char *append_hex32(char *out, char *end, u32 value) {
    static const char digits[] = "0123456789ABCDEF";
    int shift;
    out = append_text(out, end, "0x");
    for (shift = 28; shift >= 0; shift -= 4) {
        if (out < end) {
            *out++ = digits[(value >> (u32)shift) & 0x0fu];
        }
    }
    return out;
}

static int log_puts(const char *text) {
    int file;
    int written = 0;
    const char *cursor = text;
    bda_size_t length = 0u;
    file = bda_fs_fopen_raw(LOG_PATH, "ab");
    if (!bda_fs_file_is_valid(file)) {
        return 0;
    }
    while (*cursor++ != 0) {
        ++length;
    }
    if (length != 0u) {
        written = bda_fs_fwrite_raw(text, 1, length, file);
    }
    (void)bda_fs_close_raw(file);
    return length == 0u || written == (int)length;
}

static void log_reset(void) {
    int file = bda_fs_fopen_raw(LOG_PATH, "wb");
    if (bda_fs_file_is_valid(file)) {
        (void)bda_fs_close_raw(file);
    }
}

static void log_text(const char *name, const char *value) {
    char line[128];
    char *out = line;
    char *end = line + sizeof(line) - 1u;
    out = append_text(out, end, LOG_PREFIX);
    out = append_text(out, end, name);
    out = append_text(out, end, " ");
    out = append_text(out, end, value);
    out = append_text(out, end, "\r\n");
    *out = 0;
    (void)log_puts(line);
}

static void log_value(const char *name, u32 value) {
    char line[112];
    char *out = line;
    char *end = line + sizeof(line) - 1u;
    out = append_text(out, end, LOG_PREFIX);
    out = append_text(out, end, name);
    out = append_text(out, end, " ");
    out = append_hex32(out, end, value);
    out = append_text(out, end, "\r\n");
    *out = 0;
    (void)log_puts(line);
}

static void log_start(void) {
    log_reset();
    log_text("session", "start");
    log_text("backend", "MUSB");
    log_text("profile", g_profile ? g_profile->name : "unsupported");
}

static void log_summary(void) {
    log_text("session", "stop");
    log_text("profile", g_profile ? g_profile->name : "unsupported");
    log_value("exit_reason", g_exit_reason);
    log_value("reset_count", g_reset_count);
    log_value("setup_count", g_setup_count);
    log_value("unsupported_count", g_unsupported_count);
    log_value("configure_count", g_configure_count);
    log_value("class_request_count", g_class_request_count);
    log_value("capture_read_count", g_capture_read_count);
    log_value("capture_error_count", g_capture_error_count);
    log_value("capture_drop_count", g_capture_drop_count);
    log_value("pcm_packet_count", g_pcm_packet_count);
    log_value("pcm_byte_count", g_pcm_byte_count);
    log_value("usb_byte_count", g_usb_byte_count);
    log_value("out_packet_count", g_out_packet_count);
    log_value("out_byte_count", g_out_byte_count);
    log_value("original_intc_mask", g_original_intc_mask);
    log_value("irq24_left_masked", 1u);
    log_value("restart_required", 1u);
}

static void musb_index(u8 index) {
    REG8(MUSB_INDEX) = index;
}

static void reset_control_state(void) {
    g_ep0_tx_data = 0;
    g_ep0_tx_len = 0u;
    g_ep0_tx_pos = 0u;
    g_ep0_expect_out_len = 0u;
    g_ep0_out_action = CDC_CONTROL_OUT_NONE;
    g_configured = 0u;
    g_address = 0u;
    g_interface_alt = 0u;
}

static void musb_configure_endpoints(void) {
    REG16(MUSB_INTRTXE) = 0u;
    REG16(MUSB_INTRRXE) = 0u;
    REG8(MUSB_INTRUSBE) = 0u;
    REG8(MUSB_FADDR) = 0u;

    musb_index(0u);
    REG8(MUSB_CSR0) =
        CSR0_SERVICED_RXPKTRDY | CSR0_SERVICED_SETUPEND;

    musb_index(1u);
    REG16(MUSB_INMAXP) = 64u;
    REG16(MUSB_INCSR) = INCSR_RESET_VALUE;
    REG16(MUSB_OUTMAXP) = 64u;
    REG16(MUSB_OUTCSR) = OUTCSR_RESET_VALUE;

    musb_index(2u);
    REG16(MUSB_INMAXP) = 8u;
    REG16(MUSB_INCSR) = INCSR_RESET_VALUE;
    REG16(MUSB_OUTMAXP) = 0u;
    REG16(MUSB_OUTCSR) = 0u;

    musb_index(0u);
    REG16(MUSB_INTRTXE) = 0x0001u;
    REG16(MUSB_INTRRXE) = 0x0002u;
    REG8(MUSB_INTRUSBE) =
        MUSB_INTR_SUSPEND | MUSB_INTR_RESUME | MUSB_INTR_RESET;
    reset_control_state();
}

static void musb_clock_enable(void) {
    REG32(CPM_CLKGR) &= ~0x00000800u;
    REG32(CPM_UHCCDR) |= 0x00000040u;
}

static void musb_clock_disable(void) {
    REG32(CPM_UHCCDR) &= ~0x00000040u;
    REG32(CPM_CLKGR) |= 0x00000800u;
}

static void musb_start(void) {
    g_original_intc_mask = REG32(INTC_MASK);
    REG32(INTC_IMSR) = MUSB_IRQ_BIT;
    musb_clock_enable();
    REG8(MUSB_POWER) = 0u;
    delay_ticks(8u);
    (void)REG16(MUSB_INTRTX);
    (void)REG16(MUSB_INTRRX);
    (void)REG8(MUSB_INTRUSB);
    musb_configure_endpoints();
    REG8(MUSB_POWER) = MUSB_POWER_SOFT_CONNECT_FS;
}

static void musb_stop(void) {
    REG32(INTC_IMSR) = MUSB_IRQ_BIT;
    REG8(MUSB_POWER) = 0u;
    REG16(MUSB_INTRTXE) = 0u;
    REG16(MUSB_INTRRXE) = 0u;
    REG8(MUSB_INTRUSBE) = 0u;
    (void)REG16(MUSB_INTRTX);
    (void)REG16(MUSB_INTRRX);
    (void)REG8(MUSB_INTRUSB);
    musb_clock_disable();
}

static void fifo_write(u32 fifo_address, const u8 *data, u16 length) {
    volatile u32 *fifo32 = (volatile u32 *)fifo_address;
    volatile u8 *fifo8 = (volatile u8 *)fifo_address;
    u16 position = 0u;
    while ((u16)(position + 4u) <= length) {
        u32 value =
            (u32)data[position] |
            ((u32)data[position + 1u] << 8) |
            ((u32)data[position + 2u] << 16) |
            ((u32)data[position + 3u] << 24);
        *fifo32 = value;
        position = (u16)(position + 4u);
    }
    while (position < length) {
        *fifo8 = data[position++];
    }
}

static void fifo_read(u32 fifo_address, u8 *data, u16 length) {
    volatile u32 *fifo32 = (volatile u32 *)fifo_address;
    volatile u8 *fifo8 = (volatile u8 *)fifo_address;
    u16 position = 0u;
    while ((u16)(position + 4u) <= length) {
        u32 value = *fifo32;
        data[position] = (u8)value;
        data[position + 1u] = (u8)(value >> 8);
        data[position + 2u] = (u8)(value >> 16);
        data[position + 3u] = (u8)(value >> 24);
        position = (u16)(position + 4u);
    }
    while (position < length) {
        data[position++] = *fifo8;
    }
}

static void fifo_discard(u32 fifo_address, u16 length) {
    volatile u32 *fifo32 = (volatile u32 *)fifo_address;
    volatile u8 *fifo8 = (volatile u8 *)fifo_address;
    while (length >= 4u) {
        (void)*fifo32;
        length = (u16)(length - 4u);
    }
    while (length != 0u) {
        (void)*fifo8;
        --length;
    }
}

static void fifo_read_setup(usb_setup_packet_t *setup) {
    volatile u32 *fifo = (volatile u32 *)MUSB_FIFO0;
    u32 word0 = *fifo;
    u32 word1 = *fifo;
    setup->bm_request_type = (u8)word0;
    setup->b_request = (u8)(word0 >> 8);
    setup->w_value = (u16)(word0 >> 16);
    setup->w_index = (u16)word1;
    setup->w_length = (u16)(word1 >> 16);
}

static void ep0_stall(void) {
    musb_index(0u);
    g_ep0_tx_data = 0;
    g_ep0_expect_out_len = 0u;
    g_ep0_out_action = CDC_CONTROL_OUT_NONE;
    REG8(MUSB_CSR0) = CSR0_SERVICED_RXPKTRDY | CSR0_SENDSTALL;
    ++g_unsupported_count;
}

static void ep0_status_ack(void) {
    musb_index(0u);
    g_ep0_tx_data = 0;
    g_ep0_tx_len = 0u;
    g_ep0_tx_pos = 0u;
    g_ep0_expect_out_len = 0u;
    g_ep0_out_action = CDC_CONTROL_OUT_NONE;
    REG8(MUSB_CSR0) = CSR0_SERVICED_RXPKTRDY | CSR0_DATAEND;
}

static void ep0_continue_in(void) {
    u16 remaining;
    u16 packet_length;
    u8 csr;
    u8 flags;
    if (!g_ep0_tx_data) {
        return;
    }
    musb_index(0u);
    csr = REG8(MUSB_CSR0);
    if ((csr & CSR0_TXPKTRDY) != 0u) {
        return;
    }
    remaining = (u16)(g_ep0_tx_len - g_ep0_tx_pos);
    packet_length = min_u16(remaining, EP0_MAX_PACKET);
    if (packet_length != 0u) {
        fifo_write(
            MUSB_FIFO0,
            g_ep0_tx_data + g_ep0_tx_pos,
            packet_length
        );
        g_ep0_tx_pos = (u16)(g_ep0_tx_pos + packet_length);
    }
    flags = CSR0_TXPKTRDY;
    if (g_ep0_tx_pos >= g_ep0_tx_len) {
        flags |= CSR0_DATAEND;
        g_ep0_tx_data = 0;
    }
    REG8(MUSB_CSR0) = (u8)(REG8(MUSB_CSR0) | flags);
}

static void ep0_start_in(
    const u8 *data, u16 length, u16 requested_length
) {
    g_ep0_expect_out_len = 0u;
    g_ep0_out_action = CDC_CONTROL_OUT_NONE;
    g_ep0_tx_data = data;
    g_ep0_tx_len = min_u16(length, requested_length);
    g_ep0_tx_pos = 0u;
    musb_index(0u);
    REG8(MUSB_CSR0) = CSR0_SERVICED_RXPKTRDY;
    ep0_continue_in();
}

static void ep0_start_out(u16 length, u8 action) {
    g_ep0_tx_data = 0;
    g_ep0_tx_len = 0u;
    g_ep0_tx_pos = 0u;
    g_ep0_expect_out_len = length;
    g_ep0_out_action = action;
    musb_index(0u);
    REG8(MUSB_CSR0) = CSR0_SERVICED_RXPKTRDY;
}

static int descriptor_for_request(
    u16 value, const u8 **data, u16 *length
) {
    u8 type = (u8)(value >> 8);
    u8 index = (u8)value;
    if (type == USB_DESC_DEVICE && index == 0u) {
        *data = k_device_desc;
        *length = (u16)sizeof(k_device_desc);
        return 1;
    }
    if (type == USB_DESC_CONFIGURATION && index == 0u) {
        *data = k_config_desc;
        *length = (u16)sizeof(k_config_desc);
        return 1;
    }
    if (type == USB_DESC_STRING) {
        if (index == 0u) {
            *data = k_string0_desc;
            *length = (u16)sizeof(k_string0_desc);
            return 1;
        }
        if (index == 1u) {
            *data = k_string_mfr_desc;
            *length = (u16)sizeof(k_string_mfr_desc);
            return 1;
        }
        if (index == 2u) {
            if (
                g_profile &&
                g_profile->device_model == BDA_DEVICE_MODEL_9688
            ) {
                *data = k_string_product_c100_desc;
                *length = (u16)sizeof(k_string_product_c100_desc);
            } else {
                *data = k_string_product_c200_desc;
                *length = (u16)sizeof(k_string_product_c200_desc);
            }
            return 1;
        }
    }
    return 0;
}

static int handle_cdc_request(const usb_setup_packet_t *setup) {
    u8 direction_in = setup->bm_request_type & 0x80u;
    u8 interface_index = (u8)setup->w_index;
    if (interface_index != 0u) {
        return 0;
    }
    if (direction_in == 0u &&
        setup->b_request == CDC_SET_LINE_CODING &&
        setup->w_length == CDC_LINE_CODING_BYTES) {
        ++g_class_request_count;
        ep0_start_out(
            CDC_LINE_CODING_BYTES,
            CDC_CONTROL_OUT_LINE_CODING
        );
        return 1;
    }
    if (direction_in != 0u &&
        setup->b_request == CDC_GET_LINE_CODING) {
        ++g_class_request_count;
        ep0_start_in(
            g_line_coding,
            CDC_LINE_CODING_BYTES,
            setup->w_length
        );
        return 1;
    }
    if (direction_in == 0u &&
        setup->b_request == CDC_SET_CONTROL_LINE_STATE &&
        setup->w_length == 0u) {
        ++g_class_request_count;
        g_control_line_state = setup->w_value;
        ep0_status_ack();
        return 1;
    }
    if (direction_in == 0u &&
        setup->b_request == CDC_SEND_BREAK &&
        setup->w_length == 0u) {
        ++g_class_request_count;
        g_break_value = setup->w_value;
        ep0_status_ack();
        return 1;
    }
    return 0;
}

static void handle_standard_request(const usb_setup_packet_t *setup) {
    const u8 *descriptor;
    u16 descriptor_length;
    u8 recipient = setup->bm_request_type & 0x1fu;
    u8 direction_in = setup->bm_request_type & 0x80u;
    u8 value_low = (u8)setup->w_value;
    u8 index_low = (u8)setup->w_index;

    switch (setup->b_request) {
        case USB_REQ_GET_STATUS:
            if (direction_in != 0u && setup->w_length >= 2u) {
                ep0_start_in(k_zero2, 2u, setup->w_length);
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_CLEAR_FEATURE:
        case USB_REQ_SET_FEATURE:
            if (direction_in == 0u && setup->w_length == 0u) {
                ep0_status_ack();
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_SET_ADDRESS:
            if (recipient == 0u &&
                direction_in == 0u &&
                setup->w_length == 0u &&
                setup->w_value <= 127u) {
                g_address = value_low;
                ep0_status_ack();
                REG8(MUSB_FADDR) = g_address;
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_GET_DESCRIPTOR:
            if (direction_in != 0u &&
                descriptor_for_request(
                    setup->w_value,
                    &descriptor,
                    &descriptor_length)) {
                ep0_start_in(
                    descriptor,
                    descriptor_length,
                    setup->w_length
                );
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_GET_CONFIGURATION:
            if (direction_in != 0u && setup->w_length >= 1u) {
                ep0_start_in(&g_configured, 1u, setup->w_length);
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_SET_CONFIGURATION:
            if (recipient == 0u &&
                direction_in == 0u &&
                setup->w_length == 0u &&
                (setup->w_value == 0u || setup->w_value == 1u)) {
                g_configured = value_low;
                g_interface_alt = 0u;
                if (g_configured != 0u) {
                    ++g_configure_count;
                }
                ep0_status_ack();
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_GET_INTERFACE:
            if (recipient == 1u &&
                direction_in != 0u &&
                setup->w_length >= 1u &&
                (index_low == 0u || index_low == 1u)) {
                ep0_start_in(k_zero1, 1u, setup->w_length);
            } else {
                ep0_stall();
            }
            break;
        case USB_REQ_SET_INTERFACE:
            if (recipient == 1u &&
                direction_in == 0u &&
                setup->w_length == 0u &&
                (index_low == 0u || index_low == 1u) &&
                value_low == 0u) {
                g_interface_alt = 0u;
                ep0_status_ack();
            } else {
                ep0_stall();
            }
            break;
        default:
            ep0_stall();
            break;
    }
}

static void handle_setup(const usb_setup_packet_t *setup) {
    ++g_setup_count;
    if ((setup->bm_request_type & 0x60u) == 0x00u) {
        handle_standard_request(setup);
        return;
    }
    if ((setup->bm_request_type & 0x60u) == 0x20u &&
        handle_cdc_request(setup)) {
        return;
    }
    ep0_stall();
}

static void ep0_handle_out_payload(void) {
    u16 count;
    musb_index(0u);
    count = REG16(MUSB_COUNT0);
    if (g_ep0_out_action == CDC_CONTROL_OUT_LINE_CODING &&
        count == CDC_LINE_CODING_BYTES) {
        fifo_read(MUSB_FIFO0, g_line_coding, count);
    } else {
        fifo_discard(MUSB_FIFO0, count);
        ++g_unsupported_count;
    }
    g_ep0_expect_out_len = 0u;
    g_ep0_out_action = CDC_CONTROL_OUT_NONE;
    REG8(MUSB_CSR0) =
        CSR0_SERVICED_RXPKTRDY | CSR0_DATAEND;
}

static void service_ep0(void) {
    usb_setup_packet_t setup;
    u8 csr;
    musb_index(0u);
    csr = REG8(MUSB_CSR0);
    if ((csr & CSR0_SENTSTALL) != 0u) {
        REG8(MUSB_CSR0) = (u8)(csr & (u8)~CSR0_SENDSTALL);
        g_ep0_tx_data = 0;
        g_ep0_expect_out_len = 0u;
        g_ep0_out_action = CDC_CONTROL_OUT_NONE;
        return;
    }
    if ((csr & CSR0_SETUPEND) != 0u) {
        REG8(MUSB_CSR0) = CSR0_SERVICED_SETUPEND;
        g_ep0_tx_data = 0;
        g_ep0_expect_out_len = 0u;
        g_ep0_out_action = CDC_CONTROL_OUT_NONE;
        return;
    }
    if ((csr & CSR0_RXPKTRDY) != 0u) {
        if (g_ep0_expect_out_len != 0u) {
            ep0_handle_out_payload();
        } else {
            fifo_read_setup(&setup);
            handle_setup(&setup);
        }
        return;
    }
    ep0_continue_in();
}

static void service_data_out(void) {
    u16 csr;
    u16 count;
    musb_index(1u);
    csr = REG16(MUSB_OUTCSR);
    if ((csr & OUTCSR_OUTPKTRDY) == 0u) {
        return;
    }
    count = REG16(MUSB_COUNT0);
    fifo_discard(MUSB_FIFO1, count);
    REG16(MUSB_OUTCSR) = csr & (u16)~OUTCSR_OUTPKTRDY;
    ++g_out_packet_count;
    g_out_byte_count += count;
}

static void pcm_consume_frame(void) {
    g_pcm_read_offset += CDC_PCM_BYTES;
    if (g_pcm_read_offset >= PCM_BLOCK_BYTES) {
        g_pcm_read_offset = 0u;
        g_pcm_read_slot ^= 1u;
        if (g_pcm_block_count != 0u) {
            --g_pcm_block_count;
        }
    }
}

static void service_data_in(void) {
    volatile u32 *fifo = (volatile u32 *)MUSB_FIFO1;
    const u32 *pcm;
    u16 csr;
    u32 sequence;
    u32 i;
    if (g_configured == 0u || g_pcm_block_count == 0u) {
        return;
    }
    musb_index(1u);
    csr = REG16(MUSB_INCSR);
    if ((csr & INCSR_INPKTRDY) != 0u) {
        return;
    }

    sequence = g_sequence++;
    *fifo = CDC_PCM_MAGIC;
    *fifo = sequence;
    *fifo = g_loop_count;
    *fifo = CDC_PCM_SAMPLES | (1u << 16);
    pcm = &g_pcm_ring[g_pcm_read_slot][g_pcm_read_offset >> 2];
    for (i = 0u; i < CDC_PCM_WORDS; ++i) {
        *fifo = pcm[i];
    }
    *fifo = g_pcm_packet_count + 1u;
    *fifo = g_capture_read_count;
    *fifo = 0u;
    *fifo = ~sequence;
    __asm__ volatile("sync" ::: "memory");
    REG16(MUSB_INCSR) = csr | INCSR_INPKTRDY;
    ++g_pcm_packet_count;
    g_pcm_byte_count += CDC_PCM_BYTES;
    g_usb_byte_count += CDC_FRAME_BYTES;
    pcm_consume_frame();
}

static void usb_service(void) {
    u16 intrtx = REG16(MUSB_INTRTX);
    u16 intrrx = REG16(MUSB_INTRRX);
    u8 intrusb = REG8(MUSB_INTRUSB);
    u8 csr0;

    if ((intrusb & MUSB_INTR_RESET) != 0u) {
        ++g_reset_count;
        musb_configure_endpoints();
    } else if ((intrusb & MUSB_INTR_DISCONNECT) != 0u) {
        g_configured = 0u;
    }

    if ((intrtx & 0x0001u) != 0u || g_ep0_tx_data != 0) {
        service_ep0();
    } else {
        musb_index(0u);
        csr0 = REG8(MUSB_CSR0);
        if ((csr0 &
             (CSR0_RXPKTRDY | CSR0_SETUPEND | CSR0_SENTSTALL)) != 0u) {
            service_ep0();
        }
    }
    if ((intrrx & 0x0002u) != 0u) {
        service_data_out();
    } else {
        musb_index(1u);
        if ((REG16(MUSB_OUTCSR) & OUTCSR_OUTPKTRDY) != 0u) {
            service_data_out();
        }
    }
    service_data_in();
}

static int musb_backend_capture_ready(void) {
    return bda_audio_capture_ready(&g_capture) == 1;
}

static int capture_prepare(void) {
    int result = bda_audio_capture_open(&g_capture);
    if (result != BDA_AUDIO_CAPTURE_OK) {
        return 0;
    }
    result = bda_audio_capture_read(
        &g_capture, g_pcm_ring[0], PCM_BLOCK_BYTES
    );
    if (result != (int)PCM_BLOCK_BYTES) {
        ++g_capture_error_count;
        (void)bda_audio_capture_stop(&g_capture);
        return 0;
    }
    g_pcm_read_slot = 0u;
    g_pcm_write_slot = 1u;
    g_pcm_block_count = 1u;
    g_pcm_read_offset = 0u;
    g_capture_read_count = 1u;
    bda_live_ui_analyze_pcm(g_pcm_ring[0]);
    return 1;
}

static void capture_drop_oldest(void) {
    if (g_pcm_block_count == 0u) {
        return;
    }
    g_pcm_read_slot ^= 1u;
    g_pcm_read_offset = 0u;
    --g_pcm_block_count;
    ++g_capture_drop_count;
}

static void capture_service(void) {
    int result;
    if (!musb_backend_capture_ready()) {
        return;
    }
    if (g_pcm_block_count >= PCM_RING_BLOCKS) {
        if (g_configured != 0u) {
            g_sequence +=
                (PCM_BLOCK_BYTES - g_pcm_read_offset) / CDC_PCM_BYTES;
        }
        capture_drop_oldest();
    }
    result = bda_audio_capture_read(
        &g_capture,
        g_pcm_ring[g_pcm_write_slot],
        PCM_BLOCK_BYTES
    );
    if (result != (int)PCM_BLOCK_BYTES) {
        ++g_capture_error_count;
        return;
    }
    bda_live_ui_analyze_pcm(g_pcm_ring[g_pcm_write_slot]);
    g_pcm_write_slot ^= 1u;
    ++g_pcm_block_count;
    ++g_capture_read_count;
}

static void reset_counters(void) {
    g_loop_count = 0u;
    g_reset_count = 0u;
    g_setup_count = 0u;
    g_unsupported_count = 0u;
    g_configure_count = 0u;
    g_class_request_count = 0u;
    g_capture_read_count = 0u;
    g_capture_error_count = 0u;
    g_capture_drop_count = 0u;
    g_pcm_packet_count = 0u;
    g_pcm_byte_count = 0u;
    g_usb_byte_count = 0u;
    g_sequence = 0u;
    g_out_packet_count = 0u;
    g_out_byte_count = 0u;
    g_control_line_state = 0u;
    g_break_value = 0u;
    g_exit_reason = 0u;
}

static int musb_profile_detect(void) {
    g_profile = bda_usb_firmware_profile_detect();
    return g_profile &&
        g_profile->udc_kind == BDA_UDC_MUSB &&
        g_profile->udc_irq == MUSB_IRQ;
}

static void run_loop(void) {
    while (bda_live_ui_exit_reason() == BDA_LIVE_UI_EXIT_NONE) {
        ++g_loop_count;
        usb_service();
        capture_service();
        bda_live_ui_set_streaming(g_configured != 0u);
        bda_live_ui_service();
        usb_service();
        tiny_delay();
    }
    g_exit_reason = bda_live_ui_exit_reason();
}

static int usb_cdc_pcm_musb_run(void) {
    reset_counters();
    if (!musb_profile_detect()) {
        log_reset();
        log_text("error", "unsupported_firmware_or_wrong_backend");
        bda_msgbox(
            "BBK USB Mic",
            "Unsupported firmware or USB controller."
        );
        return 0;
    }

    log_start();
    if (!bda_live_ui_open()) {
        log_text("error", "ui_open_failed");
        bda_msgbox("BBK USB Mic", "Unable to open the live waveform.");
        return 0;
    }
    if (!capture_prepare()) {
        log_text("error", "capture_open_failed");
        bda_live_ui_close();
        bda_msgbox("BBK USB Mic", "Unable to start microphone capture.");
        return 0;
    }

    bda_live_ui_set_recording(1u);
    musb_start();
    bda_live_ui_arm_input();
    run_loop();

    musb_stop();
    (void)bda_audio_capture_stop(&g_capture);
    bda_live_ui_set_streaming(0u);
    bda_live_ui_set_recording(0u);
    bda_live_ui_close();
    log_summary();
    bda_msgbox(
        "BBK USB Mic",
        "USB storage is unavailable until restart.\n"
        "Restart the device before reconnecting USB."
    );
    return 0;
}
