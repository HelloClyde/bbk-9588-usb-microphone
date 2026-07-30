/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BDA_LIVE_UI_H
#define BDA_LIVE_UI_H

#include "../firmware_abi.h"
#include "bda_firmware_audio.h"
#include "bda_graphics.h"
#include "bda_input.h"
#include "bda_window.h"

#define BDA_LIVE_UI_SCREEN_WIDTH        240
#define BDA_LIVE_UI_SCREEN_HEIGHT       320
#define BDA_LIVE_UI_WAVE_X              18
#define BDA_LIVE_UI_WAVE_COLUMNS        204u
#define BDA_LIVE_UI_WAVE_CENTER_Y       140
#define BDA_LIVE_UI_WAVE_HALF_HEIGHT    56
#define BDA_LIVE_UI_WAVE_TOP            \
    (BDA_LIVE_UI_WAVE_CENTER_Y - BDA_LIVE_UI_WAVE_HALF_HEIGHT)
#define BDA_LIVE_UI_WAVE_GAIN           2
#define BDA_LIVE_UI_WAVE_GRID_STEP      50
#define BDA_LIVE_UI_WAVE_SAMPLES_COLUMN 10u
#define BDA_LIVE_UI_EXIT_X              40
#define BDA_LIVE_UI_EXIT_Y              256
#define BDA_LIVE_UI_EXIT_WIDTH          160
#define BDA_LIVE_UI_EXIT_HEIGHT         42
#define BDA_LIVE_UI_RECORD_DOT_X        18
#define BDA_LIVE_UI_LINK_DOT_X          136
#define BDA_LIVE_UI_STATUS_DOT_Y        225
#define BDA_LIVE_UI_STATUS_DOT_SIZE     8
#define BDA_LIVE_UI_RAW_EVENT_DRAIN     8u
#define BDA_LIVE_UI_INPUT_ARM_TICKS     20u

#define BDA_LIVE_UI_EXIT_NONE   0u
#define BDA_LIVE_UI_EXIT_ESCAPE 1u
#define BDA_LIVE_UI_EXIT_BUTTON 2u

typedef char bda_live_ui_wave_samples_fit_capture_block[
    (BDA_LIVE_UI_WAVE_COLUMNS * BDA_LIVE_UI_WAVE_SAMPLES_COLUMN <=
     BDA_AUDIO_CAPTURE_BLOCK_BYTES / 2u) ? 1 : -1
];

static bda_handle_t bda_live_ui_frame;
static bda_handle_t bda_live_ui_draw;
static bda_handle_t bda_live_ui_draw_owner;
static void *bda_live_ui_draw_object;
static s16 bda_live_ui_wave_min[BDA_LIVE_UI_WAVE_COLUMNS];
static s16 bda_live_ui_wave_max[BDA_LIVE_UI_WAVE_COLUMNS];
static s16 bda_live_ui_wave_mean[BDA_LIVE_UI_WAVE_COLUMNS];
static s16 bda_live_ui_snapshot_min[BDA_LIVE_UI_WAVE_COLUMNS];
static s16 bda_live_ui_snapshot_max[BDA_LIVE_UI_WAVE_COLUMNS];
static s16 bda_live_ui_snapshot_mean[BDA_LIVE_UI_WAVE_COLUMNS];
static u16 bda_live_ui_drawn_min_y[BDA_LIVE_UI_WAVE_COLUMNS];
static u16 bda_live_ui_drawn_max_y[BDA_LIVE_UI_WAVE_COLUMNS];
static u32 bda_live_ui_wave_generation;
static u32 bda_live_ui_snapshot_generation;
static u32 bda_live_ui_wave_column;
static u32 bda_live_ui_last_tick;
static u32 bda_live_ui_input_arm_tick;
static u32 bda_live_ui_previous_escape;
static u32 bda_live_ui_record_state;
static u32 bda_live_ui_stream_state;
static u32 bda_live_ui_initialized;
static u32 bda_live_ui_detached;
static u32 bda_live_ui_touch_button_down;
static u32 bda_live_ui_exit;
static u32 bda_live_ui_color_panel;
static u32 bda_live_ui_color_grid;
static u32 bda_live_ui_color_wave;
static u32 bda_live_ui_color_mean;
static u32 bda_live_ui_color_record_on;
static u32 bda_live_ui_color_record_off;
static u32 bda_live_ui_color_stream_on;
static u32 bda_live_ui_color_stream_off;
static u32 bda_live_ui_color_exit;
static u32 bda_live_ui_color_exit_pressed;

static void bda_live_ui_tiny_delay(void) {
    volatile u32 i;
    for (i = 0u; i < 2000u; ++i) {
    }
}

static void bda_live_ui_release_draw(void) {
    bda_handle_t draw = bda_live_ui_draw;
    if (!draw || (s32)draw == -1) {
        bda_live_ui_draw = 0;
        bda_live_ui_draw_owner = 0;
        return;
    }
    bda_live_ui_draw = 0;
    bda_live_ui_draw_owner = 0;
    bda_gui_end_draw(draw);
}

static int bda_live_ui_acquire_draw(bda_handle_t owner) {
    if (bda_live_ui_draw && bda_live_ui_draw_owner == owner) {
        return 1;
    }
    bda_live_ui_release_draw();
    bda_live_ui_draw = bda_gui_current_draw(owner);
    if (!bda_live_ui_draw || (s32)bda_live_ui_draw == -1) {
        bda_live_ui_draw = 0;
        return 0;
    }
    bda_live_ui_draw_owner = owner;
    return 1;
}

static int bda_live_ui_window_proc(
    bda_handle_t handle, u32 message, u32 wparam, u32 lparam
) {
    if (message == BDA_MSG_DRAW_CONTEXT_ATTACH) {
        bda_live_ui_frame = handle;
        (void)bda_live_ui_acquire_draw(handle);
        if (!bda_live_ui_draw_object) {
            bda_live_ui_draw_object = bda_gui_draw_object_create(7u);
        }
    } else if (message == BDA_MSG_DRAW_CONTEXT_DETACH) {
        if (!bda_live_ui_draw_owner || bda_live_ui_draw_owner == handle) {
            bda_live_ui_release_draw();
        }
        bda_live_ui_detached = 1u;
    }
    return bda_gui_default_proc(handle, message, wparam, lparam);
}

static void bda_live_ui_outline(
    s32 left, s32 top, s32 right, s32 bottom, u32 color
) {
    s32 x;
    s32 y;
    for (x = left; x <= right; ++x) {
        (void)bda_gui_put_pixel(bda_live_ui_draw, x, top, color);
        (void)bda_gui_put_pixel(bda_live_ui_draw, x, bottom, color);
    }
    for (y = top + 1; y < bottom; ++y) {
        (void)bda_gui_put_pixel(bda_live_ui_draw, left, y, color);
        (void)bda_gui_put_pixel(bda_live_ui_draw, right, y, color);
    }
}

static int bda_live_ui_present_initial(void) {
    void *old_object;
    u32 foreground;
    u32 muted;
    u32 accent;
    u32 button_fill;
    s32 x;
    s32 y;

    foreground = (u32)bda_gui_rgb(bda_live_ui_draw, 240u, 243u, 239u);
    muted = (u32)bda_gui_rgb(bda_live_ui_draw, 154u, 164u, 163u);
    accent = (u32)bda_gui_rgb(bda_live_ui_draw, 30u, 210u, 213u);
    button_fill = (u32)bda_gui_rgb(bda_live_ui_draw, 90u, 31u, 35u);
    bda_live_ui_color_panel =
        (u32)bda_gui_rgb(bda_live_ui_draw, 5u, 7u, 8u);
    bda_live_ui_color_grid =
        (u32)bda_gui_rgb(bda_live_ui_draw, 50u, 58u, 59u);
    bda_live_ui_color_wave =
        (u32)bda_gui_rgb(bda_live_ui_draw, 30u, 210u, 213u);
    bda_live_ui_color_mean =
        (u32)bda_gui_rgb(bda_live_ui_draw, 246u, 196u, 65u);
    bda_live_ui_color_record_on =
        (u32)bda_gui_rgb(bda_live_ui_draw, 238u, 92u, 70u);
    bda_live_ui_color_record_off =
        (u32)bda_gui_rgb(bda_live_ui_draw, 238u, 160u, 47u);
    bda_live_ui_color_stream_on =
        (u32)bda_gui_rgb(bda_live_ui_draw, 54u, 196u, 113u);
    bda_live_ui_color_stream_off = bda_live_ui_color_record_off;
    bda_live_ui_color_exit = bda_live_ui_color_record_on;
    bda_live_ui_color_exit_pressed = bda_live_ui_color_mean;

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(
        bda_live_ui_draw, bda_live_ui_draw_object
    );
    for (x = 0; x < BDA_LIVE_UI_SCREEN_WIDTH; ++x) {
        (void)bda_gui_put_pixel(bda_live_ui_draw, x, 49, accent);
        (void)bda_gui_put_pixel(bda_live_ui_draw, x, 50, accent);
    }
    bda_live_ui_outline(12, 70, 227, 207, bda_live_ui_color_grid);
    bda_live_ui_outline(14, 72, 225, 205, bda_live_ui_color_grid);
    for (y = BDA_LIVE_UI_WAVE_TOP;
         y <= BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT;
         y += BDA_LIVE_UI_WAVE_HALF_HEIGHT / 2) {
        for (x = BDA_LIVE_UI_WAVE_X;
             x < BDA_LIVE_UI_WAVE_X + (s32)BDA_LIVE_UI_WAVE_COLUMNS;
             x += 2) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw, x, y, bda_live_ui_color_grid
            );
        }
    }
    for (x = BDA_LIVE_UI_WAVE_X;
         x < BDA_LIVE_UI_WAVE_X + (s32)BDA_LIVE_UI_WAVE_COLUMNS;
         x += BDA_LIVE_UI_WAVE_GRID_STEP) {
        for (y = BDA_LIVE_UI_WAVE_TOP;
             y <= BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT;
             y += 2) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw, x, y, bda_live_ui_color_grid
            );
        }
    }
    for (x = 12; x <= 227; ++x) {
        (void)bda_gui_put_pixel(
            bda_live_ui_draw, x, 214, bda_live_ui_color_grid
        );
        (void)bda_gui_put_pixel(
            bda_live_ui_draw, x, 246, bda_live_ui_color_grid
        );
    }
    bda_live_ui_outline(
        BDA_LIVE_UI_EXIT_X - 2,
        BDA_LIVE_UI_EXIT_Y - 2,
        BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH + 1,
        BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT + 1,
        bda_live_ui_color_exit
    );
    bda_live_ui_outline(
        BDA_LIVE_UI_EXIT_X,
        BDA_LIVE_UI_EXIT_Y,
        BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH - 1,
        BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT - 1,
        bda_live_ui_color_exit
    );
    for (y = BDA_LIVE_UI_EXIT_Y + 3;
         y < BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT - 2;
         y += 4) {
        for (x = BDA_LIVE_UI_EXIT_X + 3;
             x < BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH - 3;
             ++x) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw, x, y, button_fill
            );
        }
    }
    for (y = BDA_LIVE_UI_STATUS_DOT_Y;
         y < BDA_LIVE_UI_STATUS_DOT_Y + BDA_LIVE_UI_STATUS_DOT_SIZE;
         ++y) {
        for (x = BDA_LIVE_UI_RECORD_DOT_X;
             x < BDA_LIVE_UI_RECORD_DOT_X +
                 BDA_LIVE_UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw,
                x,
                y,
                bda_live_ui_color_record_off
            );
        }
        for (x = BDA_LIVE_UI_LINK_DOT_X;
             x < BDA_LIVE_UI_LINK_DOT_X +
                 BDA_LIVE_UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw,
                x,
                y,
                bda_live_ui_color_stream_off
            );
        }
    }
    (void)bda_gui_set_text_mode(bda_live_ui_draw, 1u);
    (void)bda_gui_set_text_color(bda_live_ui_draw, foreground);
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 47, 7, "BBK USB MICROPHONE", -1
    );
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 40, 29, "LIVE 16 KHZ / 16 BIT", -1
    );
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 31, 220, "RECORDING", -1
    );
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 150, 220, "PC LINK", -1
    );
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 76, 269, "STOP & EXIT", -1
    );
    (void)bda_gui_set_text_color(bda_live_ui_draw, muted);
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 18, 54, "LIVE WAVEFORM", -1
    );
    (void)bda_gui_draw_text(
        bda_live_ui_draw, 76, 302, "ESC TO EXIT", -1
    );
    (void)bda_gui_select_draw_object(bda_live_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    return 1;
}

static void bda_live_ui_draw_status_dot(
    s32 left, u32 color
) {
    void *old_object;
    s32 x;
    s32 y;
    if (!bda_live_ui_draw || !bda_live_ui_draw_object) {
        return;
    }
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(
        bda_live_ui_draw, bda_live_ui_draw_object
    );
    for (y = BDA_LIVE_UI_STATUS_DOT_Y;
         y < BDA_LIVE_UI_STATUS_DOT_Y + BDA_LIVE_UI_STATUS_DOT_SIZE;
         ++y) {
        for (x = left; x < left + BDA_LIVE_UI_STATUS_DOT_SIZE; ++x) {
            (void)bda_gui_put_pixel(bda_live_ui_draw, x, y, color);
        }
    }
    (void)bda_gui_select_draw_object(bda_live_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
}

static void bda_live_ui_set_recording(u32 recording) {
    recording = recording != 0u;
    if (recording == bda_live_ui_record_state) {
        return;
    }
    bda_live_ui_record_state = recording;
    bda_live_ui_draw_status_dot(
        BDA_LIVE_UI_RECORD_DOT_X,
        recording
            ? bda_live_ui_color_record_on
            : bda_live_ui_color_record_off
    );
}

static void bda_live_ui_set_streaming(u32 streaming) {
    streaming = streaming != 0u;
    if (streaming == bda_live_ui_stream_state) {
        return;
    }
    bda_live_ui_stream_state = streaming;
    bda_live_ui_draw_status_dot(
        BDA_LIVE_UI_LINK_DOT_X,
        streaming
            ? bda_live_ui_color_stream_on
            : bda_live_ui_color_stream_off
    );
}

static void bda_live_ui_update_exit_button(u32 pressed) {
    void *old_object;
    u32 color;
    if (!bda_live_ui_draw || !bda_live_ui_draw_object) {
        return;
    }
    color = pressed
        ? bda_live_ui_color_exit_pressed
        : bda_live_ui_color_exit;
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(
        bda_live_ui_draw, bda_live_ui_draw_object
    );
    bda_live_ui_outline(
        BDA_LIVE_UI_EXIT_X - 2,
        BDA_LIVE_UI_EXIT_Y - 2,
        BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH + 1,
        BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT + 1,
        color
    );
    bda_live_ui_outline(
        BDA_LIVE_UI_EXIT_X,
        BDA_LIVE_UI_EXIT_Y,
        BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH - 1,
        BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT - 1,
        color
    );
    (void)bda_gui_select_draw_object(bda_live_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
}

static void bda_live_ui_close(void);

static int bda_live_ui_open(void) {
    bda_frame_desc_t descriptor;
    bda_gui_input_packet_t packet;
    u32 i;
    u32 now;

    bda_memset(&descriptor, 0, sizeof(descriptor));
    bda_memset(&packet, 0, sizeof(packet));
    bda_live_ui_frame = 0;
    bda_live_ui_draw = 0;
    bda_live_ui_draw_owner = 0;
    bda_live_ui_draw_object = 0;
    bda_live_ui_initialized = 0u;
    bda_live_ui_detached = 0u;
    bda_live_ui_touch_button_down = 0u;
    bda_live_ui_exit = BDA_LIVE_UI_EXIT_NONE;
    bda_live_ui_record_state = 0u;
    bda_live_ui_stream_state = 0u;
    bda_live_ui_wave_generation = 0u;
    bda_live_ui_snapshot_generation = 0u;
    bda_live_ui_wave_column = 0u;
    for (i = 0u; i < BDA_LIVE_UI_WAVE_COLUMNS; ++i) {
        bda_live_ui_drawn_min_y[i] = BDA_LIVE_UI_WAVE_CENTER_Y;
        bda_live_ui_drawn_max_y[i] = BDA_LIVE_UI_WAVE_CENTER_Y;
    }

    descriptor.style = 0u;
    descriptor.title = "BBK USB Mic";
    descriptor.wndproc = bda_live_ui_window_proc;
    descriptor.height = BDA_LIVE_UI_SCREEN_WIDTH;
    descriptor.width = BDA_LIVE_UI_SCREEN_HEIGHT;
    descriptor.surface = 0u;
    bda_live_ui_frame = bda_gui_register_frame_desc(&descriptor);
    if (!bda_live_ui_frame || (s32)bda_live_ui_frame == -1) {
        bda_live_ui_frame = 0;
        return 0;
    }
    (void)bda_gui_frame_activate(bda_live_ui_frame, 0x100u);
    (void)bda_live_ui_acquire_draw(bda_live_ui_frame);
    if (!bda_live_ui_draw_object) {
        bda_live_ui_draw_object = bda_gui_draw_object_create(7u);
    }
    if (!bda_live_ui_draw ||
        !bda_live_ui_draw_object ||
        (s32)(u32)bda_live_ui_draw_object == -1 ||
        !bda_live_ui_present_initial()) {
        bda_live_ui_close();
        return 0;
    }
    (void)bda_gui_input_packet(&packet);
    bda_live_ui_previous_escape =
        (u32)bda_gui_input_packet_key_pressed(&packet, BDA_KEY_ESCAPE);
    now = bda_gui_tick_count_25ms_like();
    bda_live_ui_last_tick = now;
    bda_live_ui_input_arm_tick = now + BDA_LIVE_UI_INPUT_ARM_TICKS;
    bda_live_ui_initialized = 1u;
    return 1;
}

static void bda_live_ui_close(void) {
    bda_gui_message_t message;
    u32 i;
    if (bda_live_ui_frame) {
        bda_memset(&message, 0, sizeof(message));
        (void)bda_gui_frame_stop(bda_live_ui_frame);
        (void)bda_gui_frame_release(bda_live_ui_frame);
        for (i = 0u; i < 128u && !bda_live_ui_detached; ++i) {
            if (!bda_gui_event_pump_frame_once(
                    &message, bda_live_ui_frame)) {
                break;
            }
            bda_live_ui_tiny_delay();
        }
        bda_live_ui_release_draw();
        bda_gui_close_frame(bda_live_ui_frame);
        bda_live_ui_frame = 0;
    } else {
        bda_live_ui_release_draw();
    }
    bda_live_ui_initialized = 0u;
}

static void bda_live_ui_analyze_pcm(const void *pcm) {
    const s16 *samples = (const s16 *)pcm;
    u32 column;
    for (column = 0u; column < BDA_LIVE_UI_WAVE_COLUMNS; ++column) {
        u32 base = column * BDA_LIVE_UI_WAVE_SAMPLES_COLUMN;
        u32 i;
        s32 sum = 0;
        s16 minimum = 32767;
        s16 maximum = (s16)-32768;
        for (i = 0u; i < BDA_LIVE_UI_WAVE_SAMPLES_COLUMN; ++i) {
            s16 sample = samples[base + i];
            if (sample < minimum) {
                minimum = sample;
            }
            if (sample > maximum) {
                maximum = sample;
            }
            sum += sample;
        }
        bda_live_ui_wave_min[column] = minimum;
        bda_live_ui_wave_max[column] = maximum;
        bda_live_ui_wave_mean[column] =
            (s16)(sum / (s32)BDA_LIVE_UI_WAVE_SAMPLES_COLUMN);
    }
    ++bda_live_ui_wave_generation;
}

static s32 bda_live_ui_wave_y(s16 sample) {
    s32 scaled =
        ((s32)sample * BDA_LIVE_UI_WAVE_HALF_HEIGHT *
         BDA_LIVE_UI_WAVE_GAIN) / 32768;
    s32 y = BDA_LIVE_UI_WAVE_CENTER_Y - scaled;
    if (y < BDA_LIVE_UI_WAVE_TOP) {
        y = BDA_LIVE_UI_WAVE_TOP;
    }
    if (y > BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT) {
        y = BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT;
    }
    return y;
}

static int bda_live_ui_grid_row(s32 y) {
    return y == BDA_LIVE_UI_WAVE_CENTER_Y ||
        y == BDA_LIVE_UI_WAVE_TOP ||
        y == BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT ||
        y == BDA_LIVE_UI_WAVE_CENTER_Y - BDA_LIVE_UI_WAVE_HALF_HEIGHT / 2 ||
        y == BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT / 2;
}

static u32 bda_live_ui_background_color(s32 x, s32 y) {
    s32 relative = x - BDA_LIVE_UI_WAVE_X;
    int grid_column =
        relative >= 0 &&
        relative < (s32)BDA_LIVE_UI_WAVE_COLUMNS &&
        relative % BDA_LIVE_UI_WAVE_GRID_STEP == 0;
    if ((bda_live_ui_grid_row(y) && (relative & 1) == 0) ||
        (grid_column && ((y - BDA_LIVE_UI_WAVE_TOP) & 1) == 0)) {
        return bda_live_ui_color_grid;
    }
    return bda_live_ui_color_panel;
}

static void bda_live_ui_snapshot_wave(void) {
    u32 i;
    for (i = 0u; i < BDA_LIVE_UI_WAVE_COLUMNS; ++i) {
        bda_live_ui_snapshot_min[i] = bda_live_ui_wave_min[i];
        bda_live_ui_snapshot_max[i] = bda_live_ui_wave_max[i];
        bda_live_ui_snapshot_mean[i] = bda_live_ui_wave_mean[i];
    }
    bda_live_ui_snapshot_generation = bda_live_ui_wave_generation;
}

static void bda_live_ui_draw_wave_column(void) {
    void *old_object;
    u32 column;
    s32 x;
    s32 min_y;
    s32 max_y;
    s32 mean_y;
    s32 old_top;
    s32 old_bottom;
    s32 new_top;
    s32 new_bottom;
    s32 y;

    if (!bda_live_ui_draw ||
        !bda_live_ui_draw_object ||
        bda_live_ui_wave_generation == 0u) {
        return;
    }
    if (bda_live_ui_snapshot_generation != bda_live_ui_wave_generation) {
        bda_live_ui_snapshot_wave();
    }
    column = bda_live_ui_wave_column;
    x = BDA_LIVE_UI_WAVE_X + (s32)column;
    min_y = bda_live_ui_wave_y(bda_live_ui_snapshot_min[column]);
    max_y = bda_live_ui_wave_y(bda_live_ui_snapshot_max[column]);
    mean_y = bda_live_ui_wave_y(bda_live_ui_snapshot_mean[column]);

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(
        bda_live_ui_draw, bda_live_ui_draw_object
    );
    old_top = (s32)bda_live_ui_drawn_max_y[column] - 1;
    old_bottom = (s32)bda_live_ui_drawn_min_y[column] + 1;
    if (old_top < BDA_LIVE_UI_WAVE_TOP) {
        old_top = BDA_LIVE_UI_WAVE_TOP;
    }
    if (old_bottom >
        BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT) {
        old_bottom =
            BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT;
    }
    for (y = old_top; y <= old_bottom; ++y) {
        (void)bda_gui_put_pixel(
            bda_live_ui_draw,
            x,
            y,
            bda_live_ui_background_color(x, y)
        );
    }

    new_top = max_y;
    new_bottom = min_y;
    if (new_top > new_bottom) {
        s32 swap = new_top;
        new_top = new_bottom;
        new_bottom = swap;
    }
    if (new_bottom - new_top < 2) {
        new_top = mean_y - 1;
        new_bottom = mean_y + 1;
    }
    if (new_top < BDA_LIVE_UI_WAVE_TOP) {
        new_top = BDA_LIVE_UI_WAVE_TOP;
    }
    if (new_bottom >
        BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT) {
        new_bottom =
            BDA_LIVE_UI_WAVE_CENTER_Y + BDA_LIVE_UI_WAVE_HALF_HEIGHT;
    }
    for (y = new_top; y <= new_bottom; ++y) {
        (void)bda_gui_put_pixel(
            bda_live_ui_draw, x, y, bda_live_ui_color_wave
        );
    }
    for (y = mean_y - 1; y <= mean_y + 1; ++y) {
        if (y >= BDA_LIVE_UI_WAVE_TOP &&
            y <= BDA_LIVE_UI_WAVE_CENTER_Y +
                BDA_LIVE_UI_WAVE_HALF_HEIGHT) {
            (void)bda_gui_put_pixel(
                bda_live_ui_draw, x, y, bda_live_ui_color_mean
            );
        }
    }
    (void)bda_gui_select_draw_object(bda_live_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    bda_live_ui_drawn_min_y[column] = (u16)min_y;
    bda_live_ui_drawn_max_y[column] = (u16)max_y;
    ++bda_live_ui_wave_column;
    if (bda_live_ui_wave_column >= BDA_LIVE_UI_WAVE_COLUMNS) {
        bda_live_ui_wave_column = 0u;
    }
}

static int bda_live_ui_exit_hit(u16 x, u16 y) {
    return x >= BDA_LIVE_UI_EXIT_X &&
        x < BDA_LIVE_UI_EXIT_X + BDA_LIVE_UI_EXIT_WIDTH &&
        y >= BDA_LIVE_UI_EXIT_Y &&
        y < BDA_LIVE_UI_EXIT_Y + BDA_LIVE_UI_EXIT_HEIGHT;
}

static void bda_live_ui_poll_input(u32 now) {
    bda_gui_input_packet_t packet;
    bda_gui_raw_event_t event;
    u32 current_escape;
    u32 armed = (s32)(now - bda_live_ui_input_arm_tick) >= 0;
    u32 drain;

    bda_memset(&packet, 0, sizeof(packet));
    (void)bda_gui_input_packet(&packet);
    current_escape = (u32)bda_gui_input_packet_key_pressed(
        &packet, BDA_KEY_ESCAPE
    );
    if (armed && current_escape && !bda_live_ui_previous_escape) {
        bda_live_ui_exit = BDA_LIVE_UI_EXIT_ESCAPE;
    }
    bda_live_ui_previous_escape = current_escape;

    for (drain = 0u; drain < BDA_LIVE_UI_RAW_EVENT_DRAIN; ++drain) {
        u16 x = 0xffffu;
        u16 y = 0xffffu;
        event.code = -1;
        event.value = -1;
        (void)bda_gui_raw_event_fetch(&event);
        if (event.code < 0) {
            break;
        }
        if (event.code != BDA_INPUT_EVENT_TOUCH_DOWN &&
            event.code != BDA_INPUT_EVENT_TOUCH_UP) {
            continue;
        }
        bda_gui_touch_position(&x, &y);
        if (!armed) {
            bda_live_ui_touch_button_down = 0u;
            continue;
        }
        if (event.code == BDA_INPUT_EVENT_TOUCH_DOWN) {
            bda_live_ui_touch_button_down =
                (u32)bda_live_ui_exit_hit(x, y);
            if (bda_live_ui_touch_button_down) {
                bda_live_ui_update_exit_button(1u);
            }
        } else {
            u32 was_down = bda_live_ui_touch_button_down;
            if (was_down && bda_live_ui_exit_hit(x, y)) {
                bda_live_ui_exit = BDA_LIVE_UI_EXIT_BUTTON;
            }
            bda_live_ui_touch_button_down = 0u;
            if (was_down) {
                bda_live_ui_update_exit_button(0u);
            }
        }
    }
}

static void bda_live_ui_service(void) {
    u32 now;
    if (!bda_live_ui_initialized ||
        bda_live_ui_exit != BDA_LIVE_UI_EXIT_NONE) {
        return;
    }
    now = bda_gui_tick_count_25ms_like();
    if (now == bda_live_ui_last_tick) {
        return;
    }
    bda_live_ui_last_tick = now;
    bda_live_ui_poll_input(now);
    if (bda_live_ui_exit == BDA_LIVE_UI_EXIT_NONE) {
        bda_live_ui_draw_wave_column();
    }
}

static void bda_live_ui_arm_input(void) {
    u32 now = bda_gui_tick_count_25ms_like();
    bda_live_ui_last_tick = now;
    bda_live_ui_input_arm_tick = now + BDA_LIVE_UI_INPUT_ARM_TICKS;
    bda_live_ui_touch_button_down = 0u;
}

static u32 bda_live_ui_exit_reason(void) {
    return bda_live_ui_exit;
}

#endif
