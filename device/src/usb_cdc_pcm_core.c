/* SPDX-License-Identifier: Apache-2.0 */

#include "../firmware_abi.h"

/*
 * Project copy of the probe-derived device implementation.
 *
 * New CDC PCM development should happen in this directory. Legacy UAC/HID
 * compile-time branches are retained for provenance and will be removed as
 * the firmware ABI, UDC, capture, UI, and transport modules are extracted.
 *
 * UsbKnlP4-P28/Sync1/Pch1/Act1-Act14: JZ4730/C200knl UAC1, PCM, UI, and recovery probes.
 *
 * Most variants own only the polling path and keep IRQ12 masked. Act14
 * temporarily registers a minimal IRQ12 handler through the verified C200knl
 * IRQ ABI so JZ4730 SC/SI can be acknowledged within the control-transfer
 * deadline. No filesystem call is made while the device is connected.
 */

#ifndef USB_MIC_SILENCE_PROBE
#define USB_MIC_SILENCE_PROBE 0
#endif

#ifndef USB_MIC_IAD_PROBE
#define USB_MIC_IAD_PROBE 0
#endif

#ifndef USB_MIC_EP4_PROBE
#define USB_MIC_EP4_PROBE 0
#endif

#ifndef USB_MIC_P8_PROBE
#define USB_MIC_P8_PROBE 0
#endif

#ifndef USB_MIC_P9_PROBE
#define USB_MIC_P9_PROBE 0
#endif

#ifndef USB_MIC_P10_PROBE
#define USB_MIC_P10_PROBE 0
#endif

#ifndef USB_MIC_P11_PROBE
#define USB_MIC_P11_PROBE 0
#endif

#ifndef USB_MIC_P12_PROBE
#define USB_MIC_P12_PROBE 0
#endif

#ifndef USB_MIC_P13_PROBE
#define USB_MIC_P13_PROBE 0
#endif

#ifndef USB_MIC_P14_PROBE
#define USB_MIC_P14_PROBE 0
#endif

#ifndef USB_MIC_P15_PROBE
#define USB_MIC_P15_PROBE 0
#endif

#ifndef USB_MIC_P16_PROBE
#define USB_MIC_P16_PROBE 0
#endif

#ifndef USB_MIC_P17_PROBE
#define USB_MIC_P17_PROBE 0
#endif

#ifndef USB_MIC_P18_PROBE
#define USB_MIC_P18_PROBE 0
#endif

#ifndef USB_MIC_P19_PROBE
#define USB_MIC_P19_PROBE 0
#endif

#ifndef USB_MIC_P20_PROBE
#define USB_MIC_P20_PROBE 0
#endif

#ifndef USB_MIC_P21_PROBE
#define USB_MIC_P21_PROBE 0
#endif

#ifndef USB_MIC_P22_PROBE
#define USB_MIC_P22_PROBE 0
#endif

#ifndef USB_MIC_P23_PROBE
#define USB_MIC_P23_PROBE 0
#endif

#ifndef USB_MIC_P24_PROBE
#define USB_MIC_P24_PROBE 0
#endif

#ifndef USB_MIC_P25_PROBE
#define USB_MIC_P25_PROBE 0
#endif

#ifndef USB_MIC_P26_PROBE
#define USB_MIC_P26_PROBE 0
#endif

#ifndef USB_MIC_P27_PROBE
#define USB_MIC_P27_PROBE 0
#endif

#ifndef USB_MIC_P28_PROBE
#define USB_MIC_P28_PROBE 0
#endif

#ifndef USB_MIC_SYNC1_PROBE
#define USB_MIC_SYNC1_PROBE 0
#endif

#ifndef USB_MIC_PCH1_PROBE
#define USB_MIC_PCH1_PROBE 0
#endif

#ifndef USB_MIC_ACT1_PROBE
#define USB_MIC_ACT1_PROBE 0
#endif

#ifndef USB_MIC_ACT2_PROBE
#define USB_MIC_ACT2_PROBE 0
#endif

#ifndef USB_MIC_ACT3_PROBE
#define USB_MIC_ACT3_PROBE 0
#endif

#ifndef USB_MIC_ACT4_PROBE
#define USB_MIC_ACT4_PROBE 0
#endif

#ifndef USB_MIC_ACT5_PROBE
#define USB_MIC_ACT5_PROBE 0
#endif

#ifndef USB_MIC_ACT6_PROBE
#define USB_MIC_ACT6_PROBE 0
#endif

#ifndef USB_MIC_ACT7_PROBE
#define USB_MIC_ACT7_PROBE 0
#endif

#ifndef USB_MIC_ACT8_PROBE
#define USB_MIC_ACT8_PROBE 0
#endif

#ifndef USB_MIC_ACT9_PROBE
#define USB_MIC_ACT9_PROBE 0
#endif

#ifndef USB_MIC_ACT10_PROBE
#define USB_MIC_ACT10_PROBE 0
#endif

#ifndef USB_MIC_ACT11_PROBE
#define USB_MIC_ACT11_PROBE 0
#endif

#ifndef USB_MIC_ACT12_PROBE
#define USB_MIC_ACT12_PROBE 0
#endif

#ifndef USB_MIC_ACT13_PROBE
#define USB_MIC_ACT13_PROBE 0
#endif

#ifndef USB_MIC_ACT14_PROBE
#define USB_MIC_ACT14_PROBE 0
#endif

#ifndef USB_MIC_ACT15_PROBE
#define USB_MIC_ACT15_PROBE 0
#endif

#ifndef USB_MIC_ACT16_PROBE
#define USB_MIC_ACT16_PROBE 0
#endif

#ifndef USB_MIC_ACT17_PROBE
#define USB_MIC_ACT17_PROBE 0
#endif

#ifndef USB_MIC_ACT18_PROBE
#define USB_MIC_ACT18_PROBE 0
#endif

#ifndef USB_PCM_HID_PROBE
#define USB_PCM_HID_PROBE 0
#endif

#ifndef USB_PCM_CDC_PROBE
#define USB_PCM_CDC_PROBE 0
#endif

#ifndef USB_PCM_CDC_C2_PROBE
#define USB_PCM_CDC_C2_PROBE 0
#endif

#ifndef USB_PCM_CDC_C3_PROBE
#define USB_PCM_CDC_C3_PROBE 0
#endif

#ifndef USB_PCM_RELEASE_UI
#define USB_PCM_RELEASE_UI 0
#endif

#define USB_PCM_CDC_EP0_QUEUE_PROBE \
    (USB_PCM_CDC_C2_PROBE || USB_PCM_CDC_C3_PROBE)
#define USB_PCM_TRANSPORT_PROBE \
    (USB_PCM_HID_PROBE || USB_PCM_CDC_PROBE)
#define USB_MIC_INTERRUPT_TIMED_PROBE \
    (USB_PCM_TRANSPORT_PROBE || \
     USB_MIC_ACT14_PROBE || USB_MIC_ACT15_PROBE || \
     USB_MIC_ACT16_PROBE || USB_MIC_ACT17_PROBE || USB_MIC_ACT18_PROBE)
#define USB_MIC_EP0_IRQ_PROBE \
    (USB_PCM_TRANSPORT_PROBE || \
     USB_MIC_ACT15_PROBE || USB_MIC_ACT16_PROBE || \
     USB_MIC_ACT17_PROBE || USB_MIC_ACT18_PROBE)

#define USB_MIC_STATIC_CSR_ACT_PROBE \
    (USB_MIC_ACT7_PROBE || USB_MIC_ACT8_PROBE || USB_MIC_ACT12_PROBE)
#define USB_MIC_ACTIVATION_PROBE \
    (USB_MIC_ACT1_PROBE || USB_MIC_ACT2_PROBE || \
     USB_MIC_ACT3_PROBE || USB_MIC_ACT4_PROBE || USB_MIC_ACT5_PROBE || \
     USB_MIC_ACT7_PROBE || USB_MIC_ACT8_PROBE || USB_MIC_ACT9_PROBE || \
     USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE)
#define USB_MIC_ORDERED_SI_ACT_PROBE \
    (USB_MIC_ACT5_PROBE || USB_MIC_ACT9_PROBE || USB_MIC_ACT11_PROBE)
#define USB_MIC_PROVEN_IN_PROBE \
    ((USB_MIC_P21_PROBE || USB_MIC_P22_PROBE || \
     USB_MIC_P23_PROBE || USB_MIC_P24_PROBE || USB_MIC_P25_PROBE || \
     USB_MIC_P26_PROBE || USB_MIC_P27_PROBE || USB_MIC_P28_PROBE || \
     USB_MIC_SYNC1_PROBE || USB_MIC_PCH1_PROBE) || \
     USB_MIC_ACTIVATION_PROBE)
#define USB_MIC_DYNAMIC_CSR_PROBE \
    (USB_MIC_P22_PROBE || USB_MIC_P23_PROBE || \
     USB_MIC_P24_PROBE || USB_MIC_P25_PROBE || USB_MIC_PCH1_PROBE || \
     USB_MIC_ACT5_PROBE || USB_MIC_ACT9_PROBE || USB_MIC_ACT11_PROBE)
#define USB_MIC_SERVICE_CSR_PROBE \
    ((USB_MIC_DYNAMIC_CSR_PROBE || USB_MIC_P27_PROBE || USB_MIC_P28_PROBE || \
     USB_MIC_SYNC1_PROBE) || USB_MIC_ACTIVATION_PROBE)
#define USB_MIC_SOFT_RESET_CSR_PROBE \
    (USB_MIC_P23_PROBE || USB_MIC_P24_PROBE || USB_MIC_P25_PROBE || \
     USB_MIC_PCH1_PROBE)
#define USB_MIC_POST_IRQ_STATUS_PROBE \
    ((USB_MIC_P28_PROBE || USB_MIC_SYNC1_PROBE || USB_MIC_PCH1_PROBE) || \
     USB_MIC_ACTIVATION_PROBE || USB_MIC_ACT10_PROBE)

#if USB_MIC_P14_PROBE
#include "bda_firmware_audio.h"
#endif
#if USB_MIC_P18_PROBE
#include "bda_graphics.h"
#include "bda_input.h"
#include "bda_window.h"
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define REG8(addr)  (*(volatile u8 *)(addr))
#define REG32(addr) (*(volatile u32 *)(addr))

#define APP_DATA_DIR "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd"

#if USB_PCM_CDC_C3_PROBE
#if USB_PCM_RELEASE_UI
#define PROBE_TITLE          "BBK USB Mic"
#define PROBE_PREFIX         "[UsbMic] "
#define PROBE_LOG_PATH       APP_DATA_DIR "\\9588usbmic.log"
#define PROBE_BEGIN_TEXT     "session_start"
#define PROBE_ENTRY_TEXT     ""
#define PROBE_CONFIRM_TEXT   ""
#define PROBE_COMPLETE_TEXT  \
    "USB storage is unavailable until restart. Restart the device before reconnecting USB."
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x56
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#else
#define PROBE_TITLE          "UsbCdcP3"
#define PROBE_PREFIX         "[UsbCdcP3] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbcdcp3.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 CDC IAD and config fallback PCM probe"
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x56
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "CDC C3 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts CDC C3; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "CDC C3 UDC disconnected. Reboot before reconnecting USB."
#endif
#elif USB_PCM_CDC_C2_PROBE
#define PROBE_TITLE          "UsbCdcP2"
#define PROBE_PREFIX         "[UsbCdcP2] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbcdcp2.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 CDC EP0 queue fix PCM probe"
#define PROBE_PRODUCT_DIGIT  '2'
#define PROBE_PID_LOW        0x55
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "CDC C2 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts CDC C2; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "CDC C2 UDC disconnected. Reboot before reconnecting USB."
#elif USB_PCM_CDC_PROBE
#define PROBE_TITLE          "UsbCdcP1"
#define PROBE_PREFIX         "[UsbCdcP1] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbcdcp1.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 CDC ACM bulk PCM transport probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x54
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "CDC PCM entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts CDC PCM; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "CDC UDC disconnected. Reboot before reconnecting USB."
#elif USB_PCM_HID_PROBE
#define PROBE_TITLE          "UsbHidP1"
#define PROBE_PREFIX         "[UsbHidP1] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbhidp1.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 HID interrupt PCM transport probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x53
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "HID PCM entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts HID PCM; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "HID UDC disconnected. Reboot before reconnecting USB."
#elif USB_MIC_ACT18_PROBE
#define PROBE_TITLE          "UsbAct18"
#define PROBE_PREFIX         "[UsbAct18] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica18.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 post-W1C NE plus CSR_DONE EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '8'
#define PROBE_PID_LOW        0x52
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act18 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act18; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act18_" name
#define STATIC_ACT_LOG_LABEL(name) "act18_" name
#elif USB_MIC_ACT17_PROBE
#define PROBE_TITLE          "UsbAct17"
#define PROBE_PREFIX         "[UsbAct17] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica17.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 pre-W1C SI match EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '7'
#define PROBE_PID_LOW        0x51
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act17 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act17; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act17_" name
#define STATIC_ACT_LOG_LABEL(name) "act17_" name
#elif USB_MIC_ACT16_PROBE
#define PROBE_TITLE          "UsbAct16"
#define PROBE_PREFIX         "[UsbAct16] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica16.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 IRQ CSR_DONE EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '6'
#define PROBE_PID_LOW        0x50
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act16 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act16; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act16_" name
#define STATIC_ACT_LOG_LABEL(name) "act16_" name
#elif USB_MIC_ACT15_PROBE
#define PROBE_TITLE          "UsbAct15"
#define PROBE_PREFIX         "[UsbAct15] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica15.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 IRQ EP0 and EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '5'
#define PROBE_PID_LOW        0x4F
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act15 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act15; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act15_" name
#define STATIC_ACT_LOG_LABEL(name) "act15_" name
#elif USB_MIC_ACT14_PROBE
#define PROBE_TITLE          "UsbAct14"
#define PROBE_PREFIX         "[UsbAct14] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica14.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 interrupt-timed SI EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '4'
#define PROBE_PID_LOW        0x4E
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act14 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act14; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act14_" name
#define STATIC_ACT_LOG_LABEL(name) "act14_" name
#elif USB_MIC_ACT13_PROBE
#define PROBE_TITLE          "UsbAct13"
#define PROBE_PREFIX         "[UsbAct13] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica13.log"
#define PROBE_BEGIN_TEXT     "begin JZ4730 masked-SI EP4 PCM probe"
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x4D
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act13 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act13; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#elif USB_MIC_ACT12_PROBE
#define PROBE_TITLE          "UsbAct12"
#define PROBE_PREFIX         "[UsbAct12] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica12.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 EP4 static auto-switch probe"
#define PROBE_PRODUCT_DIGIT  '2'
#define PROBE_PID_LOW        0x4C
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act12 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act12; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act12_" name
#define STATIC_ACT_LOG_LABEL(name) "act12_" name
#elif USB_MIC_ACT11_PROBE
#define PROBE_TITLE          "UsbAct11"
#define PROBE_PREFIX         "[UsbAct11] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica11.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 EP4 post-CSR status-stage probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x4B
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act11 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act11; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#define ACTIVATION_LOG_LABEL(name) "act11_" name
#elif USB_MIC_ACT10_PROBE
#define PROBE_TITLE          "UsbAct10"
#define PROBE_PREFIX         "[UsbAct10] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica10.log"
#define PROBE_BEGIN_TEXT     "begin static default-alt 16 kHz integrated probe"
#define PROBE_PRODUCT_DIGIT  '0'
#define PROBE_PID_LOW        0x4A
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act10 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act10; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "UDC disconnected. Reboot before reconnecting USB."
#elif USB_MIC_ACT9_PROBE
#define PROBE_TITLE          "UsbAct9"
#define PROBE_PREFIX         "[UsbAct9] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica9.log"
#define PROBE_BEGIN_TEXT     "begin DEVCFG hard-reset dynamic CSR probe"
#define PROBE_PRODUCT_DIGIT  '9'
#define PROBE_PID_LOW        0x49
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act9 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act9; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act9_" name
#elif USB_MIC_ACT8_PROBE
#define PROBE_TITLE          "UsbAct8"
#define PROBE_PREFIX         "[UsbAct8] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica8.log"
#define PROBE_BEGIN_TEXT     "begin correct EP2 IN CSR slot probe"
#define PROBE_PRODUCT_DIGIT  '8'
#define PROBE_PID_LOW        0x48
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act8 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act8; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act8_" name
#define STATIC_ACT_LOG_LABEL(name) "act8_" name
#elif USB_MIC_ACT7_PROBE
#define PROBE_TITLE          "UsbAct7"
#define PROBE_PREFIX         "[UsbAct7] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica7.log"
#define PROBE_BEGIN_TEXT     "begin static CSR automatic SET_INTERFACE probe"
#define PROBE_PRODUCT_DIGIT  '7'
#define PROBE_PID_LOW        0x47
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act7 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act7; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act7_" name
#define STATIC_ACT_LOG_LABEL(name) "act7_" name
#elif USB_MIC_ACT6_PROBE
#define PROBE_TITLE          "UsbAct6"
#define PROBE_PREFIX         "[UsbAct6] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica6.log"
#define PROBE_BEGIN_TEXT     "begin deferred EP2 SET_INTERFACE status probe"
#define PROBE_PRODUCT_DIGIT  '6'
#define PROBE_PID_LOW        0x46
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act6 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act6; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act6_" name
#elif USB_MIC_ACT5_PROBE
#define PROBE_TITLE          "UsbAct5"
#define PROBE_PREFIX         "[UsbAct5] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica5.log"
#define PROBE_BEGIN_TEXT     "begin ordered PCH SET_INTERFACE handshake probe"
#define PROBE_PRODUCT_DIGIT  '5'
#define PROBE_PID_LOW        0x45
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act5 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act5; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act5_" name
#elif USB_MIC_ACT4_PROBE
#define PROBE_TITLE          "UsbAct4"
#define PROBE_PREFIX         "[UsbAct4] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica4.log"
#define PROBE_BEGIN_TEXT     "begin inferred-alt PCH SET_INTERFACE probe"
#define PROBE_PRODUCT_DIGIT  '4'
#define PROBE_PID_LOW        0x44
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act4 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act4; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act4_" name
#elif USB_MIC_ACT3_PROBE
#define PROBE_TITLE          "UsbAct3"
#define PROBE_PREFIX         "[UsbAct3] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica3.log"
#define PROBE_BEGIN_TEXT     "begin 34-byte product short-packet activation probe"
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x43
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act3 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act3; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act3_" name
#elif USB_MIC_ACT2_PROBE
#define PROBE_TITLE          "UsbAct2"
#define PROBE_PREFIX         "[UsbAct2] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica2.log"
#define PROBE_BEGIN_TEXT     "begin fixed-alt1 EP2 runtime activation probe"
#define PROBE_PRODUCT_DIGIT  '2'
#define PROBE_PID_LOW        0x42
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act2 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act2; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Wait for this dialog, then reconnect USB to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act2_" name
#elif USB_MIC_ACT1_PROBE
#define PROBE_TITLE          "UsbAct1"
#define PROBE_PREFIX         "[UsbAct1] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmica1.log"
#define PROBE_BEGIN_TEXT     "begin Synopsys EP2 runtime activation probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x41
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Act1 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Act1; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#define ACTIVATION_LOG_LABEL(name) "act1_" name
#elif USB_MIC_PCH1_PROBE
#define PROBE_TITLE          "UsbPch1"
#define PROBE_PREFIX         "[UsbPch1] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicpch1.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 programmed PCH synchronous probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x40
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Pch1 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Pch1; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_SYNC1_PROBE
#define PROBE_TITLE          "UsbSync1"
#define PROBE_PREFIX         "[UsbSync1] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicsync1.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 PCH synchronous endpoint probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x3F
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "Sync1 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts Sync1; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P28_PROBE
#define PROBE_TITLE          "UsbKnl28"
#define PROBE_PREFIX         "[UsbKnl28] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp28.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 post-IRQ status activation probe"
#define PROBE_PRODUCT_DIGIT  '8'
#define PROBE_PID_LOW        0x3E
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P28 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P28; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P27_PROBE
#define PROBE_TITLE          "UsbKnl27"
#define PROBE_PREFIX         "[UsbKnl27] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp27.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 direct WASAPI PCH PIO probe"
#define PROBE_PRODUCT_DIGIT  '7'
#define PROBE_PID_LOW        0x3D
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P27 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P27; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P26_PROBE
#define PROBE_TITLE          "UsbKnl26"
#define PROBE_PREFIX         "[UsbKnl26] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp26.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 default-alt PCH PIO probe"
#define PROBE_PRODUCT_DIGIT  '6'
#define PROBE_PID_LOW        0x3C
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P26 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P26; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P25_PROBE
#define PROBE_TITLE          "UsbKnl25"
#define PROBE_PREFIX         "[UsbKnl25] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp25.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 Synopsys EP2 mapping probe"
#define PROBE_PRODUCT_DIGIT  '5'
#define PROBE_PID_LOW        0x3B
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P25 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P25; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P24_PROBE
#define PROBE_TITLE          "UsbKnl24"
#define PROBE_PREFIX         "[UsbKnl24] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp24.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 static CSR index 4 probe"
#define PROBE_PRODUCT_DIGIT  '4'
#define PROBE_PID_LOW        0x3A
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P24 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P24; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P23_PROBE
#define PROBE_TITLE          "UsbKnl23"
#define PROBE_PREFIX         "[UsbKnl23] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp23.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 soft-reset dynamic CSR probe"
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x39
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P23 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P23; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P22_PROBE
#define PROBE_TITLE          "UsbKnl22"
#define PROBE_PREFIX         "[UsbKnl22] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp22.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 dynamic CSR activation probe"
#define PROBE_PRODUCT_DIGIT  '2'
#define PROBE_PID_LOW        0x38
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P22 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P22; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P21_PROBE
#define PROBE_TITLE          "UsbKnl21"
#define PROBE_PREFIX         "[UsbKnl21] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp21.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 proven IN engine probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x37
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P21 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P21; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P20_PROBE
#define PROBE_TITLE          "UsbKnl20"
#define PROBE_PREFIX         "[UsbKnl20] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp20.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 8 kHz host negotiation probe"
#define PROBE_PRODUCT_DIGIT  '0'
#define PROBE_PID_LOW        0x36
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P20 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P20; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P19_PROBE
#define PROBE_TITLE          "UsbKnl19"
#define PROBE_PREFIX         "[UsbKnl19] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp19.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 conservative live UI probe"
#define PROBE_PRODUCT_DIGIT  '9'
#define PROBE_PID_LOW        0x35
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P19 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P19; reconnect PC after the live screen appears."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P18_PROBE
#define PROBE_TITLE          "UsbKnl18"
#define PROBE_PREFIX         "[UsbKnl18] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp18.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 live waveform and interactive exit probe"
#define PROBE_PRODUCT_DIGIT  '8'
#define PROBE_PID_LOW        0x34
#define PROBE_RUN_TICKS      (4u * 60u * 60u * 40u)
#define PROBE_ENTRY_TEXT     "P18 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P18; reconnect PC, then use ESC or the on-screen EXIT button to stop."
#define PROBE_COMPLETE_TEXT  "System USB restored. Unplug and reconnect USB once to mount storage."
#elif USB_MIC_P17_PROBE
#define PROBE_TITLE          "UsbKnl17"
#define PROBE_PREFIX         "[UsbKnl17] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp17.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 DirectShow and EP4 FIFO fix probe"
#define PROBE_PRODUCT_DIGIT  '7'
#define PROBE_PID_LOW        0x33
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P17 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P17; reconnect PC and run host capture after this dialog closes. USB restores after 90 seconds."
#define PROBE_COMPLETE_TEXT  "System USB restored. Exit P17, then unplug and reconnect USB once to mount storage."
#elif USB_MIC_P16_PROBE
#define PROBE_TITLE          "UsbKnl16"
#define PROBE_PREFIX         "[UsbKnl16] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp16.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 hardware service ack probe"
#define PROBE_PRODUCT_DIGIT  '6'
#define PROBE_PID_LOW        0x32
#define PROBE_RUN_TICKS      (60u * 40u)
#define PROBE_ENTRY_TEXT     "P16 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P16; reconnect PC and run host capture after this dialog closes. USB restores after 60 seconds."
#define PROBE_COMPLETE_TEXT  "System USB restored. Exit P16, then unplug and reconnect USB once to mount storage."
#elif USB_MIC_P15_PROBE
#define PROBE_TITLE          "UsbKnl15"
#define PROBE_PREFIX         "[UsbKnl15] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp15.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 SDK PCM recovery probe"
#define PROBE_PRODUCT_DIGIT  '5'
#define PROBE_PID_LOW        0x31
#define PROBE_RUN_TICKS      (60u * 40u)
#define PROBE_ENTRY_TEXT     "P15 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "USB disconnected. YES starts P15; reconnect PC and run host capture after this dialog closes. USB restores after 60 seconds."
#define PROBE_COMPLETE_TEXT  "System USB restored. Exit P15, then unplug and reconnect USB once to mount storage."
#elif USB_MIC_P14_PROBE
#define PROBE_TITLE          "UsbKnl14"
#define PROBE_PREFIX         "[UsbKnl14] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp14.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 SDK PCM capture probe"
#define PROBE_PRODUCT_DIGIT  '4'
#define PROBE_PID_LOW        0x30
#define PROBE_RUN_TICKS      (180u * 40u)
#define PROBE_ENTRY_TEXT     "P14 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Disconnect PC USB, then YES. After this dialog closes, reconnect USB and run the host capture script."
#define PROBE_COMPLETE_TEXT  "P14 complete; reboot, then send debug\\usbmicp14.log and the captured WAV"
#elif USB_MIC_P13_PROBE
#define PROBE_TITLE          "UsbKnl13"
#define PROBE_PREFIX         "[UsbKnl13] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp13.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 hardware endpoint map probe"
#define PROBE_PRODUCT_DIGIT  '3'
#define PROBE_PID_LOW        0x2F
#define PROBE_RUN_TICKS      (180u * 40u)
#define PROBE_ENTRY_TEXT     "P13 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Start the host capture script, connect PC USB, then YES. P13 stays online for 180 seconds."
#define PROBE_COMPLETE_TEXT  "P13 complete; reboot, then send debug\\usbmicp13.log and the host capture result"
#elif USB_MIC_P12_PROBE
#define PROBE_TITLE          "UsbKnl12"
#define PROBE_PREFIX         "[UsbKnl12] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp12.log"
#define PROBE_BEGIN_TEXT     "begin Windows UAC1 topology probe"
#define PROBE_PRODUCT_DIGIT  '2'
#define PROBE_PID_LOW        0x2E
#define PROBE_RUN_TICKS      (180u * 40u)
#define PROBE_ENTRY_TEXT     "P12 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Start the host capture script, connect PC USB, then YES. P12 stays online for 180 seconds."
#define PROBE_COMPLETE_TEXT  "P12 complete; reboot, then send debug\\usbmicp12.log and the host diagnostic logs"
#elif USB_MIC_P11_PROBE
#define PROBE_TITLE          "UsbKnl11"
#define PROBE_PREFIX         "[UsbKnl11] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp11.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 endpoint activation probe"
#define PROBE_PRODUCT_DIGIT  '1'
#define PROBE_PID_LOW        0x2D
#define PROBE_RUN_TICKS      (180u * 40u)
#define PROBE_ENTRY_TEXT     "P11 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Start the host capture script, connect PC USB, then YES. P11 stays online for 180 seconds."
#define PROBE_COMPLETE_TEXT  "P11 complete; reboot, then send debug\\usbmicp11.log and the host diagnostic logs"
#elif USB_MIC_P10_PROBE
#define PROBE_TITLE          "UsbKnl10"
#define PROBE_PREFIX         "[UsbKnl10] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp10.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 complete function probe"
#define PROBE_PRODUCT_DIGIT  '0'
#define PROBE_PID_LOW        0x2C
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P10 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Start the host capture script during the 90-second P10 test."
#define PROBE_COMPLETE_TEXT  "P10 complete; reboot, then send debug\\usbmicp10.log and the host diagnostic logs"
#elif USB_MIC_P9_PROBE
#define PROBE_TITLE          "UsbKnlP9"
#define PROBE_PREFIX         "[UsbKnlP9] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp9.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 synchronous silence probe"
#define PROBE_PRODUCT_DIGIT  '9'
#define PROBE_PID_LOW        0x2B
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P9 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Start the host capture script during the 90-second P9 test."
#define PROBE_COMPLETE_TEXT  "P9 complete; reboot, then send debug\\usbmicp9.log and the host diagnostic logs"
#elif USB_MIC_P8_PROBE
#define PROBE_TITLE          "UsbKnlP8"
#define PROBE_PREFIX         "[UsbKnlP8] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp8.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 mono channel fix probe"
#define PROBE_PRODUCT_DIGIT  '8'
#define PROBE_PID_LOW        0x2A
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P8 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Start the host capture script during the 90-second P8 test."
#define PROBE_COMPLETE_TEXT  "P8 complete; reboot, then send debug\\usbmicp8.log and the host diagnostic logs"
#elif USB_MIC_EP4_PROBE
#define PROBE_TITLE          "UsbKnlP7"
#define PROBE_PREFIX         "[UsbKnlP7] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp7.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 EP4 silence probe"
#define PROBE_PRODUCT_DIGIT  '7'
#define PROBE_PID_LOW        0x29
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P7 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Start the host capture script during the 90-second EP4 test."
#define PROBE_COMPLETE_TEXT  "P7 complete; reboot, then send debug\\usbmicp7.log and report the host capture result"
#elif USB_MIC_IAD_PROBE
#define PROBE_TITLE          "UsbKnlP6"
#define PROBE_PREFIX         "[UsbKnlP6] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp6.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 IAD silence probe"
#define PROBE_PRODUCT_DIGIT  '6'
#define PROBE_PID_LOW        0x28
#define PROBE_RUN_TICKS      (90u * 40u)
#define PROBE_ENTRY_TEXT     "P6 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Wait for driver setup and open the P6 microphone during the 90-second test."
#define PROBE_COMPLETE_TEXT  "P6 complete; reboot, then send debug\\usbmicp6.log and report the PC audio device status"
#elif USB_MIC_SILENCE_PROBE
#define PROBE_TITLE          "UsbKnlP5"
#define PROBE_PREFIX         "[UsbKnlP5] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp5.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 silence probe"
#define PROBE_PRODUCT_DIGIT  '5'
#define PROBE_PID_LOW        0x27
#define PROBE_RUN_TICKS      (60u * 40u)
#define PROBE_ENTRY_TEXT     "P5 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. Open the P5 microphone on the PC during the 60-second test."
#define PROBE_COMPLETE_TEXT  "P5 complete; reboot, then send debug\\usbmicp5.log and report the PC device status"
#else
#define PROBE_TITLE          "UsbKnlP4"
#define PROBE_PREFIX         "[UsbKnlP4] "
#define PROBE_LOG_PATH       "A:\\\xd3\xa6\xd3\xc3\\\xca\xfd\xbe\xdd\\debug\\usbmicp4.log"
#define PROBE_BEGIN_TEXT     "begin UAC1 descriptor probe"
#define PROBE_PRODUCT_DIGIT  '4'
#define PROBE_PID_LOW        0x26
#define PROBE_RUN_TICKS      (30u * 40u)
#define PROBE_ENTRY_TEXT     "P4 entry reached. Press OK to create the baseline log."
#define PROBE_CONFIRM_TEXT   "Connect PC USB now, then YES. P4 enumerates for 30 seconds and auto-disconnects."
#define PROBE_COMPLETE_TEXT  "P4 complete; send debug\\usbmicp4.log and report the PC device name"
#endif

#if USB_MIC_P15_PROBE && !USB_PCM_TRANSPORT_PROBE && \
    !USB_MIC_ACT10_PROBE && \
    !USB_MIC_ACT11_PROBE && !USB_MIC_ACT12_PROBE && \
    !USB_MIC_ACT13_PROBE
#undef PROBE_COMPLETE_TEXT
#define PROBE_COMPLETE_TEXT \
    "UDC register restore attempted. Reboot before reconnecting USB."
#endif

#define KNL_SIG_R_80005058 0x2604FE20u
#define KNL_SIG_S_8000EF6C 0x24020004u
#define KNL_SIG_USB_INIT   0x3C02B304u
#define KNL_SIG_IRQ_REG    0x000440C0u

#define USB_BASE           0xB3040000u
#define USB_DEVICE_CONFIG  (USB_BASE + 0x400u)
#define USB_DEVICE_CONTROL (USB_BASE + 0x404u)
#define USB_EP0_IN_CONTROL (USB_BASE + 0x000u)
#define USB_EP0_IN_CSR     (USB_BASE + 0x004u)
#define USB_EP0_OUT_CSR    (USB_BASE + 0x204u)
#define USB_DEVICE_STATUS  (USB_BASE + 0x408u)
#define USB_INTRUSB        (USB_BASE + 0x40Cu)
#define USB_CONTROL        (USB_BASE + 0x410u)
#define USB_EP_INTR        (USB_BASE + 0x414u)
#define USB_EP0_COMMIT     (USB_BASE + 0x41Cu)
#define USB_EP0_RESET      (USB_BASE + 0x420u)
#define USB_CSR_BUSY       (USB_BASE + 0x4F0u)
#define USB_SOFT_RESET     (USB_BASE + 0x4FCu)
#define USB_EP0_OUT_NE     (USB_BASE + 0x504u)
#define USB_EP1_OUT_NE     (USB_BASE + 0x50Cu)
#define USB_EP2_IN_NE      (USB_BASE + 0x510u)
#define USB_EP0_OUT_FIFO   (USB_BASE + 0x800u)
#define USB_EP0_IN_FIFO    (USB_BASE + 0x840u)

#if USB_PCM_CDC_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x50Cu)
#define USB_AUDIO_IN_NE_VALUE 0x020008D2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "cdc_ep2_in_count"
#define AUDIO_CSR_OR_NAME    "cdc_ep2_csr_or"
#define AUDIO_PACKET_NAME    "cdc_frame_count"
#define AUDIO_BYTE_NAME      "cdc_wire_byte_count"
#define USB_CDC_NOTIFY_CONTROL (USB_BASE + 0x020u)
#define USB_CDC_NOTIFY_CSR     (USB_BASE + 0x024u)
#define USB_CDC_NOTIFY_NE      (USB_BASE + 0x508u)
#define USB_CDC_NOTIFY_NE_VALUE 0x020000F1u
#define USB_CDC_OUT_CONTROL    (USB_BASE + 0x2A0u)
#define USB_CDC_OUT_CSR        (USB_BASE + 0x2A4u)
#define USB_CDC_OUT_MAX_PKT    (USB_BASE + 0x2ACu)
#define USB_CDC_OUT_FIFO       (USB_BASE + 0x800u)
#define USB_CDC_OUT_NE         (USB_BASE + 0x518u)
#define USB_CDC_OUT_NE_VALUE   0x020008C5u
#define CDC_OUT_INTERRUPT      0x00200000u
#elif USB_PCM_HID_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x020u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x024u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x028u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x02Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x860u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x508u)
#define USB_AUDIO_IN_NE_VALUE 0x020000F1u
#define AUDIO_IN_INTERRUPT   0x00000002u
#define AUDIO_ENDPOINT_ADDR  0x81u
#define AUDIO_COUNT_NAME     "hid_ep1_in_count"
#define AUDIO_CSR_OR_NAME    "hid_ep1_csr_or"
#define AUDIO_PACKET_NAME    "hid_report_count"
#define AUDIO_BYTE_NAME      "hid_wire_byte_count"
#elif USB_MIC_ACT13_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x080u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x084u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x088u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x08Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x920u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x514u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B4u
#define AUDIO_IN_INTERRUPT   0x00000010u
#define AUDIO_ENDPOINT_ADDR  0x84u
#define AUDIO_COUNT_NAME     "ep4_in_count"
#define AUDIO_CSR_OR_NAME    "ep4_csr_or"
#define AUDIO_PACKET_NAME    "ep4_packet_count"
#define AUDIO_BYTE_NAME      "ep4_byte_count"
#elif USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x080u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x084u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x088u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x08Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x920u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x514u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B4u
#define AUDIO_IN_INTERRUPT   0x00000010u
#define AUDIO_ENDPOINT_ADDR  0x84u
#define AUDIO_COUNT_NAME     "ep4_in_count"
#define AUDIO_CSR_OR_NAME    "ep4_csr_or"
#define AUDIO_PACKET_NAME    "ep4_packet_count"
#define AUDIO_BYTE_NAME      "ep4_byte_count"
#elif USB_MIC_ACT10_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x50Cu)
#define USB_AUDIO_IN_NE_VALUE 0x010000B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_ACT9_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x50Cu)
#define USB_AUDIO_IN_NE_VALUE 0x010088B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_ACT8_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x510u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_P26_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x50Cu)
#define USB_AUDIO_IN_NE_VALUE 0x010008B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_P25_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x508u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_P24_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x510u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_PROVEN_IN_PROBE
/*
 * Earlier probes paired this physical EP2 IN engine with slot 0x50c.
 * Keep that historical mapping for log-to-build comparison. Act8 tested the
 * then-assumed direction-slot mapping and paired physical EP2 with 0x510.
 */
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x040u)
#define USB_AUDIO_IN_CSR     (USB_BASE + 0x044u)
#define USB_AUDIO_IN_BUFFER  (USB_BASE + 0x048u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x04Cu)
#define USB_AUDIO_IN_FIFO    (USB_BASE + 0x8A0u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x50Cu)
#define USB_AUDIO_IN_NE_VALUE 0x010088B2u
#define AUDIO_IN_INTERRUPT   0x00000004u
#define AUDIO_ENDPOINT_ADDR  0x82u
#define AUDIO_COUNT_NAME     "ep2_in_count"
#define AUDIO_CSR_OR_NAME    "ep2_csr_or"
#define AUDIO_PACKET_NAME    "ep2_packet_count"
#define AUDIO_BYTE_NAME      "ep2_byte_count"
#elif USB_MIC_EP4_PROBE
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x080u)
#define USB_AUDIO_IN_CSR    (USB_BASE + 0x084u)
#define USB_AUDIO_IN_BUFFER (USB_BASE + 0x088u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x08Cu)
#if USB_MIC_P17_PROBE || USB_MIC_P18_PROBE
/* TX FIFO offsets accumulate the configured sizes of physical EP0-EP3. */
#define USB_AUDIO_IN_FIFO   (USB_BASE + 0x920u)
#else
#define USB_AUDIO_IN_FIFO   (USB_BASE + 0x8E0u)
#endif
#define USB_AUDIO_IN_NE      (USB_BASE + 0x514u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B4u
#define AUDIO_IN_INTERRUPT  0x00000010u
#define AUDIO_ENDPOINT_ADDR 0x84u
#define AUDIO_COUNT_NAME    "ep4_in_count"
#define AUDIO_CSR_OR_NAME   "ep4_csr_or"
#define AUDIO_PACKET_NAME   "ep4_packet_count"
#define AUDIO_BYTE_NAME     "ep4_byte_count"
#else
#define USB_AUDIO_IN_CONTROL (USB_BASE + 0x020u)
#define USB_AUDIO_IN_CSR    (USB_BASE + 0x024u)
#define USB_AUDIO_IN_BUFFER (USB_BASE + 0x028u)
#define USB_AUDIO_IN_MAX_PKT (USB_BASE + 0x02Cu)
#define USB_AUDIO_IN_FIFO   (USB_BASE + 0x880u)
#define USB_AUDIO_IN_NE      (USB_BASE + 0x508u)
#define USB_AUDIO_IN_NE_VALUE 0x010088B1u
#define AUDIO_IN_INTERRUPT  0x00000002u
#define AUDIO_ENDPOINT_ADDR 0x81u
#define AUDIO_COUNT_NAME    "ep1_in_count"
#define AUDIO_CSR_OR_NAME   "ep1_csr_or"
#define AUDIO_PACKET_NAME   "ep1_packet_count"
#define AUDIO_BYTE_NAME     "ep1_byte_count"
#endif

#if USB_PCM_TRANSPORT_PROBE || \
    USB_MIC_ACT13_PROBE || USB_MIC_P26_PROBE || USB_MIC_P27_PROBE || \
    USB_MIC_P28_PROBE || \
    USB_MIC_SYNC1_PROBE || USB_MIC_PCH1_PROBE || \
    USB_MIC_ACTIVATION_PROBE
/* JZ4730 IN endpoints commit packets through the global TXCONFIRM register. */
#define USB_AUDIO_IN_CONFIRM USB_EP0_COMMIT
#else
/* Keep the historical P25 mapping intact for log-to-build comparison. */
#define USB_AUDIO_IN_CONFIRM (USB_AUDIO_IN_CONTROL + 0x18u)
#endif

#if USB_PCM_TRANSPORT_PROBE || USB_MIC_P26_PROBE || USB_MIC_ACT10_PROBE
#define AUDIO_STREAMING_ALT 0u
#else
#define AUDIO_STREAMING_ALT 1u
#endif

#if USB_MIC_SYNC1_PROBE || USB_MIC_PCH1_PROBE
#define AUDIO_ENDPOINT_ATTRIBUTES 0x0Du
#elif USB_MIC_ACTIVATION_PROBE
#define AUDIO_ENDPOINT_ATTRIBUTES 0x05u
#elif USB_MIC_P12_PROBE
#define AUDIO_ENDPOINT_ATTRIBUTES 0x05u
#elif USB_MIC_P9_PROBE || USB_MIC_P10_PROBE || USB_MIC_P11_PROBE
#define AUDIO_ENDPOINT_ATTRIBUTES 0x0Du
#else
#define AUDIO_ENDPOINT_ATTRIBUTES 0x05u
#endif

#define SYS_USB_PHY        0xB0010000u
#define INTC_MASK_REG      0xB0001004u
#define INTC_MASK_SET      0xB0001008u
#define INTC_MASK_CLEAR    0xB000100Cu
#define IRQ12_BIT          0x00001000u

#if USB_MIC_P15_PROBE
#define KNL_USB_START_ADDRESS      0x8018AEA0u
#define KNL_USB_DISCONNECT_ADDRESS 0x8018AD48u
#define KNL_USB_START_SIG0         0x27BDFFE8u
#define KNL_USB_START_SIG1         0xAFBF0010u
#define KNL_USB_DISCONNECT_SIG0    0x3C04B001u
#define KNL_USB_DISCONNECT_SIG1    0x8C830000u
#define STOCK_EP4_NE               0x020000B4u
#endif

#if USB_MIC_INTERRUPT_TIMED_PROBE
#define KNL_IRQ_REGISTER_ADDRESS       0x80004A34u
#define KNL_IRQ_UNREGISTER_ADDRESS     0x80004B64u
#define KNL_STOCK_USB_IRQ_ADDRESS      0x8018A04Cu
#define KNL_IRQ_VECTOR_BASE            0x80479634u
#define KNL_IRQ_UNREGISTER_SIG0        0x27BDFFE8u
#define KNL_STOCK_USB_IRQ_SIG0         0x27BDFFE0u
#define UDC_IRQ_NUMBER                 12u
#define ACT14_DEVICE_EVENT_CAPACITY    16u
#define ACT14_AUDIO_ONLY_EP_MASK       0xFFFFFFEFu
#define ACT15_EP0_EP4_EP_MASK           0xFFFEFFEEu
#define HID_EP0_EP1_EP_MASK             0xFFFEFFFCu
#define CDC_EP0_EP2_EP5_EP_MASK         0xFFDEFFFAu
#define CDC_C2_IDLE_EP_MASK              0xFFDEFFFFu
#define ACT17_PRE_W1C_TRACE_CAPACITY    8u
#define ACT17_TRACE_INVALID             0xFFFFFFFFu
#define ACT18_POST_W1C_TRACE_CAPACITY   8u
#if USB_PCM_CDC_EP0_QUEUE_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000032u
#define INTERRUPT_TIMED_EP_MASK          CDC_C2_IDLE_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "cdc_" name
#elif USB_PCM_CDC_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         CDC_EP0_EP2_EP5_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "cdc_" name
#elif USB_PCM_HID_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         HID_EP0_EP1_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "hid_" name
#elif USB_MIC_ACT18_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         ACT15_EP0_EP4_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "act18_" name
#elif USB_MIC_ACT17_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         ACT15_EP0_EP4_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "act17_" name
#elif USB_MIC_ACT16_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         ACT15_EP0_EP4_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "act16_" name
#elif USB_MIC_ACT15_PROBE
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         ACT15_EP0_EP4_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "act15_" name
#else
#define INTERRUPT_TIMED_DEVICE_MASK      0x00000070u
#define INTERRUPT_TIMED_EP_MASK         ACT14_AUDIO_ONLY_EP_MASK
#define INTERRUPT_TIMED_LOG_LABEL(name) "act14_" name
#endif
#endif

#define USB_INTR_CONNECT   0x00000010u
#define USB_INTR_RESET     0x00000008u
#define USB_INTR_SERVICE_CONFIG    0x00000001u
#define USB_INTR_SERVICE_INTERFACE 0x00000002u
#define USB_DEVICE_CONFIG_CSR_PRG   0x00020000u
#define USB_DEVICE_CONFIG_SOFT_RESET 0x80000000u
#define USB_DEVICE_CONTROL_CSR_DONE 0x00002000u
#define USB_CSR_BUSY_ACTIVE          0x00000001u
#define USB_SOFT_RESET_CORE          0x00000001u
#define USB_SOFT_RESET_PHY           0x00000002u
#define USB_CSR_NE_CFG_MASK          0x00000780u
#define USB_CSR_NE_CFG_SHIFT         7u
#define USB_CSR_NE_INTF_MASK         0x00007800u
#define USB_CSR_NE_INTF_SHIFT        11u
#define USB_CSR_NE_ALT_MASK          0x00078000u
#define USB_CSR_NE_ALT_SHIFT         15u
#define USB_EP_CONTROL_TYPE_MASK     0x00000030u
#define USB_EP_CONTROL_TYPE_ISO      0x00000010u
#define USB_EP_CONTROL_STALL         0x00000001u
#define USB_EP_CONTROL_FLUSH         0x00000002u
#define USB_EP_CONTROL_NAK           0x00000040u
#define USB_EP_CONTROL_SNAK          0x00000080u
#define USB_EP_CONTROL_CNAK         0x00000100u
#if USB_MIC_ACT10_PROBE
#define USB_A10_LEGACY_DEVICE_STATUS_RXFIFO_GUARD 0x00008000u
#endif
#if USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE
#define USB_ACT1_AUDIO_NE_RESET      0x01000034u
#define USB_ACT1_AUDIO_NE_ALT0       0x010008B4u
#define USB_ACT1_AUDIO_NE_ALT1       0x010088B4u
#else
#define USB_ACT1_AUDIO_NE_RESET      0x01000032u
#define USB_ACT1_AUDIO_NE_ALT0       0x010008B2u
#define USB_ACT1_AUDIO_NE_ALT1       0x010088B2u
#endif
#define USB_ACT1_REASON_NORMAL       1u
#define USB_ACT1_REASON_ALT0_FALLBACK 2u
#define USB_ACT5_DECISION_IDLE       1u
#define USB_ACT5_DECISION_START      2u
#define USB_ACT5_DECISION_STOP       3u
#define EP0_OUT_INTERRUPT  0x00010000u
#define EP0_IN_INTERRUPT   0x00000001u
#define EP_CSR_TOKEN       0x00000040u
#define EP0_OUT_SETUP      0x00000020u
#define EP0_OUT_DATA       0x00000010u

#define USB_REQ_GET_STATUS        0u
#define USB_REQ_CLEAR_FEATURE     1u
#define USB_REQ_SET_FEATURE       3u
#define USB_REQ_SET_ADDRESS       5u
#define USB_REQ_GET_DESCRIPTOR    6u
#define USB_REQ_GET_CONFIGURATION 8u
#define USB_REQ_SET_CONFIGURATION 9u
#define USB_REQ_GET_INTERFACE     10u
#define USB_REQ_SET_INTERFACE     11u
#define USB_REQ_SYNCH_FRAME       12u

#define USB_DESC_DEVICE        1u
#define USB_DESC_CONFIGURATION 2u
#define USB_DESC_STRING        3u
#define USB_DESC_HID           0x21u
#define USB_DESC_HID_REPORT    0x22u

#define UAC_SET_CUR 0x01u
#define UAC_GET_CUR 0x81u
#define UAC_GET_MIN 0x82u
#define UAC_GET_MAX 0x83u
#define UAC_GET_RES 0x84u
#define HID_GET_REPORT   0x01u
#define HID_GET_IDLE     0x02u
#define HID_GET_PROTOCOL 0x03u
#define HID_SET_REPORT   0x09u
#define HID_SET_IDLE     0x0Au
#define HID_SET_PROTOCOL 0x0Bu
#define CDC_SET_LINE_CODING        0x20u
#define CDC_GET_LINE_CODING        0x21u
#define CDC_SET_CONTROL_LINE_STATE 0x22u
#define CDC_SEND_BREAK             0x23u

#define EP0_MAX_PACKET       32u
#define EP1_PACKET_BYTES     32u
#define HID_REPORT_BYTES     64u
#define HID_PCM_SAMPLES      16u
#define HID_PCM_MAGIC        0x38383539u
#define CDC_FRAME_BYTES      64u
#define CDC_PCM_SAMPLES      16u
#define CDC_PCM_MAGIC        0x38383539u
#define CDC_LINE_CODING_BYTES 7u
#define CDC_CONTROL_OUT_NONE 0u
#define CDC_CONTROL_OUT_LINE_CODING 1u
#define CDC_C3_CONFIG_FALLBACK_DELAY_TICKS 4u
#define SETUP_TRACE_CAPACITY 64u
#define SERVICE_TRACE_CAPACITY 16u
#define AUDIO_TOKEN_TRACE_CAPACITY 16u

#if USB_MIC_P14_PROBE
#define PCM_RING_BLOCKS       2u
#define PCM_BLOCK_BYTES       BDA_AUDIO_CAPTURE_BLOCK_BYTES
#define PCM_BLOCK_WORDS       (PCM_BLOCK_BYTES / (u32)sizeof(u32))
#define CAPTURE_READY_ADDRESS 0x8019A088u
#define CAPTURE_READY_SIG0    0x27BDFFE8u
#define CAPTURE_READY_SIG1    0xAFBF0014u
#endif

#if USB_MIC_P18_PROBE
#define UI_SCREEN_WIDTH        240
#define UI_SCREEN_HEIGHT       320
#define UI_VX_HEADER_SIZE      24u
#define UI_SCREEN_VX_BYTES     \
    (UI_VX_HEADER_SIZE + UI_SCREEN_WIDTH * UI_SCREEN_HEIGHT * 2u)
#define UI_WAVE_X              18
#define UI_WAVE_COLUMNS        204u
#define UI_WAVE_CENTER_Y       140
#define UI_WAVE_HALF_HEIGHT    56
#define UI_WAVE_TOP            (UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT)
#define UI_WAVE_HEIGHT         (UI_WAVE_HALF_HEIGHT * 2 + 1)
#define UI_WAVE_GAIN           2
#define UI_WAVE_GRID_STEP      50
#define UI_WAVE_SAMPLES_COLUMN 10u
#define UI_WAVE_COLUMNS_TICK   12u
#define UI_WAVE_STRIPE_VX_BYTES \
    (UI_VX_HEADER_SIZE + \
     UI_WAVE_COLUMNS_TICK * UI_WAVE_HEIGHT * 2u)
#define UI_EXIT_X              40
#define UI_EXIT_Y              256
#define UI_EXIT_WIDTH          160
#define UI_EXIT_HEIGHT         42
#define UI_RECORD_DOT_X        18
#define UI_LINK_DOT_X          136
#define UI_STATUS_DOT_Y        225
#define UI_STATUS_DOT_SIZE     8
#define UI_RAW_EVENT_DRAIN     8u
#define UI_INPUT_ARM_TICKS     20u
#define UI_EXIT_NONE           0u
#define UI_EXIT_ESCAPE         1u
#define UI_EXIT_BUTTON         2u
#define UI_EXIT_TIMEOUT        3u
typedef char ui_wave_samples_fit_block[
    (UI_WAVE_COLUMNS * UI_WAVE_SAMPLES_COLUMN <= PCM_BLOCK_BYTES / 2u)
        ? 1 : -1
];
#endif

typedef struct {
    u8 bm_request_type;
    u8 b_request;
    u16 w_value;
    u16 w_index;
    u16 w_length;
} usb_setup_packet_t;

#if USB_MIC_PROVEN_IN_PROBE
typedef struct {
    u32 intrusb;
    u32 device_status;
    u32 ep_intr;
    u32 audio_control;
    u32 audio_csr;
    u32 audio_ne;
} usb_service_trace_t;
#endif
#if USB_MIC_ACT10_PROBE || USB_MIC_ACT13_PROBE
typedef struct {
    u32 ordinal;
    u32 loop_count;
    u32 device_status;
    u32 ep_intr;
    u32 audio_control;
    u32 audio_csr;
    u32 audio_ne;
} default_alt_audio_token_trace_t;
#endif
#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
typedef struct {
    u32 ordinal;
    u32 loop_count;
    u32 device_status;
    u32 ep_intr;
    u32 decision;
    u32 effective_alt;
    u32 ep0_out_ne;
    u32 audio_control;
} act5_si_trace_t;
#endif
#if USB_MIC_ACT18_PROBE
typedef struct {
    u32 ordinal;
    u32 status_pre;
    u32 status_after_w1c;
    u32 decision;
    u32 effective_alt;
    u32 ep0_info_before;
    u32 ep0_info_written;
    u32 ep0_info_readback;
    u32 audio_info_before;
    u32 audio_info_written;
    u32 audio_info_readback;
    u32 control_before_done;
    u32 control_after_done;
    u32 status_after_done;
} act18_post_w1c_trace_t;
#endif
#if USB_MIC_ACT17_PROBE
typedef struct {
    u32 ordinal;
    u32 status_pre;
    u32 decision;
    u32 effective_alt;
    u32 ep0_info_before;
    u32 ep0_info_written;
    u32 ep0_info_readback;
    u32 audio_info;
    u32 audio_control;
    u32 status_post;
} act17_pre_w1c_trace_t;
#endif

static const u8 k_device_desc[] = {
#if USB_PCM_CDC_C3_PROBE
    0x12, 0x01, 0x10, 0x01, 0xef, 0x02, 0x01, 0x20,
#elif USB_PCM_CDC_PROBE
    0x12, 0x01, 0x10, 0x01, 0x02, 0x00, 0x00, 0x20,
#elif USB_PCM_HID_PROBE
    0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x20,
#elif USB_MIC_IAD_PROBE || USB_MIC_P10_PROBE || USB_MIC_P11_PROBE || USB_MIC_P12_PROBE
    0x12, 0x01, 0x10, 0x01, 0xef, 0x02, 0x01, 0x20,
#else
    0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x20,
#endif
    0xa5, 0xa4, PROBE_PID_LOW, 0x05, 0x00, 0x01, 0x01, 0x02,
    0x00, 0x01,
};

static const u8 k_config_desc[] = {
#if USB_PCM_CDC_C3_PROBE
    0x09, 0x02, 0x4b, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00,
    0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,
    0x05, 0x24, 0x00, 0x10, 0x01,
    0x05, 0x24, 0x01, 0x00, 0x01,
    0x04, 0x24, 0x02, 0x06,
    0x05, 0x24, 0x06, 0x00, 0x01,
    0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x20,
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,
    0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,
    0x07, 0x05, 0x05, 0x02, 0x40, 0x00, 0x00,
#elif USB_PCM_CDC_PROBE
    0x09, 0x02, 0x43, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,
    0x05, 0x24, 0x00, 0x10, 0x01,
    0x05, 0x24, 0x01, 0x00, 0x01,
    0x04, 0x24, 0x02, 0x06,
    0x05, 0x24, 0x06, 0x00, 0x01,
    0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x20,
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,
    0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,
    0x07, 0x05, 0x05, 0x02, 0x40, 0x00, 0x00,
#elif USB_PCM_HID_PROBE
    0x09, 0x02, 0x22, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
    0x09, 0x04, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00,
    0x09, 0x21, 0x11, 0x01, 0x00, 0x01, 0x22, 0x15, 0x00,
    0x07, 0x05, 0x81, 0x03, 0x40, 0x00, 0x01,
#else
#if USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || USB_MIC_ACT13_PROBE
    0x09, 0x02, 0x75, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x02, 0x00, 0x00,
#elif USB_MIC_ACT10_PROBE
    0x09, 0x02, 0x6c, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x02, 0x00, 0x00,
#elif USB_MIC_P26_PROBE
    0x09, 0x02, 0x6f, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x02, 0x00, 0x00,
#elif USB_MIC_P12_PROBE
    0x09, 0x02, 0x78, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x02, 0x00, 0x00,
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE
    0x09, 0x02, 0x77, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x01, 0x00, 0x00,
#elif USB_MIC_IAD_PROBE
    0x09, 0x02, 0x6c, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
    0x08, 0x0b, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
#else
    0x09, 0x02, 0x64, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
#endif

    0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,
#if USB_MIC_P12_PROBE
    0x09, 0x24, 0x01, 0x00, 0x01, 0x27, 0x00, 0x01, 0x01,
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE
    0x09, 0x24, 0x01, 0x00, 0x01, 0x29, 0x00, 0x01, 0x01,
#else
    0x09, 0x24, 0x01, 0x00, 0x01, 0x1e, 0x00, 0x01, 0x01,
#endif
#if USB_MIC_P12_PROBE
    0x0c, 0x24, 0x02, 0x01, 0x02, 0x02, 0x00, 0x01,
#else
    0x0c, 0x24, 0x02, 0x01, 0x01, 0x02, 0x00, 0x01,
#endif
    0x00, 0x00, 0x00, 0x00,
#if USB_MIC_P12_PROBE
    0x09, 0x24, 0x06, 0x05, 0x01, 0x01, 0x01, 0x02, 0x00,
    0x09, 0x24, 0x03, 0x03, 0x01, 0x01, 0x01, 0x05, 0x00,
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE
    0x0b, 0x24, 0x06, 0x02, 0x01, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x09, 0x24, 0x03, 0x03, 0x01, 0x01, 0x00, 0x02, 0x00,
#else
    0x09, 0x24, 0x03, 0x02, 0x01, 0x01, 0x00, 0x01, 0x00,
#endif

#if !USB_MIC_P26_PROBE && !USB_MIC_ACT10_PROBE
    0x09, 0x04, 0x01, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
#endif
#if USB_MIC_P26_PROBE || USB_MIC_ACT10_PROBE
    0x09, 0x04, 0x01, 0x00, 0x01, 0x01, 0x02, 0x00, 0x00,
#else
    0x09, 0x04, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00,
#endif
#if USB_MIC_P10_PROBE || USB_MIC_P11_PROBE || USB_MIC_P12_PROBE
    0x07, 0x24, 0x01, 0x03, 0x01, 0x01, 0x00,
#else
    0x07, 0x24, 0x01, 0x02, 0x01, 0x01, 0x00,
#endif
#if USB_MIC_ACT10_PROBE || USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || \
    USB_MIC_ACT13_PROBE
    0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01,
    0x80, 0x3e, 0x00,
#elif USB_MIC_P12_PROBE
    0x0e, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x02,
    0x40, 0x1f, 0x00, 0x80, 0x3e, 0x00,
#else
    0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01,
    0x80, 0x3e, 0x00,
#endif
    0x09, 0x05, AUDIO_ENDPOINT_ADDR, AUDIO_ENDPOINT_ATTRIBUTES,
    0x20, 0x00, 0x01, 0x00, 0x00,
#if USB_MIC_P12_PROBE
    0x07, 0x25, 0x01, 0x01, 0x00, 0x00, 0x00,
#else
    0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00,
#endif
#endif
};

#if USB_PCM_HID_PROBE
static const u8 k_hid_desc[] = {
    0x09, 0x21, 0x11, 0x01, 0x00, 0x01, 0x22, 0x15, 0x00,
};
static const u8 k_hid_report_desc[] = {
    0x06, 0x00, 0xff,
    0x09, 0x01,
    0xa1, 0x01,
    0x15, 0x00,
    0x26, 0xff, 0x00,
    0x75, 0x08,
    0x95, 0x40,
    0x09, 0x01,
    0x81, 0x02,
    0xc0,
};
#endif

static const u8 k_string0_desc[] = {0x04, 0x03, 0x09, 0x04};
static const u8 k_string_mfr_desc[] = {
    0x08, 0x03, 'B', 0x00, 'B', 0x00, 'K', 0x00,
};
static const u8 k_string_product_desc[] = {
#if USB_PCM_CDC_C3_PROBE
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '3', 0x00,
#elif USB_PCM_CDC_C2_PROBE
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '2', 0x00,
#elif USB_PCM_CDC_PROBE
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '1', 0x00,
#elif USB_PCM_HID_PROBE
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'H', 0x00, 'I', 0x00, 'D', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'H', 0x00, '1', 0x00,
#elif USB_MIC_ACT18_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '8', 0x00,
#elif USB_MIC_ACT17_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '7', 0x00,
#elif USB_MIC_ACT16_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '6', 0x00,
#elif USB_MIC_ACT15_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '5', 0x00,
#elif USB_MIC_ACT14_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '4', 0x00,
#elif USB_MIC_ACT13_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '3', 0x00,
#elif USB_MIC_ACT12_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '2', 0x00,
#elif USB_MIC_ACT11_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '1', 0x00,
#elif USB_MIC_ACT10_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '1', 0x00, '0', 0x00,
#elif USB_MIC_ACT9_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '9', 0x00, 'D', 0x00,
#elif USB_MIC_ACT8_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '8', 0x00, 'C', 0x00,
#elif USB_MIC_ACT7_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '7', 0x00, 'S', 0x00,
#elif USB_MIC_ACT6_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '6', 0x00, 'L', 0x00,
#elif USB_MIC_ACT5_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '5', 0x00, 'L', 0x00,
#elif USB_MIC_ACT4_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '4', 0x00, 'L', 0x00,
#elif USB_MIC_ACT3_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, '3', 0x00, 'L', 0x00,
#elif USB_MIC_ACTIVATION_PROBE
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'A', 0x00, PROBE_PRODUCT_DIGIT, 0x00,
#elif USB_MIC_PCH1_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, 'H', 0x00, '1', 0x00,
#elif USB_MIC_SYNC1_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, '1', 0x00,
#elif USB_MIC_P28_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '8', 0x00,
#elif USB_MIC_P27_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '7', 0x00,
#elif USB_MIC_P26_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '6', 0x00,
#elif USB_MIC_P25_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '5', 0x00,
#elif USB_MIC_P24_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '4', 0x00,
#elif USB_MIC_P23_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '3', 0x00,
#elif USB_MIC_P22_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '2', 0x00,
#elif USB_MIC_P21_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '1', 0x00,
#elif USB_MIC_P20_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '2', 0x00, '0', 0x00,
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE || USB_MIC_P12_PROBE
    0x22, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, '1', 0x00, PROBE_PRODUCT_DIGIT, 0x00,
#else
    0x20, 0x03,
    '9', 0x00, '5', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'U', 0x00, 'S', 0x00, 'B', 0x00,
    ' ', 0x00, 'M', 0x00, 'i', 0x00, 'c', 0x00,
    ' ', 0x00, 'P', 0x00, PROBE_PRODUCT_DIGIT, 0x00,
#endif
};
#if USB_PCM_CDC_C3_PROBE
static const u8 k_string_product_c100_desc[] = {
    0x20, 0x03,
    '9', 0x00, '6', 0x00, '8', 0x00, '8', 0x00,
    ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00,
    ' ', 0x00, 'P', 0x00, 'C', 0x00, 'M', 0x00,
    ' ', 0x00, 'C', 0x00, '3', 0x00,
};
#endif

static const u8 k_zero1[] = {0x00};
static const u8 k_zero2[] = {0x00, 0x00};
#if USB_PCM_HID_PROBE
static const u8 k_hid_zero_report[HID_REPORT_BYTES] = {0x00};
#endif
static const u8 k_rate_16000[] = {0x80, 0x3e, 0x00};
static const u8 k_rate_res[] = {0x01, 0x00, 0x00};
#if USB_MIC_P12_PROBE
static const u8 k_rate_8000[] = {0x40, 0x1f, 0x00};
static const u8 k_rate_res_8000[] = {0x40, 0x1f, 0x00};
static const u8 k_mute_current[] = {0x00};
static const u8 k_volume_current[] = {0x00, 0x00};
static const u8 k_volume_min[] = {0x00, 0xa0};
static const u8 k_volume_max[] = {0x00, 0x00};
static const u8 k_volume_res[] = {0x00, 0x01};
#endif

typedef char device_desc_must_be_18[(sizeof(k_device_desc) == 18u) ? 1 : -1];
#if USB_PCM_CDC_C3_PROBE
typedef char config_desc_must_be_75[(sizeof(k_config_desc) == 75u) ? 1 : -1];
#elif USB_PCM_CDC_PROBE
typedef char config_desc_must_be_67[(sizeof(k_config_desc) == 67u) ? 1 : -1];
#elif USB_PCM_HID_PROBE
typedef char config_desc_must_be_34[(sizeof(k_config_desc) == 34u) ? 1 : -1];
typedef char hid_desc_must_be_9[(sizeof(k_hid_desc) == 9u) ? 1 : -1];
typedef char hid_report_desc_must_be_21[
    (sizeof(k_hid_report_desc) == 21u) ? 1 : -1
];
#elif USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || USB_MIC_ACT13_PROBE
typedef char config_desc_must_be_117[(sizeof(k_config_desc) == 117u) ? 1 : -1];
#elif USB_MIC_ACT10_PROBE
typedef char config_desc_must_be_108[(sizeof(k_config_desc) == 108u) ? 1 : -1];
#elif USB_MIC_P26_PROBE
typedef char config_desc_must_be_111[(sizeof(k_config_desc) == 111u) ? 1 : -1];
#elif USB_MIC_P12_PROBE
typedef char config_desc_must_be_120[(sizeof(k_config_desc) == 120u) ? 1 : -1];
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE
typedef char config_desc_must_be_119[(sizeof(k_config_desc) == 119u) ? 1 : -1];
#elif USB_MIC_IAD_PROBE
typedef char config_desc_must_be_108[(sizeof(k_config_desc) == 108u) ? 1 : -1];
#else
typedef char config_desc_must_be_100[(sizeof(k_config_desc) == 100u) ? 1 : -1];
#endif
#if USB_PCM_TRANSPORT_PROBE
typedef char product_desc_must_be_32[(sizeof(k_string_product_desc) == 32u) ? 1 : -1];
#if USB_PCM_CDC_C3_PROBE
typedef char c100_product_desc_must_be_32[
    (sizeof(k_string_product_c100_desc) == 32u) ? 1 : -1
];
#endif
#elif USB_MIC_ACT10_PROBE || USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || \
    USB_MIC_ACT13_PROBE || \
    USB_MIC_ACT3_PROBE || USB_MIC_ACT4_PROBE || \
    USB_MIC_ACT5_PROBE || \
    USB_MIC_ACT7_PROBE || USB_MIC_ACT8_PROBE || USB_MIC_ACT9_PROBE
typedef char product_desc_must_be_34[(sizeof(k_string_product_desc) == 34u) ? 1 : -1];
#elif USB_MIC_ACTIVATION_PROBE
typedef char product_desc_must_be_32[(sizeof(k_string_product_desc) == 32u) ? 1 : -1];
#elif USB_MIC_P10_PROBE || USB_MIC_P11_PROBE || USB_MIC_P12_PROBE
typedef char product_desc_must_be_34[(sizeof(k_string_product_desc) == 34u) ? 1 : -1];
#else
typedef char product_desc_must_be_32[(sizeof(k_string_product_desc) == 32u) ? 1 : -1];
#endif

#if USB_MIC_INTERRUPT_TIMED_PROBE
#define USB_IRQ_SHARED volatile
#else
#define USB_IRQ_SHARED
#endif

static usb_setup_packet_t g_setup_trace[SETUP_TRACE_CAPACITY];
static USB_IRQ_SHARED u32 g_setup_count;
static u32 g_setup_trace_count;
static u32 g_unsupported_count;
static u32 g_intrusb_or;
static u32 g_intrusb_count;
static u32 g_reset_count;
static u32 g_connect_count;
static u32 g_service_config_count;
static u32 g_service_interface_count;
static u32 g_device_status_or;
static u32 g_device_status_last;
static u32 g_device_status_change_count;
static u32 g_ep0_out_count;
static u32 g_ep0_in_count;
static USB_IRQ_SHARED u32 g_ep1_in_count;
static USB_IRQ_SHARED u32 g_ep1_csr_or;
static USB_IRQ_SHARED u32 g_ep1_packet_count;
static USB_IRQ_SHARED u32 g_ep1_byte_count;
#if USB_PCM_CDC_PROBE
static USB_IRQ_SHARED u32 g_cdc_sequence;
static USB_IRQ_SHARED u32 g_cdc_valid_pcm_frame_count;
static USB_IRQ_SHARED u32 g_cdc_empty_token_count;
static USB_IRQ_SHARED u32 g_cdc_class_request_count;
static USB_IRQ_SHARED u32 g_cdc_set_line_coding_count;
static USB_IRQ_SHARED u32 g_cdc_get_line_coding_count;
static USB_IRQ_SHARED u32 g_cdc_set_control_line_count;
static USB_IRQ_SHARED u32 g_cdc_send_break_count;
static USB_IRQ_SHARED u32 g_cdc_control_line_state;
static USB_IRQ_SHARED u32 g_cdc_break_value;
static USB_IRQ_SHARED u32 g_cdc_csr_done_suppressed_count;
static USB_IRQ_SHARED u32 g_cdc_out_irq_count;
static USB_IRQ_SHARED u32 g_cdc_out_packet_count;
static USB_IRQ_SHARED u32 g_cdc_out_byte_count;
static USB_IRQ_SHARED u32 g_cdc_out_last_count;
static USB_IRQ_SHARED u32 g_cdc_bus_reset_rearm_count;
static USB_IRQ_SHARED u32 g_cdc_ep2_irq_enable_count;
static USB_IRQ_SHARED u32 g_cdc_ep2_irq_disable_count;
static USB_IRQ_SHARED u32 g_cdc_ep2_irq_mask_last;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
static USB_IRQ_SHARED u32 g_cdc_ep0_irq_enable_count;
static USB_IRQ_SHARED u32 g_cdc_ep0_irq_disable_count;
static USB_IRQ_SHARED u32 g_cdc_ep0_empty_irq_count;
static USB_IRQ_SHARED u32 g_cdc_ep0_w1c_before_service_count;
static USB_IRQ_SHARED u32 g_cdc_ep0_irq_mask_last;
#endif
#if USB_PCM_CDC_C3_PROBE
static USB_IRQ_SHARED u32 g_cdc_c3_bus_setup_base;
static USB_IRQ_SHARED u32 g_cdc_c3_bus_sc_base;
static USB_IRQ_SHARED u32 g_cdc_c3_wait_armed;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_done_on_bus;
static USB_IRQ_SHARED u32 g_cdc_c3_wait_armed_count;
static USB_IRQ_SHARED u32 g_cdc_c3_wait_cancelled_by_sc_count;
static USB_IRQ_SHARED u32 g_cdc_c3_cfg1_poll_count;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_trigger_count;
static USB_IRQ_SHARED u32 g_cdc_c3_wait_start_tick;
static USB_IRQ_SHARED u32 g_cdc_c3_wait_start_loop;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_last_status;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_last_intr_before;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_last_intr_after;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_last_loop;
static USB_IRQ_SHARED u32 g_cdc_c3_fallback_last_setup_count;
#endif
static u8 g_cdc_line_coding[CDC_LINE_CODING_BYTES] = {
    0x00, 0xc2, 0x01, 0x00, 0x00, 0x00, 0x08
};
static u8 g_cdc_out_buffer[64];
#endif
#if USB_PCM_HID_PROBE
static USB_IRQ_SHARED u32 g_hid_sequence;
static USB_IRQ_SHARED u32 g_hid_valid_pcm_report_count;
static USB_IRQ_SHARED u32 g_hid_silence_report_count;
static USB_IRQ_SHARED u32 g_hid_class_request_count;
static USB_IRQ_SHARED u32 g_hid_set_idle_count;
static USB_IRQ_SHARED u32 g_hid_get_idle_count;
static USB_IRQ_SHARED u32 g_hid_set_protocol_count;
static USB_IRQ_SHARED u32 g_hid_get_protocol_count;
static USB_IRQ_SHARED u32 g_hid_report_descriptor_count;
static USB_IRQ_SHARED u32 g_hid_cnak_count;
static USB_IRQ_SHARED u32 g_hid_csr_done_suppressed_count;
#endif
static u32 g_ep0_packet_count;
static u32 g_ep0_byte_count;
static u32 g_status_out_count;
static u32 g_other_ep_intr_or;
static USB_IRQ_SHARED u32 g_loop_count;
static u32 g_original_phy;
static u32 g_original_intc_mask;
static u32 g_stop_intrusb;
static u32 g_stop_ep_intr;
#if USB_MIC_P13_PROBE
static u32 g_stop_device_status;
static u32 g_stop_audio_control;
static u32 g_stop_audio_csr;
static u32 g_stop_audio_buffer_size;
static u32 g_stop_audio_max_packet;
static u32 g_stop_audio_ne;
#endif
#if USB_MIC_P14_PROBE
static bda_audio_capture_t g_capture = BDA_AUDIO_CAPTURE_INITIALIZER;
static const bda_firmware_profile_t *g_active_profile;
static u32 g_pcm_ring[PCM_RING_BLOCKS][PCM_BLOCK_WORDS]
    __attribute__((aligned(4)));
static USB_IRQ_SHARED u32 g_pcm_read_slot;
static u32 g_pcm_write_slot;
static USB_IRQ_SHARED u32 g_pcm_block_count;
static USB_IRQ_SHARED u32 g_pcm_read_offset;
static u32 g_pcm_ring_high_water;
static u32 g_capture_firmware;
static u32 g_capture_started;
static u32 g_capture_open_result;
static u32 g_capture_preroll_result;
static u32 g_capture_last_read_result;
static u32 g_capture_stop_result;
static u32 g_capture_read_count;
static u32 g_capture_read_error_count;
static u32 g_capture_ready_count;
static u32 g_capture_slow_read_count;
static u32 g_capture_backpressure_count;
static u32 g_capture_idle_drop_count;
static USB_IRQ_SHARED u32 g_audio_streaming;
static USB_IRQ_SHARED u32 g_pcm_packet_count;
static USB_IRQ_SHARED u32 g_pcm_byte_count;
static USB_IRQ_SHARED u32 g_silence_packet_count;
static USB_IRQ_SHARED u32 g_pcm_underrun_count;
static u32 g_preroll_nonzero_samples;
static u32 g_preroll_peak;
#endif
#if USB_MIC_P15_PROBE
static u32 g_recovery_attempted;
static u32 g_recovery_precheck_ok;
static u32 g_recovery_ok;
static u32 g_recovery_mask_before;
static u32 g_recovery_mask_after;
static u32 g_recovery_phy_after;
static u32 g_recovery_core_after;
static u32 g_recovery_control_after;
static u32 g_recovery_ep4_ne_after;
static u32 g_recovery_intrusb_after;
static u32 g_recovery_ep_intr_after;
#endif
#if USB_MIC_P16_PROBE
static u32 g_service_status_or;
static u32 g_service_status_last;
static u32 g_service_config_last;
static u32 g_service_interface_last;
static u32 g_service_alt_last;
static u32 g_service_invalid_count;
static u32 g_csr_done_count;
static u32 g_ep4_cnak_count;
static u32 g_device_control_after_service;
#endif
#if USB_MIC_POST_IRQ_STATUS_PROBE
static u32 g_service_status_pre_clear_or;
static u32 g_service_status_pre_clear_last;
static u32 g_service_status_post_clear_or;
static u32 g_service_status_post_clear_last;
static u32 g_service_post_clear_alt1_count;
#endif
#if USB_MIC_PROVEN_IN_PROBE
static usb_service_trace_t g_service_trace[SERVICE_TRACE_CAPACITY];
static u32 g_service_trace_count;
#endif
#if USB_MIC_DYNAMIC_CSR_PROBE
static u32 g_device_config_after_init;
static u32 g_device_config_last_service;
static u32 g_device_config_at_stop;
static u32 g_csr_wait_count;
static u32 g_csr_wait_timeout_count;
static u32 g_csr_wait_spin_max;
static u32 g_csr_write_count;
static u32 g_csr_busy_last;
#endif
#if USB_MIC_SERVICE_CSR_PROBE
static u32 g_ep0_out_ne_before_service;
static u32 g_ep0_out_ne_after_service;
static u32 g_dynamic_config_program_count;
static u32 g_dynamic_interface_program_count;
#endif
#if USB_MIC_SOFT_RESET_CSR_PROBE
static u32 g_soft_reset_before;
static u32 g_soft_reset_after;
static u32 g_soft_reset_count;
static u32 g_device_config_before_write;
static u32 g_device_config_write_value;
#endif
#if USB_MIC_ACTIVATION_PROBE
static u32 g_act1_device_config_after_init;
static u32 g_act1_device_config_at_stop;
static u32 g_act1_initial_control;
static u32 g_act1_initial_ne;
static u32 g_act1_initial_buffer;
static u32 g_act1_initial_max_packet;
static u32 g_act1_idle_reset_count;
static u32 g_act1_service_count;
static u32 g_act1_combined_si_count;
static u32 g_act1_standalone_si_count;
static u32 g_act1_alt0_count;
static u32 g_act1_alt1_count;
static u32 g_act1_activation_count;
static u32 g_act1_normal_activation_count;
static u32 g_act1_fallback_activation_count;
static u32 g_act1_disable_count;
static u32 g_act1_fallback_armed;
static u32 g_act1_activation_reason;
static u32 g_act1_ne_before_activation;
static u32 g_act1_ne_after_activation;
static u32 g_act1_ne_last;
static u32 g_act1_control_before_activation;
static u32 g_act1_control_after_activation;
static u32 g_act1_control_last;
static u32 g_act1_status_after_activation;
static u32 g_act1_ep_intr_after_activation;
static u32 g_act1_cnak_write_count;
static u32 g_act1_cnak_retry_count;
static u32 g_act1_nak_after_cnak_count;
static u32 g_act1_status_after_csr_done;
#endif
#if USB_MIC_ACT4_PROBE || USB_MIC_ORDERED_SI_ACT_PROBE
static u32 g_act4_effective_alt_last;
static u32 g_act4_inferred_alt1_count;
static u32 g_act4_inferred_alt0_count;
static u32 g_act4_ep0_out_ne_before;
static u32 g_act4_ep0_out_ne_after;
static u32 g_act4_ep0_in_control_before;
static u32 g_act4_ep0_in_control_after;
static u32 g_act4_clear_stall_nak_count;
#endif
#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
static act5_si_trace_t g_act5_si_trace[8];
static u32 g_act5_si_trace_count;
static u32 g_act5_idle_primed;
static u32 g_act5_idle_si_count;
static u32 g_act5_start_si_count;
static u32 g_act5_stop_si_count;
static u32 g_act5_csr_busy_before_done;
#endif
#if USB_MIC_ACT6_PROBE
static u32 g_act6_deferred_start_count;
static u32 g_act6_deferred_stop_count;
static u32 g_act6_status_guard_count;
static u32 g_act6_audio_preack_skip_count;
static u32 g_act6_device_control_before_done;
static u32 g_act6_device_control_after_done;
static u32 g_act6_audio_control_before_done;
static u32 g_act6_audio_control_after_done;
static u32 g_act6_audio_ne_after_done;
static u32 g_act6_first_audio_token_loop;
#endif
#if USB_MIC_STATIC_CSR_ACT_PROBE
static u32 g_act7_static_service_count;
static u32 g_act7_ne_write_suppressed_count;
static u32 g_act7_csr_done_suppressed_count;
static u32 g_act7_software_start_count;
static u32 g_act7_software_stop_count;
static USB_IRQ_SHARED u32 g_act7_first_audio_token_loop;
static u32 g_act7_device_config_write_value;
static u32 g_act7_device_config_readback;
static u32 g_act7_ep0_ne_initial;
static u32 g_act7_ep0_ne_last;
static u32 g_act7_audio_ne_initial;
static u32 g_act7_audio_ne_last;
static u32 g_act7_audio_control_at_start;
static u32 g_act7_prearm_count;
static u32 g_act7_prearm_cnak_write_count;
static u32 g_act7_prearm_nak_after_count;
static u32 g_act7_audio_control_before_prearm;
static u32 g_act7_audio_control_after_prearm;
#endif
#if USB_MIC_ACT8_PROBE
static u32 g_act8_ep1_out_slot_initial;
static u32 g_act8_ep1_out_slot_last;
static u32 g_act8_ep2_in_slot_initial;
static u32 g_act8_ep2_in_slot_last;
#endif
#if USB_MIC_ACT9_PROBE
static u32 g_act9_devcfg_before_reset;
static u32 g_act9_devcfg_after_reset;
static u32 g_act9_devcfg_write_value;
static u32 g_act9_devcfg_readback;
static u32 g_act9_hard_reset_count;
static u32 g_act9_dynamic_ready;
static u32 g_act9_epreg_reset_count;
static u32 g_act9_epreg_clear_write_count;
static u32 g_act9_epreg_nonzero_after_clear;
static u32 g_act9_ep0_ne_after_reset;
static u32 g_act9_audio_ne_after_reset;
static u32 g_act9_first_audio_token_loop;
#endif
#if USB_MIC_ACT10_PROBE
static default_alt_audio_token_trace_t
    g_a10_audio_token_trace[AUDIO_TOKEN_TRACE_CAPACITY];
static u32 g_a10_audio_token_trace_count;
static u32 g_a10_device_config_after_init;
static u32 g_a10_initial_control;
static u32 g_a10_initial_csr;
static u32 g_a10_initial_ne;
static u32 g_a10_initial_buffer;
static u32 g_a10_initial_max_packet;
static u32 g_a10_control_last;
static u32 g_a10_csr_last;
static u32 g_a10_ne_last;
static u32 g_a10_software_reset_count;
static u32 g_a10_static_service_count;
static u32 g_a10_csr_done_suppressed_count;
static u32 g_a10_first_audio_token_loop;
static u32 g_a10_cnak_pending;
static u32 g_a10_cnak_write_count;
static u32 g_a10_cnak_deferred_count;
static u32 g_a10_cnak_success_count;
static u32 g_a10_cnak_retry_limit_count;
static u32 g_a10_cnak_status_last;
#endif
#if USB_MIC_ACT13_PROBE
static default_alt_audio_token_trace_t
    g_act13_audio_token_trace[AUDIO_TOKEN_TRACE_CAPACITY];
static u32 g_act13_audio_token_trace_count;
static u32 g_act13_device_config_after_init;
static u32 g_act13_initial_control;
static u32 g_act13_initial_csr;
static u32 g_act13_initial_ne;
static u32 g_act13_initial_buffer;
static u32 g_act13_initial_max_packet;
static u32 g_act13_control_last;
static u32 g_act13_csr_last;
static u32 g_act13_ne_last;
static u32 g_act13_software_reset_count;
static u32 g_act13_static_service_count;
static u32 g_act13_csr_done_suppressed_count;
static u32 g_act13_first_audio_token_loop;
static u32 g_act13_masked_si_seen;
static u32 g_act13_masked_si_poll_count;
static u32 g_act13_masked_si_first_loop;
static u32 g_act13_masked_si_status_first;
static u32 g_act13_masked_si_status_last;
#endif
#if USB_MIC_INTERRUPT_TIMED_PROBE
static volatile u32
    g_act14_event_intr[ACT14_DEVICE_EVENT_CAPACITY];
static volatile u32
    g_act14_event_status_pre[ACT14_DEVICE_EVENT_CAPACITY];
static volatile u32
    g_act14_event_status_post[ACT14_DEVICE_EVENT_CAPACITY];
static volatile u32 g_act14_event_write;
static volatile u32 g_act14_event_read;
static volatile u32 g_act14_irq_entry_count;
static volatile u32 g_act14_device_irq_count;
static volatile u32 g_act14_audio_irq_count;
static volatile u32 g_act14_spurious_irq_count;
static volatile u32 g_act14_event_overflow_count;
static volatile u32 g_act14_event_queue_high_water;
static volatile u32 g_act14_si_ack_count;
static volatile u32 g_act14_sc_ack_count;
static volatile u32 g_act14_reset_ack_count;
static volatile u32 g_act14_first_si_irq_loop;
static volatile u32 g_act14_si_status_pre_first;
static volatile u32 g_act14_si_status_post_first;
static volatile u32 g_act14_si_status_post_last;
static volatile u32 g_act14_post_ack_alt1_count;
static u32 g_act14_irq_handler_before;
static u32 g_act14_irq_argument_before;
static u32 g_act14_irq_register_result;
static u32 g_act14_irq_handler_after_install;
static u32 g_act14_irq_argument_after_install;
static u32 g_act14_intc_mask_after_install;
static u32 g_act14_irq_handler_after_restore;
static u32 g_act14_irq_argument_after_restore;
static u32 g_act14_irq_install_ok;
static u32 g_act14_irq_restore_ok;
static u32 g_act14_irq_installed;
static u32 g_act14_device_mask_readback;
static u32 g_act14_endpoint_mask_readback;
#endif
#if USB_MIC_EP0_IRQ_PROBE
static volatile u32 g_act15_ep_intr_or;
static volatile u32 g_act15_ep0_out_irq_count;
static volatile u32 g_act15_ep0_in_irq_count;
static volatile u32 g_act15_other_ep_irq_count;
static volatile u32 g_act15_reset_ep_discard_count;
static volatile u32 g_act15_reset_fast_count;
static volatile u32 g_act15_endpoint_mask_last;
#endif
#if USB_MIC_ACT16_PROBE
static volatile u32 g_act16_csr_done_count;
static volatile u32 g_act16_sc_csr_done_count;
static volatile u32 g_act16_si_csr_done_count;
static volatile u32 g_act16_csr_done_control_before_first;
static volatile u32 g_act16_csr_done_control_before_last;
static volatile u32 g_act16_csr_done_control_after_first;
static volatile u32 g_act16_csr_done_control_after_last;
static volatile u32 g_act16_status_pre_done_first;
static volatile u32 g_act16_status_pre_done_last;
static volatile u32 g_act16_status_post_done_first;
static volatile u32 g_act16_status_post_done_last;
static volatile u32 g_act16_ep0_in_control_before_last;
static volatile u32 g_act16_ep0_in_control_after_last;
static volatile u32 g_act16_ep0_in_nak_seen_count;
static volatile u32 g_act16_ep0_in_cnak_write_count;
#endif
#if USB_MIC_ACT18_PROBE
static act18_post_w1c_trace_t
    g_act18_post_w1c_trace[ACT18_POST_W1C_TRACE_CAPACITY];
static volatile u32 g_act18_post_w1c_trace_count;
static volatile u32 g_act18_si_complete_count;
static volatile u32 g_act18_irq_idle_primed;
static volatile u32 g_act18_irq_streaming;
static volatile u32 g_act18_idle_si_count;
static volatile u32 g_act18_start_si_count;
static volatile u32 g_act18_stop_si_count;
static volatile u32 g_act18_unexpected_si_state_count;
static volatile u32 g_act18_ep0_info_write_count;
static volatile u32 g_act18_ep0_info_readback_mismatch_count;
static volatile u32 g_act18_audio_info_rewrite_count;
static volatile u32 g_act18_audio_info_readback_mismatch_count;
static volatile u32 g_act18_csr_done_write_count;
static volatile u32 g_act18_ep0_info_last;
static volatile u32 g_act18_audio_info_last;
static volatile u32 g_act18_control_before_done_last;
static volatile u32 g_act18_control_after_done_last;
static volatile u32 g_act18_status_after_w1c_last;
static volatile u32 g_act18_status_after_done_last;
#endif
#if USB_MIC_ACT17_PROBE
static act17_pre_w1c_trace_t
    g_act17_pre_w1c_trace[ACT17_PRE_W1C_TRACE_CAPACITY];
static volatile u32 g_act17_pre_w1c_trace_count;
static volatile u32 g_act17_si_prepare_count;
static volatile u32 g_act17_irq_idle_primed;
static volatile u32 g_act17_irq_streaming;
static volatile u32 g_act17_idle_si_count;
static volatile u32 g_act17_start_si_count;
static volatile u32 g_act17_stop_si_count;
static volatile u32 g_act17_unexpected_si_state_count;
static volatile u32 g_act17_ep0_info_write_count;
static volatile u32 g_act17_ep0_info_readback_mismatch_count;
static volatile u32 g_act17_pre_w1c_delay_count;
static volatile u32 g_act17_ep0_info_last;
static volatile u32 g_act17_audio_info_last;
#endif
#if USB_MIC_ACT11_PROBE
static u32 g_act11_status_pending;
static u32 g_act11_pending_decision;
static u32 g_act11_status_arm_count;
static u32 g_act11_status_overwrite_count;
static u32 g_act11_status_attempt_count;
static u32 g_act11_status_deferred_count;
static u32 g_act11_status_retry_limit_count;
static u32 g_act11_ep0_cnak_write_count;
static u32 g_act11_ep0_cnak_success_count;
static u32 g_act11_ep0_nak_after_count;
static u32 g_act11_ep0_out_changed_count;
static u32 g_act11_device_control_before_done;
static u32 g_act11_device_control_after_done;
static u32 g_act11_ep0_in_control_before;
static u32 g_act11_ep0_in_control_after;
static u32 g_act11_ep0_out_control_before;
static u32 g_act11_ep0_out_control_after;
static u32 g_act11_ep0_ne_before_status;
static u32 g_act11_ep0_ne_after_status;
static u32 g_act11_audio_ne_before_status;
static u32 g_act11_audio_ne_after_status;
static u32 g_act11_audio_start_after_ack_count;
static u32 g_act11_audio_stop_after_ack_count;
static u32 g_act11_idle_ack_count;
static u32 g_act11_first_audio_token_loop;
static u32 g_act11_sequence_serial;
static u32 g_act11_csr_done_sequence;
static u32 g_act11_ep0_cnak_sequence;
static u32 g_act11_audio_activation_sequence;
static u32 g_act11_order_violation_count;
#endif
#if USB_MIC_P18_PROBE
#if !USB_MIC_P19_PROBE
static u8 g_ui_screen_vx[UI_SCREEN_VX_BYTES] __attribute__((aligned(4)));
static u8 g_ui_wave_vx[UI_WAVE_STRIPE_VX_BYTES]
    __attribute__((aligned(4)));
#else
static u16 g_ui_drawn_min_y[UI_WAVE_COLUMNS];
static u16 g_ui_drawn_max_y[UI_WAVE_COLUMNS];
static u16 g_ui_drawn_mean_y[UI_WAVE_COLUMNS];
#endif
static bda_handle_t g_ui_frame;
static bda_handle_t g_ui_draw;
static bda_handle_t g_ui_draw_owner;
static bda_handle_t g_ui_back;
static void *g_ui_draw_object;
static s16 g_wave_min[UI_WAVE_COLUMNS];
static s16 g_wave_max[UI_WAVE_COLUMNS];
static s16 g_wave_mean[UI_WAVE_COLUMNS];
static s16 g_ui_wave_min[UI_WAVE_COLUMNS];
static s16 g_ui_wave_max[UI_WAVE_COLUMNS];
static s16 g_ui_wave_mean[UI_WAVE_COLUMNS];
static u32 g_wave_generation;
static u32 g_ui_wave_generation;
static u32 g_ui_wave_column;
#if !USB_MIC_P19_PROBE
static u32 g_ui_wave_vx_width;
#endif
static u32 g_ui_last_tick;
static u32 g_ui_input_arm_tick;
static u32 g_ui_previous_escape;
static u32 g_ui_record_state;
static u32 g_ui_stream_state;
static u32 g_ui_initialized;
static u32 g_ui_open_ok;
static u32 g_ui_detached;
static u32 g_ui_touch_button_down;
static u32 g_ui_exit_reason;
static u32 g_ui_wave_frame_count;
static u32 g_ui_input_poll_count;
static u32 g_ui_raw_event_count;
static u32 g_ui_touch_down_count;
static u32 g_ui_touch_up_count;
static u32 g_ui_initial_draw_result;
static u32 g_ui_initial_copy_result;
static u32 g_ui_wave_draw_error_count;
#if USB_MIC_P19_PROBE
static u32 g_ui_color_panel;
static u32 g_ui_color_grid;
static u32 g_ui_color_wave;
static u32 g_ui_color_mean;
static u32 g_ui_color_record_on;
static u32 g_ui_color_record_off;
static u32 g_ui_color_exit;
static u32 g_ui_color_exit_pressed;
#endif
static u32 g_ui_color_stream_on;
static u32 g_ui_color_stream_off;
#endif

static const u8 *g_tx_data;
static u16 g_tx_len;
static u16 g_tx_pos;
static u16 g_expect_out_len;
static u8 g_tx_active;
static u8 g_tx_need_zlp;
static u8 g_zlp_pending;
#if USB_PCM_CDC_PROBE
static u8 g_control_out_action;
#endif
static u8 g_configured;
static u8 g_interface_alt;
static u8 g_address;

static void tiny_delay(void) {
    volatile u32 i;
    for (i = 0; i < 20000u; ++i) {
    }
}

static void delay_ticks(u32 ticks) {
    u32 start;
    start = bda_gui_tick_count_25ms_like();
    while ((bda_gui_tick_count_25ms_like() - start) < ticks) tiny_delay();
}

static void log_value(const char *label, u32 value);
static void log_stage(const char *stage);
static void log_key_value(const char *label, u32 value);
static void log_key_stage(const char *stage);
#if USB_PCM_CDC_PROBE
static void cdc_set_ep2_irq_enabled(u32 enabled) {
    u32 mask = REG32(USB_BASE + 0x418u);
    if (enabled != 0u) {
        if ((mask & AUDIO_IN_INTERRUPT) != 0u) {
            mask &= ~AUDIO_IN_INTERRUPT;
            REG32(USB_BASE + 0x418u) = mask;
            ++g_cdc_ep2_irq_enable_count;
        }
    } else if ((mask & AUDIO_IN_INTERRUPT) == 0u) {
        mask |= AUDIO_IN_INTERRUPT;
        REG32(USB_BASE + 0x418u) = mask;
        ++g_cdc_ep2_irq_disable_count;
    }
    g_cdc_ep2_irq_mask_last = REG32(USB_BASE + 0x418u);
}
#endif

#if USB_PCM_CDC_EP0_QUEUE_PROBE
static void cdc_c2_set_ep0_in_irq_enabled(u32 enabled) {
    u32 mask = REG32(USB_BASE + 0x418u);
    if (enabled != 0u) {
        if ((mask & EP0_IN_INTERRUPT) != 0u) {
            mask &= ~EP0_IN_INTERRUPT;
            REG32(USB_BASE + 0x418u) = mask;
            ++g_cdc_ep0_irq_enable_count;
        }
    } else if ((mask & EP0_IN_INTERRUPT) == 0u) {
        mask |= EP0_IN_INTERRUPT;
        REG32(USB_BASE + 0x418u) = mask;
        ++g_cdc_ep0_irq_disable_count;
    }
    g_cdc_ep0_irq_mask_last = REG32(USB_BASE + 0x418u);
}

static u32 cdc_c2_ep0_response_queued(void) {
    return g_tx_active != 0u ||
           g_tx_need_zlp != 0u ||
           g_zlp_pending != 0u;
}
#endif

#if USB_MIC_P18_PROBE
#if !USB_MIC_P19_PROBE
static void ui_write_u16_le(u8 *out, u16 value) {
    out[0] = (u8)value;
    out[1] = (u8)(value >> 8);
}

static void ui_write_u32_le(u8 *out, u32 value) {
    out[0] = (u8)value;
    out[1] = (u8)(value >> 8);
    out[2] = (u8)(value >> 16);
    out[3] = (u8)(value >> 24);
}

static u16 ui_rgb565(u32 red, u32 green, u32 blue) {
    return (u16)(
        ((red & 0xf8u) << 8) |
        ((green & 0xfcu) << 3) |
        (blue >> 3)
    );
}

static void ui_vx_put_pixel(s32 x, s32 y, u16 color) {
    u32 offset;
    if (x < 0 || x >= UI_SCREEN_WIDTH ||
        y < 0 || y >= UI_SCREEN_HEIGHT) return;
    offset = UI_VX_HEADER_SIZE +
        (u32)(y * UI_SCREEN_WIDTH + x) * 2u;
    ui_write_u16_le(g_ui_screen_vx + offset, color);
}

static void ui_vx_fill_rect(
    s32 x, s32 y, s32 width, s32 height, u16 color
) {
    s32 px;
    s32 py;
    for (py = y; py < y + height; ++py) {
        for (px = x; px < x + width; ++px) {
            ui_vx_put_pixel(px, py, color);
        }
    }
}

static void ui_init_screen_vx(void) {
    u16 background = ui_rgb565(20u, 22u, 23u);
    u16 header = ui_rgb565(244u, 241u, 232u);
    u16 accent = ui_rgb565(24u, 178u, 181u);
    u16 panel_border = ui_rgb565(87u, 94u, 95u);
    u16 panel = ui_rgb565(5u, 7u, 8u);
    u16 grid = ui_rgb565(50u, 58u, 59u);
    u16 status = ui_rgb565(34u, 38u, 39u);
    u16 exit_border = ui_rgb565(238u, 160u, 47u);
    u16 exit_fill = ui_rgb565(183u, 51u, 48u);
    u16 stream_off = ui_rgb565(238u, 160u, 47u);
    s32 y;
    s32 index;

    bda_memset(g_ui_screen_vx, 0, sizeof(g_ui_screen_vx));
    g_ui_screen_vx[0] = 'V';
    g_ui_screen_vx[1] = 'X';
    for (index = 2; index < 6; ++index) g_ui_screen_vx[index] = 0xccu;
    ui_write_u32_le(g_ui_screen_vx + 6, UI_SCREEN_WIDTH);
    ui_write_u32_le(g_ui_screen_vx + 10, UI_SCREEN_HEIGHT);
    for (index = 14; index < 20; ++index) g_ui_screen_vx[index] = 0xccu;
    for (index = 20; index < (s32)UI_VX_HEADER_SIZE; ++index) {
        g_ui_screen_vx[index] = 0xffu;
    }

    ui_vx_fill_rect(0, 0, UI_SCREEN_WIDTH, UI_SCREEN_HEIGHT, background);
    ui_vx_fill_rect(0, 0, UI_SCREEN_WIDTH, 56, header);
    ui_vx_fill_rect(0, 53, UI_SCREEN_WIDTH, 3, accent);
    ui_vx_fill_rect(12, 70, 216, 150, panel_border);
    ui_vx_fill_rect(14, 72, 212, 146, panel);
    for (y = UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT;
         y <= UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
         y += UI_WAVE_HALF_HEIGHT / 2) {
        ui_vx_fill_rect(UI_WAVE_X, y, UI_WAVE_COLUMNS, 1, grid);
    }
    ui_vx_fill_rect(12, 222, 216, 28, status);
    ui_vx_fill_rect(136, 231, 8, 8, stream_off);
    ui_vx_fill_rect(
        UI_EXIT_X - 2, UI_EXIT_Y - 2,
        UI_EXIT_WIDTH + 4, UI_EXIT_HEIGHT + 4,
        exit_border
    );
    ui_vx_fill_rect(
        UI_EXIT_X, UI_EXIT_Y, UI_EXIT_WIDTH, UI_EXIT_HEIGHT, exit_fill
    );
}
#endif

static void ui_release_draw(void) {
    bda_handle_t draw = g_ui_draw;
    if (!draw || (s32)draw == -1) {
        g_ui_draw = 0;
        g_ui_draw_owner = 0;
        return;
    }
    g_ui_draw = 0;
    g_ui_draw_owner = 0;
    bda_gui_end_draw(draw);
}

static int ui_acquire_draw(bda_handle_t owner) {
    if (g_ui_draw && g_ui_draw_owner == owner) return 1;
    ui_release_draw();
    g_ui_draw = bda_gui_current_draw(owner);
    if (!g_ui_draw || (s32)g_ui_draw == -1) {
        g_ui_draw = 0;
        return 0;
    }
    g_ui_draw_owner = owner;
    return 1;
}

static int ui_window_proc(
    bda_handle_t handle, u32 message, u32 wparam, u32 lparam
) {
    if (message == BDA_MSG_DRAW_CONTEXT_ATTACH) {
        g_ui_frame = handle;
        (void)ui_acquire_draw(handle);
        if (!g_ui_draw_object) {
            g_ui_draw_object = bda_gui_draw_object_create(7u);
        }
    } else if (message == BDA_MSG_DRAW_CONTEXT_DETACH) {
        if (!g_ui_draw_owner || g_ui_draw_owner == handle) ui_release_draw();
        g_ui_detached = 1u;
    }
    return bda_gui_default_proc(handle, message, wparam, lparam);
}

#if USB_MIC_P19_PROBE
static void ui_draw_direct_outline(
    s32 left, s32 top, s32 right, s32 bottom, u32 color
) {
    s32 x;
    s32 y;
    for (x = left; x <= right; ++x) {
        (void)bda_gui_put_pixel(g_ui_draw, x, top, color);
        (void)bda_gui_put_pixel(g_ui_draw, x, bottom, color);
    }
    for (y = top + 1; y < bottom; ++y) {
        (void)bda_gui_put_pixel(g_ui_draw, left, y, color);
        (void)bda_gui_put_pixel(g_ui_draw, right, y, color);
    }
}

static int ui_present_initial_direct(void) {
    void *old_object;
    u32 foreground;
    u32 muted;
    u32 accent;
    u32 grid;
    u32 button_fill;
    s32 x;
    s32 y;

    foreground = (u32)bda_gui_rgb(g_ui_draw, 240u, 243u, 239u);
    muted = (u32)bda_gui_rgb(g_ui_draw, 154u, 164u, 163u);
    accent = (u32)bda_gui_rgb(g_ui_draw, 30u, 210u, 213u);
    grid = (u32)bda_gui_rgb(g_ui_draw, 50u, 58u, 59u);
    button_fill = (u32)bda_gui_rgb(g_ui_draw, 90u, 31u, 35u);
    g_ui_color_panel = (u32)bda_gui_rgb(g_ui_draw, 5u, 7u, 8u);
    g_ui_color_grid = (u32)bda_gui_rgb(g_ui_draw, 50u, 58u, 59u);
    g_ui_color_wave = (u32)bda_gui_rgb(g_ui_draw, 30u, 210u, 213u);
    g_ui_color_mean = (u32)bda_gui_rgb(g_ui_draw, 246u, 196u, 65u);
    g_ui_color_record_on =
        (u32)bda_gui_rgb(g_ui_draw, 238u, 92u, 70u);
    g_ui_color_record_off =
        (u32)bda_gui_rgb(g_ui_draw, 238u, 160u, 47u);
    g_ui_color_exit = g_ui_color_record_on;
    g_ui_color_exit_pressed =
        (u32)bda_gui_rgb(g_ui_draw, 246u, 196u, 65u);
    g_ui_color_stream_on =
        (u32)bda_gui_rgb(g_ui_draw, 54u, 196u, 113u);
    g_ui_color_stream_off =
        (u32)bda_gui_rgb(g_ui_draw, 238u, 160u, 47u);

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    for (x = 0; x < UI_SCREEN_WIDTH; ++x) {
        (void)bda_gui_put_pixel(g_ui_draw, x, 49, accent);
        (void)bda_gui_put_pixel(g_ui_draw, x, 50, accent);
    }
    ui_draw_direct_outline(12, 70, 227, 207, grid);
    ui_draw_direct_outline(14, 72, 225, 205, grid);
    for (y = UI_WAVE_TOP;
         y <= UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
         y += UI_WAVE_HALF_HEIGHT / 2) {
        for (x = UI_WAVE_X; x < UI_WAVE_X + (s32)UI_WAVE_COLUMNS; x += 2) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, grid);
        }
    }
    for (x = UI_WAVE_X;
         x < UI_WAVE_X + (s32)UI_WAVE_COLUMNS;
         x += UI_WAVE_GRID_STEP) {
        for (y = UI_WAVE_TOP;
             y <= UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
             y += 2) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, grid);
        }
    }
    for (x = 13; x < 39; ++x) {
        (void)bda_gui_put_pixel(g_ui_draw, x, 70, accent);
        (void)bda_gui_put_pixel(g_ui_draw, 239 - x, 207, accent);
    }
    for (x = 12; x <= 227; ++x) {
        (void)bda_gui_put_pixel(g_ui_draw, x, 214, grid);
        (void)bda_gui_put_pixel(g_ui_draw, x, 246, grid);
    }
    ui_draw_direct_outline(
        UI_EXIT_X - 2, UI_EXIT_Y - 2,
        UI_EXIT_X + UI_EXIT_WIDTH + 1,
        UI_EXIT_Y + UI_EXIT_HEIGHT + 1,
        g_ui_color_exit
    );
    ui_draw_direct_outline(
        UI_EXIT_X, UI_EXIT_Y,
        UI_EXIT_X + UI_EXIT_WIDTH - 1,
        UI_EXIT_Y + UI_EXIT_HEIGHT - 1,
        g_ui_color_exit
    );
    for (y = UI_EXIT_Y + 3;
         y < UI_EXIT_Y + UI_EXIT_HEIGHT - 2;
         y += 4) {
        for (x = UI_EXIT_X + 3;
             x < UI_EXIT_X + UI_EXIT_WIDTH - 3;
             ++x) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, button_fill);
        }
    }
    for (y = UI_STATUS_DOT_Y;
         y < UI_STATUS_DOT_Y + UI_STATUS_DOT_SIZE;
         ++y) {
        for (x = UI_RECORD_DOT_X;
             x < UI_RECORD_DOT_X + UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(
                g_ui_draw, x, y, g_ui_color_record_off
            );
        }
        for (x = UI_LINK_DOT_X;
             x < UI_LINK_DOT_X + UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(
                g_ui_draw, x, y, g_ui_color_stream_off
            );
        }
    }
    (void)bda_gui_set_text_mode(g_ui_draw, 1u);
    (void)bda_gui_set_text_color(g_ui_draw, foreground);
    (void)bda_gui_draw_text(g_ui_draw, 44, 7, "9588 USB MICROPHONE", -1);
    (void)bda_gui_draw_text(g_ui_draw, 40, 29, "LIVE 16 KHZ / 16 BIT", -1);
    (void)bda_gui_draw_text(g_ui_draw, 31, 220, "RECORDING", -1);
    (void)bda_gui_draw_text(g_ui_draw, 150, 220, "PC LINK", -1);
    (void)bda_gui_draw_text(g_ui_draw, 76, 269, "STOP & EXIT", -1);
    (void)bda_gui_set_text_color(g_ui_draw, muted);
    (void)bda_gui_draw_text(g_ui_draw, 18, 54, "LIVE WAVEFORM", -1);
    (void)bda_gui_draw_text(g_ui_draw, 76, 302, "ESC TO EXIT", -1);
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    g_ui_initial_draw_result = 0u;
    g_ui_initial_copy_result = 0u;
    return 1;
}
#endif

static int ui_present_initial(void) {
#if USB_MIC_P19_PROBE
    return ui_present_initial_direct();
#else
    void *old_object;
    u32 title_color;
    u32 body_color;
    u32 muted_color;

    ui_init_screen_vx();
    g_ui_back = bda_gui_compatible_context_create(g_ui_draw);
    if (!g_ui_back || (s32)g_ui_back == -1) {
        g_ui_back = 0;
        return 0;
    }
    g_ui_initial_draw_result =
        (u32)bda_gui_draw_vx(g_ui_back, 0, 0, g_ui_screen_vx);
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    g_ui_initial_copy_result = (u32)bda_gui_context_copy(
        g_ui_back, 0, 0, UI_SCREEN_WIDTH, UI_SCREEN_HEIGHT,
        g_ui_draw, 0, 0, BDA_GUI_COLOR_KEY_BLACK_RGB565
    );
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    bda_gui_compatible_context_free(g_ui_back);
    g_ui_back = 0;
    if (g_ui_initial_draw_result != 0u || g_ui_initial_copy_result != 0u) {
        return 0;
    }

    title_color = (u32)bda_gui_rgb(g_ui_draw, 29u, 33u, 34u);
    body_color = (u32)bda_gui_rgb(g_ui_draw, 240u, 243u, 239u);
    muted_color = (u32)bda_gui_rgb(g_ui_draw, 154u, 164u, 163u);
    g_ui_color_stream_on = (u32)bda_gui_rgb(g_ui_draw, 54u, 196u, 113u);
    g_ui_color_stream_off = (u32)bda_gui_rgb(g_ui_draw, 238u, 160u, 47u);

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    (void)bda_gui_set_text_mode(g_ui_draw, 1u);
    (void)bda_gui_set_text_color(g_ui_draw, title_color);
    (void)bda_gui_draw_text(g_ui_draw, 58, 10, "9588 USB MIC", -1);
    (void)bda_gui_draw_text(g_ui_draw, 44, 34, "16 KHZ  16 BIT  MONO", -1);
    (void)bda_gui_set_text_color(g_ui_draw, muted_color);
    (void)bda_gui_draw_text(g_ui_draw, 18, 58, "REALTIME PCM", -1);
    (void)bda_gui_set_text_color(g_ui_draw, body_color);
    (void)bda_gui_draw_text(g_ui_draw, 20, 228, "MIC INPUT", -1);
    (void)bda_gui_draw_text(g_ui_draw, 150, 228, "PC LINK", -1);
    (void)bda_gui_draw_text(g_ui_draw, 101, 271, "EXIT", -1);
    (void)bda_gui_set_text_color(g_ui_draw, muted_color);
    (void)bda_gui_draw_text(g_ui_draw, 105, 305, "ESC", -1);
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    return 1;
#endif
}

static void ui_update_record_indicator(u32 recording) {
#if USB_MIC_P19_PROBE
    void *old_object;
    u32 color;
    s32 x;
    s32 y;

    recording = recording != 0u;
    if (recording == g_ui_record_state || !g_ui_draw) return;
    g_ui_record_state = recording;
    color = recording ? g_ui_color_record_on : g_ui_color_record_off;
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    for (y = UI_STATUS_DOT_Y;
         y < UI_STATUS_DOT_Y + UI_STATUS_DOT_SIZE;
         ++y) {
        for (x = UI_RECORD_DOT_X;
             x < UI_RECORD_DOT_X + UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, color);
        }
    }
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
#else
    (void)recording;
#endif
}

static void ui_update_exit_button(u32 pressed) {
#if USB_MIC_P19_PROBE
    void *old_object;
    u32 color;

    if (!g_ui_draw || !g_ui_draw_object) return;
    color = pressed ? g_ui_color_exit_pressed : g_ui_color_exit;
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    ui_draw_direct_outline(
        UI_EXIT_X - 2, UI_EXIT_Y - 2,
        UI_EXIT_X + UI_EXIT_WIDTH + 1,
        UI_EXIT_Y + UI_EXIT_HEIGHT + 1,
        color
    );
    ui_draw_direct_outline(
        UI_EXIT_X, UI_EXIT_Y,
        UI_EXIT_X + UI_EXIT_WIDTH - 1,
        UI_EXIT_Y + UI_EXIT_HEIGHT - 1,
        color
    );
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
#else
    (void)pressed;
#endif
}

static void ui_close(void) {
    bda_gui_message_t message;
    u32 i;

    if (g_ui_back) {
        bda_gui_compatible_context_free(g_ui_back);
        g_ui_back = 0;
    }
    if (g_ui_frame) {
        bda_memset(&message, 0, sizeof(message));
        (void)bda_gui_frame_stop(g_ui_frame);
        (void)bda_gui_frame_release(g_ui_frame);
        for (i = 0u; i < 128u && !g_ui_detached; ++i) {
            if (!bda_gui_event_pump_frame_once(&message, g_ui_frame)) break;
            tiny_delay();
        }
        ui_release_draw();
        bda_gui_close_frame(g_ui_frame);
        g_ui_frame = 0;
    } else {
        ui_release_draw();
    }
    g_ui_initialized = 0u;
}

static int ui_open(void) {
    bda_frame_desc_t descriptor;
    bda_gui_input_packet_t packet;
#if !USB_MIC_P19_PROBE
    bda_gui_raw_event_t event;
#endif
    u32 i;
    u32 now;

    bda_memset(&descriptor, 0, sizeof(descriptor));
    bda_memset(&packet, 0, sizeof(packet));
    g_ui_frame = 0;
    g_ui_draw = 0;
    g_ui_draw_owner = 0;
    g_ui_back = 0;
    g_ui_draw_object = 0;
    g_ui_initialized = 0u;
    g_ui_detached = 0u;
    g_ui_touch_button_down = 0u;
    g_ui_exit_reason = UI_EXIT_NONE;
    g_ui_record_state = 0u;
    g_ui_stream_state = 0u;
    g_ui_wave_generation = 0u;
#if USB_MIC_P19_PROBE
    g_ui_wave_column = 0u;
    for (i = 0u; i < UI_WAVE_COLUMNS; ++i) {
        g_ui_drawn_min_y[i] = UI_WAVE_CENTER_Y;
        g_ui_drawn_max_y[i] = UI_WAVE_CENTER_Y;
        g_ui_drawn_mean_y[i] = UI_WAVE_CENTER_Y;
    }
#else
    g_ui_wave_column = UI_WAVE_COLUMNS;
#endif

    descriptor.style =
#if USB_MIC_P19_PROBE
        0u;
    descriptor.title = PROBE_TITLE;
#else
        0x08000000u;
    descriptor.title = 0;
#endif
    descriptor.wndproc = ui_window_proc;
    descriptor.height = UI_SCREEN_WIDTH;
    descriptor.width = UI_SCREEN_HEIGHT;
#if USB_MIC_P19_PROBE
    descriptor.surface = 0u;
    log_stage("ui_descriptor_ready_default_surface");
    log_stage("ui_frame_register_begin");
#else
    descriptor.surface = (u32)bda_gui_draw_object_create(15u);
#endif
    g_ui_frame = bda_gui_register_frame_desc(&descriptor);
#if USB_MIC_P19_PROBE
    log_value("ui_frame_handle", (u32)g_ui_frame);
#endif
    if (!g_ui_frame || (s32)g_ui_frame == -1) {
        g_ui_frame = 0;
        return 0;
    }
#if USB_MIC_P19_PROBE
    log_stage("ui_frame_activate_begin");
#endif
    (void)bda_gui_frame_activate(g_ui_frame, 0x100u);
#if USB_MIC_P19_PROBE
    log_stage("ui_draw_acquire_begin");
#endif
    (void)ui_acquire_draw(g_ui_frame);
#if USB_MIC_P19_PROBE
    log_value("ui_draw_handle", (u32)g_ui_draw);
    log_stage("ui_draw_object_create_begin");
#endif
    if (!g_ui_draw_object) g_ui_draw_object = bda_gui_draw_object_create(7u);
#if USB_MIC_P19_PROBE
    log_value("ui_draw_object", (u32)g_ui_draw_object);
    log_stage("ui_initial_present_begin");
#endif
    if (!g_ui_draw || !g_ui_draw_object ||
        (s32)(u32)g_ui_draw_object == -1 || !ui_present_initial()) {
        ui_close();
        return 0;
    }

#if USB_MIC_P19_PROBE
    log_stage("ui_initial_present_done");
    log_stage("ui_input_packet_begin");
#else
    for (i = 0u; i < 32u; ++i) {
        event.code = -1;
        event.value = -1;
        (void)bda_gui_raw_event_fetch(&event);
        if (event.code < 0) break;
    }
#endif
    (void)bda_gui_input_packet(&packet);
#if USB_MIC_P19_PROBE
    log_stage("ui_input_packet_done");
#endif
    g_ui_previous_escape = (u32)bda_gui_input_packet_key_pressed(
        &packet, BDA_KEY_ESCAPE
    );
    now = bda_gui_tick_count_25ms_like();
    g_ui_last_tick = now;
    g_ui_input_arm_tick = now + UI_INPUT_ARM_TICKS;
    g_ui_initialized = 1u;
    g_ui_open_ok = 1u;
    return 1;
}

static void ui_analyze_pcm(const void *pcm) {
    const s16 *samples = (const s16 *)pcm;
    u32 column;
    for (column = 0u; column < UI_WAVE_COLUMNS; ++column) {
        u32 base = column * UI_WAVE_SAMPLES_COLUMN;
        u32 i;
        s32 sum = 0;
        s16 minimum = 32767;
        s16 maximum = (s16)-32768;
        for (i = 0u; i < UI_WAVE_SAMPLES_COLUMN; ++i) {
            s16 sample = samples[base + i];
            if (sample < minimum) minimum = sample;
            if (sample > maximum) maximum = sample;
            sum += sample;
        }
        g_wave_min[column] = minimum;
        g_wave_max[column] = maximum;
        g_wave_mean[column] = (s16)(sum / (s32)UI_WAVE_SAMPLES_COLUMN);
    }
    ++g_wave_generation;
}

static s32 ui_wave_y(s16 sample) {
    s32 scaled =
        ((s32)sample * UI_WAVE_HALF_HEIGHT * UI_WAVE_GAIN) / 32768;
    s32 y = UI_WAVE_CENTER_Y - scaled;
    if (y < UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT) {
        y = UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT;
    }
    if (y > UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT) {
        y = UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
    }
    return y;
}

static int ui_wave_grid_row(s32 y) {
    if (y == UI_WAVE_CENTER_Y ||
        y == UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT ||
        y == UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT ||
        y == UI_WAVE_CENTER_Y - UI_WAVE_HALF_HEIGHT / 2 ||
        y == UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT / 2) {
        return 1;
    }
    return 0;
}

#if USB_MIC_P19_PROBE
static int ui_wave_grid_column(s32 x) {
    s32 relative = x - UI_WAVE_X;
    return relative >= 0 &&
        relative < (s32)UI_WAVE_COLUMNS &&
        relative % UI_WAVE_GRID_STEP == 0;
}

static u32 ui_wave_background_color(s32 x, s32 y) {
    if ((ui_wave_grid_row(y) && ((x - UI_WAVE_X) & 1) == 0) ||
        (ui_wave_grid_column(x) && ((y - UI_WAVE_TOP) & 1) == 0)) {
        return g_ui_color_grid;
    }
    return g_ui_color_panel;
}
#endif

#if !USB_MIC_P19_PROBE
static void ui_wave_vx_put_pixel(s32 x, s32 y, u16 color) {
    u32 offset;
    if (x < 0 || x >= (s32)g_ui_wave_vx_width ||
        y < 0 || y >= UI_WAVE_HEIGHT) return;
    offset = UI_VX_HEADER_SIZE +
        (u32)(y * (s32)g_ui_wave_vx_width + x) * 2u;
    ui_write_u16_le(g_ui_wave_vx + offset, color);
}

static void ui_init_wave_stripe(u32 width) {
    u16 panel = ui_rgb565(5u, 7u, 8u);
    u16 grid = ui_rgb565(50u, 58u, 59u);
    s32 x;
    s32 y;
    s32 index;

    g_ui_wave_vx_width = width;
    bda_memset(g_ui_wave_vx, 0, sizeof(g_ui_wave_vx));
    g_ui_wave_vx[0] = 'V';
    g_ui_wave_vx[1] = 'X';
    for (index = 2; index < 6; ++index) g_ui_wave_vx[index] = 0xccu;
    ui_write_u32_le(g_ui_wave_vx + 6, width);
    ui_write_u32_le(g_ui_wave_vx + 10, UI_WAVE_HEIGHT);
    for (index = 14; index < 20; ++index) g_ui_wave_vx[index] = 0xccu;
    for (index = 20; index < (s32)UI_VX_HEADER_SIZE; ++index) {
        g_ui_wave_vx[index] = 0xffu;
    }
    for (y = 0; y < UI_WAVE_HEIGHT; ++y) {
        u16 color = ui_wave_grid_row(UI_WAVE_TOP + y) ? grid : panel;
        for (x = 0; x < (s32)width; ++x) {
            ui_wave_vx_put_pixel(x, y, color);
        }
    }
}
#endif

static void ui_copy_wave_snapshot(void) {
    u32 i;
    for (i = 0u; i < UI_WAVE_COLUMNS; ++i) {
        g_ui_wave_min[i] = g_wave_min[i];
        g_ui_wave_max[i] = g_wave_max[i];
        g_ui_wave_mean[i] = g_wave_mean[i];
    }
    g_ui_wave_generation = g_wave_generation;
#if !USB_MIC_P19_PROBE
    g_ui_wave_column = 0u;
#endif
}

static void ui_draw_wave_batch(void) {
#if USB_MIC_P19_PROBE
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

    if (!g_ui_draw || !g_ui_draw_object || g_wave_generation == 0u) return;
    if (g_ui_wave_generation != g_wave_generation) {
        ui_copy_wave_snapshot();
    }
    column = g_ui_wave_column;
    x = UI_WAVE_X + (s32)column;
    min_y = ui_wave_y(g_ui_wave_min[column]);
    max_y = ui_wave_y(g_ui_wave_max[column]);
    mean_y = ui_wave_y(g_ui_wave_mean[column]);

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    old_top = (s32)g_ui_drawn_max_y[column] - 1;
    old_bottom = (s32)g_ui_drawn_min_y[column] + 1;
    if (old_top < UI_WAVE_TOP) old_top = UI_WAVE_TOP;
    if (old_bottom > UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT) {
        old_bottom = UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
    }
    for (y = old_top; y <= old_bottom; ++y) {
        (void)bda_gui_put_pixel(
            g_ui_draw, x, y, ui_wave_background_color(x, y)
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
    if (new_top < UI_WAVE_TOP) new_top = UI_WAVE_TOP;
    if (new_bottom > UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT) {
        new_bottom = UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT;
    }
    for (y = new_top; y <= new_bottom; ++y) {
        (void)bda_gui_put_pixel(g_ui_draw, x, y, g_ui_color_wave);
    }
    for (y = mean_y - 1; y <= mean_y + 1; ++y) {
        if (y >= UI_WAVE_TOP &&
            y <= UI_WAVE_CENTER_Y + UI_WAVE_HALF_HEIGHT) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, g_ui_color_mean);
        }
    }
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    g_ui_drawn_min_y[column] = (u16)min_y;
    g_ui_drawn_max_y[column] = (u16)max_y;
    g_ui_drawn_mean_y[column] = (u16)mean_y;
    ++g_ui_wave_column;
    if (g_ui_wave_column >= UI_WAVE_COLUMNS) {
        g_ui_wave_column = 0u;
        ++g_ui_wave_frame_count;
    }
#else
    void *old_object;
    u16 wave = ui_rgb565(30u, 210u, 213u);
    u16 mean = ui_rgb565(246u, 196u, 65u);
    u32 start;
    u32 end;
    int draw_result;

    if (!g_ui_draw || !g_ui_draw_object) return;
    if (g_ui_wave_column >= UI_WAVE_COLUMNS) {
        if (g_ui_wave_generation == g_wave_generation) return;
        ui_copy_wave_snapshot();
    }
    start = g_ui_wave_column;
    end = g_ui_wave_column + UI_WAVE_COLUMNS_TICK;
    if (end > UI_WAVE_COLUMNS) end = UI_WAVE_COLUMNS;
    ui_init_wave_stripe(end - start);

    while (g_ui_wave_column < end) {
        u32 column = g_ui_wave_column;
        s32 x = (s32)(column - start);
        s32 min_y = ui_wave_y(g_ui_wave_min[column]);
        s32 max_y = ui_wave_y(g_ui_wave_max[column]);
        s32 mean_y = ui_wave_y(g_ui_wave_mean[column]);
        s32 y;

        if (max_y > min_y) {
            s32 swap = max_y;
            max_y = min_y;
            min_y = swap;
        }
        for (y = max_y; y <= min_y; ++y) {
            ui_wave_vx_put_pixel(x, y - UI_WAVE_TOP, wave);
        }
        ui_wave_vx_put_pixel(x, mean_y - UI_WAVE_TOP, mean);
        ++g_ui_wave_column;
    }

    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    draw_result = bda_gui_draw_vx(
        g_ui_draw, UI_WAVE_X + (s32)start, UI_WAVE_TOP, g_ui_wave_vx
    );
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
    if (draw_result != 0) ++g_ui_wave_draw_error_count;
    if (g_ui_wave_column >= UI_WAVE_COLUMNS) ++g_ui_wave_frame_count;
#endif
}

static void ui_update_stream_indicator(void) {
    void *old_object;
    u32 stream = g_audio_streaming != 0u;
    u32 color;
    s32 x;
    s32 y;
    if (stream == g_ui_stream_state || !g_ui_draw) return;
    g_ui_stream_state = stream;
    color = stream ? g_ui_color_stream_on : g_ui_color_stream_off;
    (void)bda_gui_draw_guard_begin();
    old_object = bda_gui_select_draw_object(g_ui_draw, g_ui_draw_object);
    for (y = UI_STATUS_DOT_Y;
         y < UI_STATUS_DOT_Y + UI_STATUS_DOT_SIZE;
         ++y) {
        for (x = UI_LINK_DOT_X;
             x < UI_LINK_DOT_X + UI_STATUS_DOT_SIZE;
             ++x) {
            (void)bda_gui_put_pixel(g_ui_draw, x, y, color);
        }
    }
    (void)bda_gui_select_draw_object(g_ui_draw, old_object);
    (void)bda_gui_draw_guard_end();
}

static int ui_exit_hit(u16 x, u16 y) {
    return x >= UI_EXIT_X && x < UI_EXIT_X + UI_EXIT_WIDTH &&
        y >= UI_EXIT_Y && y < UI_EXIT_Y + UI_EXIT_HEIGHT;
}

static void ui_poll_input(u32 now) {
    bda_gui_input_packet_t packet;
    bda_gui_raw_event_t event;
    u32 current_escape;
    u32 armed = (s32)(now - g_ui_input_arm_tick) >= 0;
    u32 drain;

    bda_memset(&packet, 0, sizeof(packet));
    (void)bda_gui_input_packet(&packet);
    current_escape = (u32)bda_gui_input_packet_key_pressed(
        &packet, BDA_KEY_ESCAPE
    );
    if (armed && current_escape && !g_ui_previous_escape) {
        g_ui_exit_reason = UI_EXIT_ESCAPE;
    }
    g_ui_previous_escape = current_escape;

    for (drain = 0u; drain < UI_RAW_EVENT_DRAIN; ++drain) {
        u16 x = 0xffffu;
        u16 y = 0xffffu;
        event.code = -1;
        event.value = -1;
        (void)bda_gui_raw_event_fetch(&event);
        if (event.code < 0) break;
        ++g_ui_raw_event_count;
        if (event.code != BDA_INPUT_EVENT_TOUCH_DOWN &&
            event.code != BDA_INPUT_EVENT_TOUCH_UP) continue;
        bda_gui_touch_position(&x, &y);
        if (!armed) {
            g_ui_touch_button_down = 0u;
            continue;
        }
        if (event.code == BDA_INPUT_EVENT_TOUCH_DOWN) {
            ++g_ui_touch_down_count;
            g_ui_touch_button_down = (u32)ui_exit_hit(x, y);
            if (g_ui_touch_button_down) ui_update_exit_button(1u);
        } else {
            u32 was_button_down = g_ui_touch_button_down;
            ++g_ui_touch_up_count;
            if (g_ui_touch_button_down && ui_exit_hit(x, y)) {
                g_ui_exit_reason = UI_EXIT_BUTTON;
            }
            g_ui_touch_button_down = 0u;
            if (was_button_down) ui_update_exit_button(0u);
        }
    }
}

static void ui_service_once(void) {
    u32 now;
    if (!g_ui_initialized || g_ui_exit_reason != UI_EXIT_NONE) return;
    now = bda_gui_tick_count_25ms_like();
    if (now == g_ui_last_tick) return;
    g_ui_last_tick = now;
    ++g_ui_input_poll_count;
    ui_poll_input(now);
    if (g_ui_exit_reason != UI_EXIT_NONE) return;
    ui_update_stream_indicator();
    ui_draw_wave_batch();
}

static void ui_arm_runtime_input(void) {
    u32 now = bda_gui_tick_count_25ms_like();
    g_ui_last_tick = now;
    g_ui_input_arm_tick = now + UI_INPUT_ARM_TICKS;
    g_ui_touch_button_down = 0u;
}
#endif

static int log_puts(const char *text) {
    int file;
    int written;
    const char *p;
    bda_size_t length;

    file = bda_fs_fopen_raw(PROBE_LOG_PATH, "ab");
    if (!bda_fs_file_is_valid(file)) return 0;
    p = text;
    length = 0;
    while (*p++ != 0) ++length;
    written = 0;
    if (length != 0u) written = bda_fs_fwrite_raw(text, 1, length, file);
    (void)bda_fs_close_raw(file);
    return length == 0u || written == (int)length;
}

static int log_reset(void) {
    int file = bda_fs_fopen_raw(PROBE_LOG_PATH, "wb");
    if (!bda_fs_file_is_valid(file)) return 0;
    (void)bda_fs_close_raw(file);
    return 1;
}

static char *log_append_text(char *out, char *end, const char *text) {
    while (*text != 0 && out < end) *out++ = *text++;
    return out;
}

static char *log_append_hex8(char *out, char *end, u32 value) {
    static const char digits[] = "0123456789ABCDEF";
    int i;
    out = log_append_text(out, end, "0x");
    for (i = 0; i < 8; ++i) {
        if (out < end) {
            *out++ = digits[(value >> (28 - i * 4)) & 0x0fu];
        }
    }
    return out;
}

static void log_write_value(const char *name, u32 value) {
    char line[96];
    char *out = line;
    char *end = line + sizeof(line) - 1u;
    out = log_append_text(out, end, PROBE_PREFIX);
    out = log_append_text(out, end, name);
    out = log_append_text(out, end, " ");
    out = log_append_hex8(out, end, value);
    out = log_append_text(out, end, "\r\n");
    *out = 0;
    (void)log_puts(line);
}

static void log_write_stage(const char *stage) {
    char line[112];
    char *out = line;
    char *end = line + sizeof(line) - 1u;
    out = log_append_text(out, end, PROBE_PREFIX "stage ");
    out = log_append_text(out, end, stage);
    out = log_append_text(out, end, "\r\n");
    *out = 0;
    (void)log_puts(line);
}

static void log_key_value(const char *name, u32 value) {
    log_write_value(name, value);
}

static void log_key_stage(const char *stage) {
    log_write_stage(stage);
}

static void log_value(const char *name, u32 value) {
#if USB_PCM_RELEASE_UI
    (void)name;
    (void)value;
#else
    log_write_value(name, value);
#endif
}

static void log_stage(const char *stage) {
#if USB_PCM_RELEASE_UI
    (void)stage;
#else
    log_write_stage(stage);
#endif
}

static int signature_ok(void) {
    const volatile u32 *stop_code;

    g_active_profile = bda_firmware_profile_detect();
    if (!g_active_profile ||
        g_active_profile->udc_kind != BDA_UDC_PCH_STYLE ||
        g_active_profile->udc_irq != UDC_IRQ_NUMBER) {
        return 0;
    }
    if (REG32(g_active_profile->usb_init) != KNL_SIG_USB_INIT) return 0;
    if (REG32(g_active_profile->irq_register) != KNL_SIG_IRQ_REG) return 0;
    stop_code = (const volatile u32 *)g_active_profile->capture_stop;
    if (stop_code[0] != 0x3c03b001u ||
        stop_code[1] != 0x34630080u) {
        return 0;
    }
#if USB_MIC_INTERRUPT_TIMED_PROBE
    if (REG32(g_active_profile->irq_unregister) !=
            KNL_IRQ_UNREGISTER_SIG0) return 0;
    if (REG32(g_active_profile->stock_usb_irq) !=
            KNL_STOCK_USB_IRQ_SIG0) return 0;
#endif
    return 1;
}

#if USB_MIC_P14_PROBE
static int capture_ready_now(void) {
    typedef int (*capture_ready_fn_t)(void);
    capture_ready_fn_t ready_fn;

    if (!g_active_profile) return 0;
    ready_fn = (capture_ready_fn_t)g_active_profile->capture_ready;
    return ready_fn() != 0;
}

static void capture_measure_preroll(void) {
    const s16 *samples = (const s16 *)g_pcm_ring[0];
    u32 i;
    u32 peak = 0u;
    u32 nonzero = 0u;

    for (i = 0u; i < PCM_BLOCK_BYTES / 2u; ++i) {
        s32 sample = samples[i];
        u32 magnitude =
            sample < 0 ? (u32)(-sample) : (u32)sample;
        if (magnitude > peak) peak = magnitude;
        if (sample != 0) ++nonzero;
    }
    g_preroll_peak = peak;
    g_preroll_nonzero_samples = nonzero;
}

static int capture_prepare(void) {
    int result;

    g_capture_open_result = (u32)BDA_AUDIO_CAPTURE_INVALID_STATE;
    g_capture_preroll_result = (u32)BDA_AUDIO_CAPTURE_INVALID_STATE;
    result = bda_audio_capture_open(&g_capture);
    g_capture_open_result = (u32)result;
    if (result != BDA_AUDIO_CAPTURE_OK) return 0;

    g_capture_started = 1u;
    result = bda_audio_capture_read(
        &g_capture,
        g_pcm_ring[0],
        PCM_BLOCK_BYTES
    );
    g_capture_preroll_result = (u32)result;
    g_capture_last_read_result = (u32)result;
    if (result != (int)PCM_BLOCK_BYTES) {
        ++g_capture_read_error_count;
        g_capture_stop_result = (u32)bda_audio_capture_stop(&g_capture);
        g_capture_started = 0u;
        return 0;
    }

    g_pcm_read_slot = 0u;
    g_pcm_write_slot = 1u;
    g_pcm_block_count = 1u;
    g_pcm_read_offset = 0u;
    g_pcm_ring_high_water = 1u;
    g_capture_read_count = 1u;
    capture_measure_preroll();
#if USB_MIC_P18_PROBE
    ui_analyze_pcm(g_pcm_ring[0]);
#endif
    return 1;
}

static void capture_drop_oldest_block(void) {
    if (g_pcm_block_count == 0u) return;
    g_pcm_read_slot ^= 1u;
    g_pcm_read_offset = 0u;
    --g_pcm_block_count;
    ++g_capture_idle_drop_count;
}

static void capture_service_once(void) {
    u32 start_tick;
    int result;

    if (g_capture_started == 0u || !capture_ready_now()) return;
    ++g_capture_ready_count;

    if (g_pcm_block_count >= PCM_RING_BLOCKS) {
        if (g_audio_streaming != 0u) {
            ++g_capture_backpressure_count;
            return;
        }
        capture_drop_oldest_block();
    }

    start_tick = bda_gui_tick_count_25ms_like();
    result = bda_audio_capture_read(
        &g_capture,
        g_pcm_ring[g_pcm_write_slot],
        PCM_BLOCK_BYTES
    );
    if (bda_gui_tick_count_25ms_like() != start_tick) {
        ++g_capture_slow_read_count;
    }
    g_capture_last_read_result = (u32)result;
    if (result != (int)PCM_BLOCK_BYTES) {
        ++g_capture_read_error_count;
        return;
    }

#if USB_MIC_P18_PROBE
    ui_analyze_pcm(g_pcm_ring[g_pcm_write_slot]);
#endif
    g_pcm_write_slot ^= 1u;
#if USB_MIC_INTERRUPT_TIMED_PROBE
    __asm__ volatile("sync" ::: "memory");
#endif
    ++g_pcm_block_count;
    ++g_capture_read_count;
    if (g_pcm_block_count > g_pcm_ring_high_water) {
        g_pcm_ring_high_water = g_pcm_block_count;
    }
#if USB_PCM_CDC_PROBE
    if (g_audio_streaming != 0u) {
        cdc_set_ep2_irq_enabled(1u);
    }
#endif
}
#endif

#if USB_MIC_DYNAMIC_CSR_PROBE
static void wait_csr_idle(void) {
    u32 spins = 0u;
    u32 busy;

    ++g_csr_wait_count;
    busy = REG32(USB_CSR_BUSY);
    while ((busy & USB_CSR_BUSY_ACTIVE) != 0u && spins < 100000u) {
        ++spins;
        busy = REG32(USB_CSR_BUSY);
    }
    if (spins > g_csr_wait_spin_max) g_csr_wait_spin_max = spins;
    g_csr_busy_last = busy;
    if ((busy & USB_CSR_BUSY_ACTIVE) != 0u) {
        ++g_csr_wait_timeout_count;
    }
}
#endif

static u32 read_endpoint_csr(u32 address) {
#if USB_MIC_DYNAMIC_CSR_PROBE
    volatile u32 dummy;

    wait_csr_idle();
    dummy = REG32(address);
    (void)dummy;
    wait_csr_idle();
#endif
    return REG32(address);
}

static void write_endpoint_csr(u32 address, u32 value) {
#if USB_MIC_DYNAMIC_CSR_PROBE
    wait_csr_idle();
#endif
    REG32(address) = value;
#if USB_MIC_DYNAMIC_CSR_PROBE
    ++g_csr_write_count;
    wait_csr_idle();
#endif
}

#if USB_MIC_ACT9_PROBE
static void act9_hard_reset_dynamic_core(void) {
    u32 attempt;

    g_act9_devcfg_before_reset = REG32(USB_DEVICE_CONFIG);
    g_act9_devcfg_write_value =
        0x00000017u | USB_DEVICE_CONFIG_CSR_PRG;
    g_act9_devcfg_readback = 0u;

    /*
     * Historical A9 test borrowed the CS5536 DEVCFG.SOFTRST definition.
     * JZ4730 does not document this bit; the failed readback is retained.
     */
    for (attempt = 0u; attempt < 2u; ++attempt) {
        REG32(USB_CONTROL) = 0x00000070u;
        REG32(USB_DEVICE_CONFIG) = USB_DEVICE_CONFIG_SOFT_RESET;
        ++g_act9_hard_reset_count;
        delay_ticks(1u);
        g_act9_devcfg_after_reset = REG32(USB_DEVICE_CONFIG);

        REG32(USB_DEVICE_CONFIG) = g_act9_devcfg_write_value;
        tiny_delay();
        g_act9_devcfg_readback = REG32(USB_DEVICE_CONFIG);
        if ((g_act9_devcfg_readback &
             USB_DEVICE_CONFIG_CSR_PRG) != 0u) {
            break;
        }
    }
    g_act9_dynamic_ready =
        (g_act9_devcfg_readback & USB_DEVICE_CONFIG_CSR_PRG) != 0u;
}

static void act9_reset_dynamic_endpoint_map(void) {
    u32 address;
    u32 i;

    if (g_act9_dynamic_ready == 0u) return;

    /*
     * Historical A9 treated the JZ4730 static endpoint information table as
     * a CS5536 dynamic table. Keep the code only for artifact reproducibility.
     */
    for (i = 0u; i < 9u; ++i) {
        address = USB_BASE + 0x500u + i * 4u;
        write_endpoint_csr(address, 0u);
        ++g_act9_epreg_clear_write_count;
    }
    for (i = 0u; i < 9u; ++i) {
        address = USB_BASE + 0x500u + i * 4u;
        if (read_endpoint_csr(address) != 0u) {
            ++g_act9_epreg_nonzero_after_clear;
        }
    }

    write_endpoint_csr(USB_EP0_OUT_NE, 0x01000000u);
    g_act9_ep0_ne_after_reset = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act9_audio_ne_after_reset = read_endpoint_csr(USB_AUDIO_IN_NE);
    ++g_act9_epreg_reset_count;
}
#endif

#if USB_MIC_SOFT_RESET_CSR_PROBE
static void reset_usb_for_dynamic_csr(void) {
    g_soft_reset_before = REG32(USB_SOFT_RESET);
    REG32(USB_SOFT_RESET) = USB_SOFT_RESET_CORE;
    REG32(USB_SOFT_RESET) =
        USB_SOFT_RESET_CORE | USB_SOFT_RESET_PHY;
    delay_ticks(1u);
    REG32(USB_SOFT_RESET) = USB_SOFT_RESET_CORE;
    REG32(USB_SOFT_RESET) = 0u;
    delay_ticks(1u);
    g_soft_reset_after = REG32(USB_SOFT_RESET);
    ++g_soft_reset_count;
}
#endif

static void init_usb_core_group(void) {
#if USB_MIC_ACT13_PROBE
    REG32(USB_DEVICE_CONFIG) = 0x00000017u;
    g_act13_device_config_after_init = REG32(USB_DEVICE_CONFIG);
#elif USB_MIC_ACT10_PROBE
    REG32(USB_DEVICE_CONFIG) = 0x00000017u;
    g_a10_device_config_after_init = REG32(USB_DEVICE_CONFIG);
#elif USB_MIC_ACT9_PROBE
    act9_hard_reset_dynamic_core();
    g_act1_device_config_after_init = g_act9_devcfg_readback;
    g_device_config_after_init = g_act9_devcfg_readback;
#elif USB_MIC_STATIC_CSR_ACT_PROBE
    g_act7_device_config_write_value = 0x00000017u;
    REG32(USB_DEVICE_CONFIG) = g_act7_device_config_write_value;
    g_act7_device_config_readback = REG32(USB_DEVICE_CONFIG);
    g_act1_device_config_after_init = g_act7_device_config_readback;
#elif USB_MIC_ACTIVATION_PROBE
    REG32(USB_DEVICE_CONFIG) =
        0x00000017u | USB_DEVICE_CONFIG_CSR_PRG;
    g_act1_device_config_after_init = REG32(USB_DEVICE_CONFIG);
#if USB_MIC_DYNAMIC_CSR_PROBE
    g_device_config_after_init = g_act1_device_config_after_init;
#endif
#elif USB_MIC_SOFT_RESET_CSR_PROBE
    reset_usb_for_dynamic_csr();
    g_device_config_before_write = REG32(USB_DEVICE_CONFIG);
    g_device_config_write_value =
        g_device_config_before_write |
        0x00000017u |
        USB_DEVICE_CONFIG_CSR_PRG;
    REG32(USB_DEVICE_CONFIG) = g_device_config_write_value;
    g_device_config_after_init = REG32(USB_DEVICE_CONFIG);
#elif USB_MIC_DYNAMIC_CSR_PROBE
    REG32(USB_DEVICE_CONFIG) =
        0x00000017u | USB_DEVICE_CONFIG_CSR_PRG;
    g_device_config_after_init = REG32(USB_DEVICE_CONFIG);
#else
    REG32(USB_DEVICE_CONFIG) = 0x00000017u;
#endif
    REG32(USB_BASE + 0x404u) = 0x00000000u;
#if USB_MIC_ACT13_PROBE
    /* JZ4730 static CSR mode handles SET_INTERFACE while SI is masked. */
    REG32(USB_BASE + 0x410u) = 0x00000072u;
#elif USB_PCM_CDC_EP0_QUEUE_PROBE
    /* Match jz4730_udc.c: mask SOF, suspend and SI; enable reset and SC. */
    REG32(USB_BASE + 0x410u) = INTERRUPT_TIMED_DEVICE_MASK;
#else
    REG32(USB_BASE + 0x410u) = 0x00000070u;
#endif
#if USB_MIC_EP0_IRQ_PROBE
    /*
     * EPIntMR uses one bits for masked sources. The profile-specific mask
     * keeps EP0 IN/OUT and the selected data IN endpoint enabled.
     */
    REG32(USB_BASE + 0x418u) = INTERRUPT_TIMED_EP_MASK;
#elif USB_MIC_ACT14_PROBE
    /*
     * EP0 stays on the proven polling path. Only EP4 IN can interrupt the
     * main loop once one-millisecond isochronous tokens begin.
     */
    REG32(USB_BASE + 0x418u) = ACT14_AUDIO_ONLY_EP_MASK;
#else
    REG32(USB_BASE + 0x418u) = 0x00000000u;
#endif
    REG32(USB_BASE + 0x000u) = 0x00000002u;
    REG32(USB_BASE + 0x000u) = 0x00000000u;
#if USB_PCM_CDC_PROBE
    /* The JZ4730 BSP uses only type and FIFO-flush bits in EP control. */
    REG32(USB_BASE + 0x020u) = 0x00000032u;
    REG32(USB_BASE + 0x020u) = 0x00000030u;
#elif USB_PCM_HID_PROBE
    REG32(USB_BASE + 0x020u) = 0x00000032u;
    REG32(USB_BASE + 0x020u) = 0x00000030u;
#elif USB_MIC_SILENCE_PROBE && !USB_MIC_EP4_PROBE
    REG32(USB_BASE + 0x020u) = 0x00000012u;
    REG32(USB_BASE + 0x020u) = 0x00000010u;
#else
    REG32(USB_BASE + 0x020u) = 0x00000032u;
    REG32(USB_BASE + 0x020u) = 0x00000030u;
#endif
#if USB_PCM_CDC_PROBE
    REG32(USB_BASE + 0x040u) = 0x00000022u;
    REG32(USB_BASE + 0x040u) = 0x00000020u;
#elif USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || USB_MIC_ACT13_PROBE
    REG32(USB_BASE + 0x040u) = 0x00000022u;
    REG32(USB_BASE + 0x040u) = 0x00000020u;
#elif USB_MIC_ACTIVATION_PROBE
    REG32(USB_BASE + 0x040u) =
        USB_EP_CONTROL_TYPE_ISO |
        USB_EP_CONTROL_SNAK |
        USB_EP_CONTROL_FLUSH;
    REG32(USB_BASE + 0x040u) =
        USB_EP_CONTROL_TYPE_ISO | USB_EP_CONTROL_SNAK;
#elif USB_MIC_PROVEN_IN_PROBE
    REG32(USB_BASE + 0x040u) = 0x00000012u;
    REG32(USB_BASE + 0x040u) = 0x00000010u;
#else
    REG32(USB_BASE + 0x040u) = 0x00000022u;
    REG32(USB_BASE + 0x040u) = 0x00000020u;
#endif
    REG32(USB_BASE + 0x060u) = 0x00000022u;
    REG32(USB_BASE + 0x060u) = 0x00000020u;
    REG32(USB_BASE + 0x080u) = 0x00000012u;
    REG32(USB_BASE + 0x080u) = 0x00000010u;
}

static void init_usb_endpoint_group(void) {
    REG32(USB_BASE + 0x200u) = 0x00000000u;
#if USB_PCM_CDC_PROBE
    REG32(USB_BASE + 0x2A0u) = 0x00000020u;
#else
    REG32(USB_BASE + 0x2A0u) = 0x00000020u;
#endif
    REG32(USB_BASE + 0x2C0u) = 0x00000020u;
    REG32(USB_BASE + 0x2E0u) = 0x00000010u;
    REG32(USB_BASE + 0x008u) = 0x00000008u;
#if USB_PCM_HID_PROBE
    REG32(USB_BASE + 0x028u) = 0x00000010u;
#elif USB_MIC_SILENCE_PROBE && !USB_MIC_EP4_PROBE
    REG32(USB_BASE + 0x028u) = 0x00000008u;
#else
    REG32(USB_BASE + 0x028u) = 0x00000010u;
#endif
    REG32(USB_BASE + 0x048u) = 0x00000010u;
    REG32(USB_BASE + 0x068u) = 0x00000010u;
#if USB_MIC_P13_PROBE
    /* 32-byte max packet with the controller's required double buffering. */
    REG32(USB_BASE + 0x088u) = 0x00000010u;
#elif USB_MIC_EP4_PROBE
    REG32(USB_BASE + 0x088u) = 0x00000008u;
#else
    REG32(USB_BASE + 0x088u) = 0x00000010u;
#endif
    REG32(USB_BASE + 0x00Cu) = 0x00000020u;
#if USB_PCM_HID_PROBE
    REG32(USB_BASE + 0x02Cu) = HID_REPORT_BYTES;
#elif USB_MIC_SILENCE_PROBE && !USB_MIC_EP4_PROBE
    REG32(USB_BASE + 0x02Cu) = 0x00000020u;
#else
    REG32(USB_BASE + 0x02Cu) = 0x00000040u;
#endif
#if USB_MIC_PROVEN_IN_PROBE && !USB_MIC_ACT13_PROBE
    REG32(USB_BASE + 0x04Cu) = 0x00000020u;
#else
    REG32(USB_BASE + 0x04Cu) = 0x00000040u;
#endif
    REG32(USB_BASE + 0x06Cu) = 0x00000040u;
#if USB_PCM_CDC_PROBE
    REG32(USB_BASE + 0x08Cu) = 0x00000040u;
#elif USB_MIC_EP4_PROBE
    REG32(USB_BASE + 0x08Cu) = 0x00000020u;
#else
    REG32(USB_BASE + 0x08Cu) = 0x00000040u;
#endif
    REG32(USB_BASE + 0x20Cu) = 0x00000020u;
    REG32(USB_BASE + 0x2ACu) = 0x00000040u;
    REG32(USB_BASE + 0x2CCu) = 0x00000040u;
    REG32(USB_BASE + 0x2ECu) = 0x00000040u;
}

static void init_usb_dma_group(void) {
#if USB_MIC_ACT9_PROBE
    act9_reset_dynamic_endpoint_map();
#else
    write_endpoint_csr(USB_BASE + 0x500u, 0x0000FFFFu);
    write_endpoint_csr(USB_BASE + 0x504u, 0x01000080u);
#if USB_PCM_CDC_PROBE
    write_endpoint_csr(USB_BASE + 0x508u, USB_CDC_NOTIFY_NE_VALUE);
#elif USB_PCM_HID_PROBE
    write_endpoint_csr(USB_BASE + 0x508u, USB_AUDIO_IN_NE_VALUE);
#elif USB_MIC_P25_PROBE
    write_endpoint_csr(USB_BASE + 0x508u, USB_AUDIO_IN_NE_VALUE);
#else
    write_endpoint_csr(USB_BASE + 0x508u, 0x020000F1u);
#endif
#if USB_PCM_CDC_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_AUDIO_IN_NE_VALUE);
#elif USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || USB_MIC_ACT13_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, 0x020000D2u);
#elif USB_MIC_ACT10_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_AUDIO_IN_NE_VALUE);
#elif USB_MIC_ACT8_PROBE
    write_endpoint_csr(USB_EP1_OUT_NE, 0x020000D2u);
#elif USB_MIC_ACT2_PROBE || USB_MIC_ACT3_PROBE || \
    USB_MIC_ACT4_PROBE || USB_MIC_ACT5_PROBE || USB_MIC_ACT7_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_ACT1_AUDIO_NE_ALT1);
#elif USB_MIC_ACT1_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_ACT1_AUDIO_NE_RESET);
#elif USB_MIC_P26_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_AUDIO_IN_NE_VALUE);
#elif USB_MIC_P24_PROBE || USB_MIC_P25_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, 0x020000D2u);
#elif USB_MIC_PROVEN_IN_PROBE
    write_endpoint_csr(USB_BASE + 0x50Cu, USB_AUDIO_IN_NE_VALUE);
#else
    write_endpoint_csr(USB_BASE + 0x50Cu, 0x020000D2u);
#endif
#if (USB_MIC_P24_PROBE && !USB_MIC_P25_PROBE) || USB_MIC_ACT8_PROBE
    write_endpoint_csr(USB_BASE + 0x510u, USB_AUDIO_IN_NE_VALUE);
#else
    write_endpoint_csr(USB_BASE + 0x510u, 0x020000D3u);
#endif
#if USB_PCM_CDC_PROBE
    write_endpoint_csr(USB_BASE + 0x514u, 0x020000B4u);
#elif USB_MIC_ACT13_PROBE
    write_endpoint_csr(USB_BASE + 0x514u, USB_AUDIO_IN_NE_VALUE);
#elif USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE
    write_endpoint_csr(USB_BASE + 0x514u, USB_ACT1_AUDIO_NE_ALT1);
#elif USB_MIC_PROVEN_IN_PROBE
    write_endpoint_csr(USB_BASE + 0x514u, 0x020000B4u);
#elif USB_MIC_P13_PROBE
    /* EP4 IN, ISO, config 1, interface 1, alt 1, max packet 32. */
    write_endpoint_csr(USB_BASE + 0x514u, 0x010088B4u);
#else
    write_endpoint_csr(USB_BASE + 0x514u, 0x020000B4u);
#endif
#if USB_PCM_CDC_PROBE
    write_endpoint_csr(USB_BASE + 0x518u, USB_CDC_OUT_NE_VALUE);
#else
    write_endpoint_csr(USB_BASE + 0x518u, 0x020000C5u);
#endif
    write_endpoint_csr(USB_BASE + 0x51Cu, 0x020000C6u);
    write_endpoint_csr(USB_BASE + 0x520u, 0x020000A7u);
#endif
#if USB_MIC_ACTIVATION_PROBE
    g_act1_initial_control = REG32(USB_AUDIO_IN_CONTROL);
    g_act1_initial_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act1_initial_buffer = REG32(USB_AUDIO_IN_BUFFER);
    g_act1_initial_max_packet = REG32(USB_AUDIO_IN_MAX_PKT);
#endif
#if USB_MIC_STATIC_CSR_ACT_PROBE
    g_act7_ep0_ne_initial = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act7_ep0_ne_last = g_act7_ep0_ne_initial;
    g_act7_audio_ne_initial = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act7_audio_ne_last = g_act7_audio_ne_initial;
#endif
#if USB_MIC_ACT8_PROBE
    g_act8_ep1_out_slot_initial = read_endpoint_csr(USB_EP1_OUT_NE);
    g_act8_ep1_out_slot_last = g_act8_ep1_out_slot_initial;
    g_act8_ep2_in_slot_initial = read_endpoint_csr(USB_EP2_IN_NE);
    g_act8_ep2_in_slot_last = g_act8_ep2_in_slot_initial;
#endif
#if USB_MIC_ACT10_PROBE
    g_a10_initial_control = REG32(USB_AUDIO_IN_CONTROL);
    g_a10_initial_csr = REG32(USB_AUDIO_IN_CSR);
    g_a10_initial_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_a10_initial_buffer = REG32(USB_AUDIO_IN_BUFFER);
    g_a10_initial_max_packet = REG32(USB_AUDIO_IN_MAX_PKT);
    g_a10_control_last = g_a10_initial_control;
    g_a10_csr_last = g_a10_initial_csr;
    g_a10_ne_last = g_a10_initial_ne;
#endif
#if USB_MIC_ACT13_PROBE
    g_act13_initial_control = REG32(USB_AUDIO_IN_CONTROL);
    g_act13_initial_csr = REG32(USB_AUDIO_IN_CSR);
    g_act13_initial_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act13_initial_buffer = REG32(USB_AUDIO_IN_BUFFER);
    g_act13_initial_max_packet = REG32(USB_AUDIO_IN_MAX_PKT);
    g_act13_control_last = g_act13_initial_control;
    g_act13_csr_last = g_act13_initial_csr;
    g_act13_ne_last = g_act13_initial_ne;
#endif
}

#if USB_PCM_CDC_PROBE
static void cdc_rearm_after_bus_reset(void) {
    REG32(USB_EP0_IN_CSR) = 0u;
    REG32(USB_EP0_OUT_CSR) = 0u;
    REG32(USB_CDC_NOTIFY_CSR) = 0u;
    REG32(USB_AUDIO_IN_CSR) = 0u;
    REG32(USB_CDC_OUT_CSR) = 0u;

    init_usb_endpoint_group();
    init_usb_dma_group();

    REG32(USB_BASE + 0x000u) = 0x00000002u;
    REG32(USB_BASE + 0x000u) = 0x00000000u;
    REG32(USB_CDC_NOTIFY_CONTROL) = 0x00000032u;
    REG32(USB_CDC_NOTIFY_CONTROL) = 0x00000030u;
    REG32(USB_AUDIO_IN_CONTROL) = 0x00000022u;
    REG32(USB_AUDIO_IN_CONTROL) = 0x00000020u;
    REG32(USB_CDC_OUT_CONTROL) = 0x00000020u;
    g_audio_streaming = 0u;
    REG32(USB_BASE + 0x418u) =
        INTERRUPT_TIMED_EP_MASK | AUDIO_IN_INTERRUPT;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    REG32(USB_CONTROL) = INTERRUPT_TIMED_DEVICE_MASK;
    g_cdc_ep0_irq_mask_last = REG32(USB_BASE + 0x418u);
#endif
#if USB_PCM_CDC_C3_PROBE
    g_cdc_c3_bus_setup_base = g_setup_count;
    g_cdc_c3_bus_sc_base = g_act14_sc_ack_count;
    g_cdc_c3_wait_armed = 0u;
    g_cdc_c3_fallback_done_on_bus = 0u;
#endif
    g_cdc_ep2_irq_mask_last = REG32(USB_BASE + 0x418u);
    ++g_cdc_bus_reset_rearm_count;
}

static void cdc_apply_hardware_configuration(u32 status) {
    u8 configured = (u8)(status & 0x0fu);
    g_configured = configured;
    g_interface_alt = 0u;
    g_audio_streaming = configured == 1u;
    cdc_set_ep2_irq_enabled(
        g_audio_streaming != 0u && g_pcm_block_count != 0u
    );
}

#if USB_PCM_CDC_C3_PROBE
static void cdc_c3_service_config_fallback(void) {
    u32 now;
    u32 status;

    if (g_cdc_c3_fallback_done_on_bus != 0u) return;
    if (g_act14_sc_ack_count != g_cdc_c3_bus_sc_base) {
        if (g_cdc_c3_wait_armed != 0u) {
            ++g_cdc_c3_wait_cancelled_by_sc_count;
        }
        g_cdc_c3_wait_armed = 0u;
        g_cdc_c3_fallback_done_on_bus = 1u;
        return;
    }
    if ((g_setup_count - g_cdc_c3_bus_setup_base) < 5u) return;

    status = REG32(USB_DEVICE_STATUS);
    if ((status & 0x0fu) != 1u) return;
    ++g_cdc_c3_cfg1_poll_count;

    now = bda_gui_tick_count_25ms_like();
    if (g_cdc_c3_wait_armed == 0u) {
        g_cdc_c3_wait_armed = 1u;
        ++g_cdc_c3_wait_armed_count;
        g_cdc_c3_wait_start_tick = now;
        g_cdc_c3_wait_start_loop = g_loop_count;
        return;
    }
    if ((now - g_cdc_c3_wait_start_tick) <
        CDC_C3_CONFIG_FALLBACK_DELAY_TICKS) {
        return;
    }
    if (g_act14_sc_ack_count != g_cdc_c3_bus_sc_base) {
        ++g_cdc_c3_wait_cancelled_by_sc_count;
        g_cdc_c3_wait_armed = 0u;
        g_cdc_c3_fallback_done_on_bus = 1u;
        return;
    }

    g_cdc_c3_wait_armed = 0u;
    g_cdc_c3_fallback_done_on_bus = 1u;
    ++g_cdc_c3_fallback_trigger_count;
    g_cdc_c3_fallback_last_status = status;
    g_cdc_c3_fallback_last_loop = g_loop_count;
    g_cdc_c3_fallback_last_setup_count = g_setup_count;
    g_cdc_c3_fallback_last_intr_before = REG32(USB_INTRUSB);

    /*
     * JZ4730 consumes SET_CONFIGURATION in hardware. If DevSR reached
     * configuration 1 without an observable SC IRQ, finish the same work
     * as the BSP's psudo_set_config() path and acknowledge only SC.
     */
    cdc_apply_hardware_configuration(status);
    tiny_delay();
    REG32(USB_INTRUSB) = USB_INTR_SERVICE_CONFIG;
    __asm__ volatile("sync" ::: "memory");
    g_cdc_c3_fallback_last_intr_after = REG32(USB_INTRUSB);
}
#endif
#endif

static void connect_usb_direct(void) {
    REG32(SYS_USB_PHY) = g_original_phy | 0x20000000u;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    REG32(USB_CONTROL) = INTERRUPT_TIMED_DEVICE_MASK;
#else
    REG32(USB_CONTROL) = 0x00000060u;
#endif
}

static void disconnect_usb_direct(void) {
    REG32(USB_CONTROL) = 0x00000070u;
    REG32(SYS_USB_PHY) = g_original_phy;
}

#if USB_MIC_ACTIVATION_PROBE
static void act1_snak_and_flush_audio_endpoint(void) {
    u32 control;

    control = REG32(USB_AUDIO_IN_CONTROL);
    control &= ~(USB_EP_CONTROL_TYPE_MASK |
                 USB_EP_CONTROL_FLUSH |
                 USB_EP_CONTROL_CNAK);
    control |= USB_EP_CONTROL_TYPE_ISO | USB_EP_CONTROL_SNAK;
    REG32(USB_AUDIO_IN_CONTROL) = control;
    REG32(USB_AUDIO_IN_CONTROL) = control | USB_EP_CONTROL_FLUSH;
    control = REG32(USB_AUDIO_IN_CONTROL);
    if ((control & USB_EP_CONTROL_FLUSH) != 0u) {
        REG32(USB_AUDIO_IN_CONTROL) =
            control & ~USB_EP_CONTROL_FLUSH;
    }
}

static void act1_clear_audio_pending(void) {
    u32 status = REG32(USB_AUDIO_IN_CSR);
    u32 pending = REG32(USB_EP_INTR);

    if ((status & EP_CSR_TOKEN) != 0u) {
        REG32(USB_AUDIO_IN_CSR) = status & ~EP_CSR_TOKEN;
    }
    if ((pending & AUDIO_IN_INTERRUPT) != 0u) {
        REG32(USB_EP_INTR) = AUDIO_IN_INTERRUPT;
    }
}

#if USB_MIC_STATIC_CSR_ACT_PROBE
static void act7_prearm_static_audio_endpoint(void) {
    u32 attempt;
    u32 control;

    ++g_act7_prearm_count;
    control = REG32(USB_AUDIO_IN_CONTROL);
    g_act7_audio_control_before_prearm = control;
    for (attempt = 0u; attempt < 4u; ++attempt) {
        REG32(USB_AUDIO_IN_CONTROL) =
            (control & ~(USB_EP_CONTROL_STALL |
                         USB_EP_CONTROL_SNAK |
                         USB_EP_CONTROL_FLUSH)) |
            USB_EP_CONTROL_TYPE_ISO |
            USB_EP_CONTROL_CNAK;
        ++g_act7_prearm_cnak_write_count;
        control = REG32(USB_AUDIO_IN_CONTROL);
        if ((control & USB_EP_CONTROL_NAK) == 0u) break;
        if (attempt != 3u) tiny_delay();
    }
    if ((control & USB_EP_CONTROL_NAK) != 0u) {
        ++g_act7_prearm_nak_after_count;
    }
    g_act7_audio_control_after_prearm = control;
}
#endif

static void act1_reset_audio_endpoint(void) {
    act1_snak_and_flush_audio_endpoint();
    REG32(USB_AUDIO_IN_BUFFER) = 0x00000010u;
    REG32(USB_AUDIO_IN_MAX_PKT) = EP1_PACKET_BYTES;
#if USB_MIC_ACT9_PROBE
    write_endpoint_csr(USB_AUDIO_IN_NE, 0u);
#elif USB_MIC_ACT2_PROBE || USB_MIC_ACT3_PROBE || USB_MIC_ACT4_PROBE || \
    USB_MIC_ACT5_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    write_endpoint_csr(USB_AUDIO_IN_NE, USB_ACT1_AUDIO_NE_ALT1);
#else
    write_endpoint_csr(USB_AUDIO_IN_NE, USB_ACT1_AUDIO_NE_RESET);
#endif
    act1_clear_audio_pending();
#if USB_MIC_STATIC_CSR_ACT_PROBE
    /*
     * Static CSR matching keeps this endpoint inactive until interface 1,
     * alt 1 is selected. Clear NAK after every bus reset so the first host
     * IN token can arrive without touching the SET_INTERFACE status path.
     */
    act7_prearm_static_audio_endpoint();
#endif
    g_audio_streaming = 0u;
    g_act1_fallback_armed = 0u;
    g_act1_activation_reason = 0u;
    ++g_act1_idle_reset_count;
}

static void act1_update_audio_ne(
    u32 intrusb,
    u8 configured,
    u8 interface_number,
    u8 interface_alt
) {
#if USB_MIC_ACT9_PROBE || USB_MIC_ACT2_PROBE || USB_MIC_ACT3_PROBE || \
    USB_MIC_ACT4_PROBE || \
    USB_MIC_ACT5_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    (void)intrusb;
    (void)configured;
    (void)interface_number;
    (void)interface_alt;
    g_act1_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#else
    u32 audio_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
    u32 effective_alt = interface_alt;

    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        audio_ne =
            (audio_ne & ~USB_CSR_NE_CFG_MASK) |
            ((u32)configured << USB_CSR_NE_CFG_SHIFT);
    }
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
        if ((intrusb & USB_INTR_SERVICE_CONFIG) == 0u &&
            interface_alt == 0u &&
            g_act1_fallback_armed != 0u &&
            g_audio_streaming != 0u) {
            effective_alt = AUDIO_STREAMING_ALT;
        }
        audio_ne =
            (audio_ne & ~(USB_CSR_NE_INTF_MASK | USB_CSR_NE_ALT_MASK)) |
            ((u32)interface_number << USB_CSR_NE_INTF_SHIFT) |
            (effective_alt << USB_CSR_NE_ALT_SHIFT);
    }
    write_endpoint_csr(USB_AUDIO_IN_NE, audio_ne);
    g_act1_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#endif
}

#if USB_MIC_ACT4_PROBE || USB_MIC_ACT5_PROBE || USB_MIC_ACT9_PROBE
static void act4_clear_stall_and_nak(u32 control_address) {
    u32 control = REG32(control_address);
    control &= ~USB_EP_CONTROL_STALL;
    REG32(control_address) = control | USB_EP_CONTROL_CNAK;
    ++g_act4_clear_stall_nak_count;
}

static void act4_prepare_set_interface(u8 effective_alt) {
    u32 ep0_out_ne;

    g_act4_effective_alt_last = effective_alt;
    ep0_out_ne = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act4_ep0_out_ne_before = ep0_out_ne;
    ep0_out_ne =
        (ep0_out_ne &
         ~(USB_CSR_NE_CFG_MASK |
           USB_CSR_NE_INTF_MASK |
           USB_CSR_NE_ALT_MASK)) |
        (1u << USB_CSR_NE_CFG_SHIFT) |
        (1u << USB_CSR_NE_INTF_SHIFT) |
        ((u32)effective_alt << USB_CSR_NE_ALT_SHIFT);
    write_endpoint_csr(USB_EP0_OUT_NE, ep0_out_ne);
    g_act4_ep0_out_ne_after = read_endpoint_csr(USB_EP0_OUT_NE);

    g_act4_ep0_in_control_before = REG32(USB_BASE + 0x000u);
    act4_clear_stall_and_nak(USB_BASE + 0x000u);
    act4_clear_stall_and_nak(USB_BASE + 0x200u);
#if USB_MIC_ACT9_PROBE
    /* Audio endpoint state is changed explicitly after the requested alt is known. */
#elif USB_MIC_ACT6_PROBE
    /*
     * EP2 stays quiescent until the controller has accepted CSR_DONE.
     * A fresh bus reset leaves the unused endpoint controls unstalled.
     */
    g_act6_audio_control_before_done = REG32(USB_AUDIO_IN_CONTROL);
    ++g_act6_audio_preack_skip_count;
#else
    act4_clear_stall_and_nak(USB_BASE + 0x020u);
    act4_clear_stall_and_nak(USB_BASE + 0x2A0u);
    act4_clear_stall_and_nak(USB_BASE + 0x040u);
    act4_clear_stall_and_nak(USB_BASE + 0x2C0u);
    act4_clear_stall_and_nak(USB_BASE + 0x060u);
    act4_clear_stall_and_nak(USB_BASE + 0x2E0u);
#endif
    g_act4_ep0_in_control_after = REG32(USB_BASE + 0x000u);
}
#endif

#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
static void act5_record_si_trace(
    u32 status,
    u32 decision,
    u8 effective_alt
) {
    act5_si_trace_t *slot;

    if (g_act5_si_trace_count >= 8u) return;
    slot = &g_act5_si_trace[g_act5_si_trace_count++];
    slot->ordinal = g_act1_standalone_si_count;
    slot->loop_count = g_loop_count;
    slot->device_status = status;
    slot->ep_intr = REG32(USB_EP_INTR);
    slot->decision = decision;
    slot->effective_alt = effective_alt;
    slot->ep0_out_ne = read_endpoint_csr(USB_EP0_OUT_NE);
    slot->audio_control = REG32(USB_AUDIO_IN_CONTROL);
}
#endif

static void act1_activate_audio_endpoint(u32 reason) {
    u32 control;
    u32 attempt;

    g_act1_ne_before_activation = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act1_control_before_activation = REG32(USB_AUDIO_IN_CONTROL);

    act1_snak_and_flush_audio_endpoint();
    REG32(USB_AUDIO_IN_BUFFER) = 0x00000010u;
    REG32(USB_AUDIO_IN_MAX_PKT) = EP1_PACKET_BYTES;
    write_endpoint_csr(USB_AUDIO_IN_NE, USB_ACT1_AUDIO_NE_ALT1);
    act1_clear_audio_pending();

    control = REG32(USB_AUDIO_IN_CONTROL);
    for (attempt = 0u; attempt < 4u; ++attempt) {
        REG32(USB_AUDIO_IN_CONTROL) =
            (control & ~(USB_EP_CONTROL_SNAK |
                         USB_EP_CONTROL_FLUSH)) |
            USB_EP_CONTROL_CNAK;
        ++g_act1_cnak_write_count;
        control = REG32(USB_AUDIO_IN_CONTROL);
        if ((control & USB_EP_CONTROL_NAK) == 0u) break;
        if (attempt != 3u) {
            ++g_act1_cnak_retry_count;
            tiny_delay();
        }
    }
    if ((control & USB_EP_CONTROL_NAK) != 0u) {
        ++g_act1_nak_after_cnak_count;
    }

    g_audio_streaming = 1u;
    g_act1_activation_reason = reason;
    g_act1_fallback_armed =
        reason == USB_ACT1_REASON_ALT0_FALLBACK;
    ++g_act1_activation_count;
    if (reason == USB_ACT1_REASON_NORMAL) {
        ++g_act1_normal_activation_count;
    } else {
        ++g_act1_fallback_activation_count;
    }
    g_act1_ne_after_activation = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act1_control_after_activation = REG32(USB_AUDIO_IN_CONTROL);
    g_act1_status_after_activation = REG32(USB_AUDIO_IN_CSR);
    g_act1_ep_intr_after_activation = REG32(USB_EP_INTR);
}

static void act1_disable_audio_endpoint(void) {
    act1_snak_and_flush_audio_endpoint();
#if USB_MIC_ACT9_PROBE
    write_endpoint_csr(USB_AUDIO_IN_NE, 0u);
#elif USB_MIC_ACT2_PROBE || USB_MIC_ACT3_PROBE || USB_MIC_ACT4_PROBE || \
    USB_MIC_ACT5_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    write_endpoint_csr(USB_AUDIO_IN_NE, USB_ACT1_AUDIO_NE_ALT1);
#else
    write_endpoint_csr(USB_AUDIO_IN_NE, USB_ACT1_AUDIO_NE_ALT0);
#endif
    act1_clear_audio_pending();
    g_audio_streaming = 0u;
    g_act1_fallback_armed = 0u;
    g_act1_activation_reason = 0u;
    ++g_act1_disable_count;
    g_act1_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act1_control_last = REG32(USB_AUDIO_IN_CONTROL);
}

#if USB_MIC_ACT11_PROBE
static void act11_prepare_set_interface(u8 effective_alt) {
    u32 ep0_out_ne;

    g_act4_effective_alt_last = effective_alt;
    ep0_out_ne = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act4_ep0_out_ne_before = ep0_out_ne;
    g_act11_ep0_ne_before_status = ep0_out_ne;
    ep0_out_ne =
        (ep0_out_ne &
         ~(USB_CSR_NE_CFG_MASK |
           USB_CSR_NE_INTF_MASK |
           USB_CSR_NE_ALT_MASK)) |
        (1u << USB_CSR_NE_CFG_SHIFT) |
        (1u << USB_CSR_NE_INTF_SHIFT) |
        ((u32)effective_alt << USB_CSR_NE_ALT_SHIFT);
    write_endpoint_csr(USB_EP0_OUT_NE, ep0_out_ne);
    g_act4_ep0_out_ne_after = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act11_ep0_ne_after_status = g_act4_ep0_out_ne_after;

    g_act11_audio_ne_before_status = read_endpoint_csr(USB_AUDIO_IN_NE);
    write_endpoint_csr(
        USB_AUDIO_IN_NE,
        effective_alt == AUDIO_STREAMING_ALT
            ? USB_ACT1_AUDIO_NE_ALT1
            : USB_ACT1_AUDIO_NE_ALT0
    );
    g_act11_audio_ne_after_status = read_endpoint_csr(USB_AUDIO_IN_NE);

    act1_snak_and_flush_audio_endpoint();
    g_act4_ep0_in_control_before = REG32(USB_BASE + 0x000u);
    g_act4_ep0_in_control_after = g_act4_ep0_in_control_before;
}

static void act11_service_status_ack(void) {
    u32 decision;

    if (g_act11_status_pending == 0u) return;

    if (g_act11_status_attempt_count >= 8u) {
        ++g_act11_status_retry_limit_count;
        g_act11_status_pending = 0u;
        g_act11_pending_decision = 0u;
        return;
    }

    g_act11_ep0_in_control_before = REG32(USB_BASE + 0x000u);
    g_act11_ep0_out_control_before = REG32(USB_BASE + 0x200u);
    ++g_act11_status_attempt_count;
    g_act11_ep0_cnak_sequence = ++g_act11_sequence_serial;
    if (g_act11_csr_done_sequence == 0u ||
        g_act11_ep0_cnak_sequence <= g_act11_csr_done_sequence) {
        ++g_act11_order_violation_count;
    }
    if ((g_act11_ep0_in_control_before & USB_EP_CONTROL_NAK) != 0u) {
        REG32(USB_BASE + 0x000u) =
            g_act11_ep0_in_control_before | USB_EP_CONTROL_CNAK;
        ++g_act11_ep0_cnak_write_count;
        tiny_delay();
    }
    g_act11_ep0_in_control_after = REG32(USB_BASE + 0x000u);
    g_act11_ep0_out_control_after = REG32(USB_BASE + 0x200u);
    if (g_act11_ep0_out_control_after !=
        g_act11_ep0_out_control_before) {
        ++g_act11_ep0_out_changed_count;
    }
    if ((g_act11_ep0_in_control_after & USB_EP_CONTROL_NAK) != 0u) {
        ++g_act11_ep0_nak_after_count;
        return;
    }

    decision = g_act11_pending_decision;
    g_act11_pending_decision = 0u;
    g_act11_status_pending = 0u;
    ++g_act11_ep0_cnak_success_count;

    if (decision == USB_ACT5_DECISION_START) {
        act1_activate_audio_endpoint(USB_ACT1_REASON_ALT0_FALLBACK);
        ++g_act11_audio_start_after_ack_count;
        g_act11_audio_activation_sequence = ++g_act11_sequence_serial;
        if (g_act11_audio_activation_sequence <=
            g_act11_ep0_cnak_sequence) {
            ++g_act11_order_violation_count;
        }
    } else if (decision == USB_ACT5_DECISION_STOP) {
        act1_disable_audio_endpoint();
        ++g_act11_audio_stop_after_ack_count;
    } else if (decision == USB_ACT5_DECISION_IDLE) {
        ++g_act11_idle_ack_count;
    }
}

static void act11_arm_status_after_csr_done(u32 decision) {
    if (g_act11_status_pending != 0u) {
        ++g_act11_status_overwrite_count;
    }
    g_act11_pending_decision = decision;
    g_act11_status_pending = 1u;
    g_act11_status_attempt_count = 0u;
    ++g_act11_status_arm_count;
    act11_service_status_ack();
}
#endif
#endif

#if USB_MIC_P15_PROBE && !USB_PCM_TRANSPORT_PROBE && \
    !USB_MIC_ACT10_PROBE && \
    !USB_MIC_ACT11_PROBE && !USB_MIC_ACT12_PROBE && \
    !USB_MIC_ACT13_PROBE
static void attempt_system_usb_register_restore(void) {
    typedef void (*void_fn_t)(void);
    void_fn_t usb_start = (void_fn_t)KNL_USB_START_ADDRESS;
    void_fn_t usb_disconnect =
        (void_fn_t)KNL_USB_DISCONNECT_ADDRESS;
    u32 pending;

    g_recovery_attempted = 1u;
    g_recovery_mask_before = REG32(INTC_MASK_REG);
    REG32(INTC_MASK_SET) = IRQ12_BIT;

    REG32(USB_CONTROL) = 0x00000070u;
    usb_disconnect();
    pending = REG32(USB_INTRUSB);
    if (pending != 0u) REG32(USB_INTRUSB) = pending;
    pending = REG32(USB_EP_INTR);
    if (pending != 0u) REG32(USB_EP_INTR) = pending;

    /*
     * This attempts to rebuild the stock register image. It does not prove
     * that the firmware MSC task or host-visible enumeration was restored.
     */
    usb_start();
    usb_disconnect();
    REG32(USB_CONTROL) = 0x00000070u;
    REG32(SYS_USB_PHY) = g_original_phy & ~0x20000000u;

    pending = REG32(USB_INTRUSB);
    if (pending != 0u) REG32(USB_INTRUSB) = pending;
    pending = REG32(USB_EP_INTR);
    if (pending != 0u) REG32(USB_EP_INTR) = pending;

    g_recovery_phy_after = REG32(SYS_USB_PHY);
    g_recovery_core_after = REG32(USB_BASE + 0x400u);
    g_recovery_control_after = REG32(USB_CONTROL);
    g_recovery_ep4_ne_after = REG32(USB_BASE + 0x514u);
    g_recovery_intrusb_after = REG32(USB_INTRUSB);
    g_recovery_ep_intr_after = REG32(USB_EP_INTR);
    g_recovery_precheck_ok =
        g_recovery_core_after == 0x00000017u &&
        g_recovery_control_after == 0x00000070u &&
        g_recovery_ep4_ne_after == STOCK_EP4_NE &&
        (g_recovery_phy_after & 0x20000000u) == 0u &&
        g_recovery_intrusb_after == 0u &&
        g_recovery_ep_intr_after == 0u;

    if (g_recovery_precheck_ok != 0u) {
        if ((g_original_intc_mask & IRQ12_BIT) != 0u) {
            REG32(INTC_MASK_SET) = IRQ12_BIT;
        } else {
            REG32(INTC_MASK_CLEAR) = IRQ12_BIT;
        }
    }
    g_recovery_mask_after = REG32(INTC_MASK_REG);
    g_recovery_ok =
        g_recovery_precheck_ok != 0u &&
        (g_recovery_mask_after & IRQ12_BIT) ==
            (g_original_intc_mask & IRQ12_BIT);
    if (g_recovery_ok == 0u) {
        REG32(INTC_MASK_SET) = IRQ12_BIT;
        g_recovery_mask_after = REG32(INTC_MASK_REG);
    }
}
#endif

static u16 min_u16(u16 left, u16 right) {
    return left < right ? left : right;
}

static void reset_transfer_state(void) {
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    cdc_c2_set_ep0_in_irq_enabled(0u);
#endif
    g_tx_data = 0;
    g_tx_len = 0;
    g_tx_pos = 0;
    g_expect_out_len = 0;
    g_tx_active = 0;
    g_tx_need_zlp = 0;
    g_zlp_pending = 0;
#if USB_PCM_CDC_PROBE
    g_control_out_action = CDC_CONTROL_OUT_NONE;
#endif
}

static void reset_control_bus_state(void) {
    reset_transfer_state();
    g_configured = 0;
    g_interface_alt = 0;
    g_address = 0;
#if USB_PCM_CDC_PROBE
    g_cdc_sequence = 0u;
    g_cdc_control_line_state = 0u;
#elif USB_PCM_HID_PROBE
    g_hid_sequence = 0u;
#endif
#if USB_MIC_ACT18_PROBE
    g_act18_irq_idle_primed = 0u;
    g_act18_irq_streaming = 0u;
#endif
#if USB_MIC_ACT17_PROBE
    g_act17_irq_idle_primed = 0u;
    g_act17_irq_streaming = 0u;
#endif
}

static void reset_runtime_bus_state(void) {
#if USB_MIC_ACT9_PROBE
    act9_reset_dynamic_endpoint_map();
#endif
#if USB_MIC_ACTIVATION_PROBE
    act1_reset_audio_endpoint();
#endif
#if USB_MIC_ACT10_PROBE
    ++g_a10_software_reset_count;
    g_a10_cnak_pending = 0u;
#endif
#if USB_MIC_ACT13_PROBE
    ++g_act13_software_reset_count;
#endif
#if USB_MIC_ACT11_PROBE
    g_act11_status_pending = 0u;
    g_act11_pending_decision = 0u;
    g_act11_status_attempt_count = 0u;
#endif
#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    g_act5_idle_primed = 0u;
#endif
#if USB_MIC_P14_PROBE
    g_audio_streaming = 0u;
#endif
}

static void reset_bus_state(void) {
    reset_control_bus_state();
    reset_runtime_bus_state();
}

static void start_in_response(const u8 *data, u16 length, u16 requested) {
    u16 actual;
    reset_transfer_state();
    actual = min_u16(length, requested);
    if (actual == 0u) {
        g_zlp_pending = 1;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
        cdc_c2_set_ep0_in_irq_enabled(1u);
#endif
        return;
    }
    g_tx_data = data;
    g_tx_len = actual;
    g_tx_active = 1;
    if (actual < requested && (actual % EP0_MAX_PACKET) == 0u) {
        g_tx_need_zlp = 1;
    }
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    cdc_c2_set_ep0_in_irq_enabled(1u);
#endif
}

static void start_status_zlp(void) {
    reset_transfer_state();
    g_zlp_pending = 1;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    cdc_c2_set_ep0_in_irq_enabled(1u);
#endif
}

static void start_out_discard(u16 length) {
    reset_transfer_state();
    g_expect_out_len = length;
}

#if USB_PCM_CDC_PROBE
static void start_out_line_coding(u16 length) {
    reset_transfer_state();
    g_expect_out_len = length;
    g_control_out_action = CDC_CONTROL_OUT_LINE_CODING;
}
#endif

static int descriptor_for_request(u16 value, const u8 **data, u16 *length) {
    u8 type;
    u8 index;
    type = (u8)(value >> 8);
    index = (u8)value;
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
#if USB_PCM_HID_PROBE
    if (type == USB_DESC_HID && index == 0u) {
        *data = k_hid_desc;
        *length = (u16)sizeof(k_hid_desc);
        return 1;
    }
    if (type == USB_DESC_HID_REPORT && index == 0u) {
        ++g_hid_report_descriptor_count;
        *data = k_hid_report_desc;
        *length = (u16)sizeof(k_hid_report_desc);
        return 1;
    }
#endif
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
#if USB_PCM_CDC_C3_PROBE
            if (g_active_profile &&
                g_active_profile->machine == BDA_MACHINE_C100) {
                *data = k_string_product_c100_desc;
                *length = (u16)sizeof(k_string_product_c100_desc);
                return 1;
            }
#endif
            *data = k_string_product_desc;
            *length = (u16)sizeof(k_string_product_desc);
            return 1;
        }
    }
    return 0;
}

static void record_setup(const usb_setup_packet_t *setup) {
    usb_setup_packet_t *slot;
    ++g_setup_count;
    if (g_setup_trace_count >= SETUP_TRACE_CAPACITY) return;
    slot = &g_setup_trace[g_setup_trace_count++];
    slot->bm_request_type = setup->bm_request_type;
    slot->b_request = setup->b_request;
    slot->w_value = setup->w_value;
    slot->w_index = setup->w_index;
    slot->w_length = setup->w_length;
}

static void unsupported_response(const usb_setup_packet_t *setup) {
    ++g_unsupported_count;
    if ((setup->bm_request_type & 0x80u) != 0u) {
        start_in_response(k_zero1, 0u, setup->w_length);
    } else if (setup->w_length == 0u) {
        start_status_zlp();
    } else {
        start_out_discard(setup->w_length);
    }
}

static void handle_standard_request(const usb_setup_packet_t *setup) {
    const u8 *data;
    u16 length;
    u8 direction_in;
    u8 recipient;
    u8 value_low;
    u8 index_low;

    direction_in = (u8)(setup->bm_request_type & 0x80u);
    recipient = (u8)(setup->bm_request_type & 0x1fu);
    value_low = (u8)setup->w_value;
    index_low = (u8)setup->w_index;

    switch (setup->b_request) {
        case USB_REQ_GET_STATUS:
            if (direction_in != 0u) start_in_response(k_zero2, 2u, setup->w_length);
            else unsupported_response(setup);
            break;
        case USB_REQ_CLEAR_FEATURE:
        case USB_REQ_SET_FEATURE:
            if (direction_in == 0u && setup->w_length == 0u) start_status_zlp();
            else unsupported_response(setup);
            break;
        case USB_REQ_SET_ADDRESS:
            if (recipient == 0u && direction_in == 0u && setup->w_length == 0u) {
                g_address = (u8)(setup->w_value & 0x7fu);
                start_status_zlp();
            } else unsupported_response(setup);
            break;
        case USB_REQ_GET_DESCRIPTOR:
            if (direction_in != 0u
                && descriptor_for_request(setup->w_value, &data, &length)) {
                start_in_response(data, length, setup->w_length);
            } else unsupported_response(setup);
            break;
        case USB_REQ_GET_CONFIGURATION:
            if (direction_in != 0u) {
                start_in_response(&g_configured, 1u, setup->w_length);
            } else unsupported_response(setup);
            break;
        case USB_REQ_SET_CONFIGURATION:
            if (recipient == 0u && direction_in == 0u && setup->w_length == 0u
                && (value_low == 0u || value_low == 1u)) {
                g_configured = value_low;
                g_interface_alt = 0;
#if USB_PCM_CDC_PROBE
                cdc_apply_hardware_configuration(value_low);
#endif
                start_status_zlp();
            } else unsupported_response(setup);
            break;
        case USB_REQ_GET_INTERFACE:
            if (recipient == 1u && direction_in != 0u) {
                if (index_low == 1u) {
                    start_in_response(&g_interface_alt, 1u, setup->w_length);
                } else {
                    start_in_response(k_zero1, 1u, setup->w_length);
                }
            } else unsupported_response(setup);
            break;
        case USB_REQ_SET_INTERFACE:
#if USB_PCM_CDC_PROBE
            if (recipient == 1u && direction_in == 0u &&
                setup->w_length == 0u &&
                index_low <= 1u && value_low == 0u) {
                g_interface_alt = 0u;
                start_status_zlp();
            } else unsupported_response(setup);
#else
            if (recipient == 1u && direction_in == 0u && setup->w_length == 0u
                && ((index_low == 0u && value_low == 0u)
                    || (index_low == 1u && value_low <= 1u))) {
                if (index_low == 1u) {
                    g_interface_alt = value_low;
#if USB_MIC_P14_PROBE
                    g_audio_streaming = value_low == AUDIO_STREAMING_ALT;
#endif
                }
                start_status_zlp();
            } else unsupported_response(setup);
#endif
            break;
        case USB_REQ_SYNCH_FRAME:
            if (direction_in != 0u) start_in_response(k_zero2, 2u, setup->w_length);
            else unsupported_response(setup);
            break;
        default:
            unsupported_response(setup);
            break;
    }
}

static void handle_class_request(const usb_setup_packet_t *setup) {
    u8 direction_in;
    u8 recipient;
    u8 selector;
    u8 endpoint;
#if USB_MIC_P12_PROBE
    u8 channel;
    u8 entity;
    u8 interface_number;
#endif

    direction_in = (u8)(setup->bm_request_type & 0x80u);
    recipient = (u8)(setup->bm_request_type & 0x1fu);
    selector = (u8)(setup->w_value >> 8);
    endpoint = (u8)setup->w_index;

#if USB_PCM_CDC_PROBE
    ++g_cdc_class_request_count;
    if (recipient == 1u && (u8)setup->w_index == 0u) {
        if (direction_in == 0u &&
            setup->b_request == CDC_SET_LINE_CODING &&
            setup->w_length == CDC_LINE_CODING_BYTES) {
            start_out_line_coding(setup->w_length);
            return;
        }
        if (direction_in != 0u &&
            setup->b_request == CDC_GET_LINE_CODING) {
            ++g_cdc_get_line_coding_count;
            start_in_response(
                g_cdc_line_coding,
                CDC_LINE_CODING_BYTES,
                setup->w_length
            );
            return;
        }
        if (direction_in == 0u &&
            setup->b_request == CDC_SET_CONTROL_LINE_STATE &&
            setup->w_length == 0u) {
            g_cdc_control_line_state = setup->w_value;
            ++g_cdc_set_control_line_count;
            start_status_zlp();
            return;
        }
        if (direction_in == 0u &&
            setup->b_request == CDC_SEND_BREAK &&
            setup->w_length == 0u) {
            g_cdc_break_value = setup->w_value;
            ++g_cdc_send_break_count;
            start_status_zlp();
            return;
        }
    }
    unsupported_response(setup);
    return;
#endif

#if USB_PCM_HID_PROBE
    ++g_hid_class_request_count;
    if (recipient == 1u && (u8)setup->w_index == 0u) {
        if (direction_in != 0u && setup->b_request == HID_GET_REPORT) {
            start_in_response(
                k_hid_zero_report,
                (u16)sizeof(k_hid_zero_report),
                setup->w_length
            );
            return;
        }
        if (direction_in != 0u && setup->b_request == HID_GET_IDLE) {
            ++g_hid_get_idle_count;
            start_in_response(k_zero1, 1u, setup->w_length);
            return;
        }
        if (direction_in != 0u && setup->b_request == HID_GET_PROTOCOL) {
            ++g_hid_get_protocol_count;
            start_in_response(k_zero1, 1u, setup->w_length);
            return;
        }
        if (direction_in == 0u && setup->b_request == HID_SET_REPORT) {
            if (setup->w_length == 0u) start_status_zlp();
            else start_out_discard(setup->w_length);
            return;
        }
        if (direction_in == 0u && setup->b_request == HID_SET_IDLE
            && setup->w_length == 0u) {
            ++g_hid_set_idle_count;
            start_status_zlp();
            return;
        }
        if (direction_in == 0u && setup->b_request == HID_SET_PROTOCOL
            && setup->w_length == 0u) {
            ++g_hid_set_protocol_count;
            start_status_zlp();
            return;
        }
    }
    unsupported_response(setup);
    return;
#endif

#if USB_MIC_P12_PROBE
    channel = (u8)setup->w_value;
    entity = (u8)(setup->w_index >> 8);
    interface_number = (u8)setup->w_index;
    if (recipient == 1u && entity == 5u && interface_number == 0u
        && channel <= 1u) {
        if (selector == 1u) {
            if (direction_in != 0u && setup->b_request == UAC_GET_CUR) {
                start_in_response(k_mute_current, 1u, setup->w_length);
                return;
            }
            if (direction_in == 0u && setup->b_request == UAC_SET_CUR) {
                if (setup->w_length == 0u) start_status_zlp();
                else start_out_discard(setup->w_length);
                return;
            }
        }
        if (selector == 2u) {
            if (direction_in != 0u) {
                if (setup->b_request == UAC_GET_CUR) {
                    start_in_response(k_volume_current, 2u, setup->w_length);
                    return;
                }
                if (setup->b_request == UAC_GET_MIN) {
                    start_in_response(k_volume_min, 2u, setup->w_length);
                    return;
                }
                if (setup->b_request == UAC_GET_MAX) {
                    start_in_response(k_volume_max, 2u, setup->w_length);
                    return;
                }
                if (setup->b_request == UAC_GET_RES) {
                    start_in_response(k_volume_res, 2u, setup->w_length);
                    return;
                }
            } else if (setup->b_request == UAC_SET_CUR) {
                if (setup->w_length == 0u) start_status_zlp();
                else start_out_discard(setup->w_length);
                return;
            }
        }
    }
#endif

    if (recipient == 2u && endpoint == AUDIO_ENDPOINT_ADDR && selector == 1u) {
        if (direction_in != 0u) {
            if (setup->b_request == UAC_GET_RES) {
#if USB_MIC_ACT10_PROBE || USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || \
    USB_MIC_ACT13_PROBE
                start_in_response(k_rate_res, 3u, setup->w_length);
#elif USB_MIC_P12_PROBE
                start_in_response(k_rate_res_8000, 3u, setup->w_length);
#else
                start_in_response(k_rate_res, 3u, setup->w_length);
#endif
                return;
            }
            if (setup->b_request == UAC_GET_CUR) {
                start_in_response(k_rate_16000, 3u, setup->w_length);
                return;
            }
            if (setup->b_request == UAC_GET_MIN) {
#if USB_MIC_ACT10_PROBE || USB_MIC_ACT11_PROBE || USB_MIC_ACT12_PROBE || \
    USB_MIC_ACT13_PROBE
                start_in_response(k_rate_16000, 3u, setup->w_length);
#elif USB_MIC_P12_PROBE
                start_in_response(k_rate_8000, 3u, setup->w_length);
#else
                start_in_response(k_rate_16000, 3u, setup->w_length);
#endif
                return;
            }
            if (setup->b_request == UAC_GET_MAX) {
                start_in_response(k_rate_16000, 3u, setup->w_length);
                return;
            }
        } else if (setup->b_request == UAC_SET_CUR) {
            if (setup->w_length == 0u) start_status_zlp();
            else start_out_discard(setup->w_length);
            return;
        }
    }
    unsupported_response(setup);
}

static void handle_setup(const usb_setup_packet_t *setup) {
    record_setup(setup);
    reset_transfer_state();
    if ((setup->bm_request_type & 0x60u) == 0x00u) {
        handle_standard_request(setup);
    } else if ((setup->bm_request_type & 0x60u) == 0x20u) {
        handle_class_request(setup);
    } else {
        unsupported_response(setup);
    }
}

static void read_setup_fifo(usb_setup_packet_t *setup) {
    u32 first;
    u32 second;
    first = REG32(USB_EP0_OUT_FIFO);
    second = REG32(USB_EP0_OUT_FIFO);
    setup->bm_request_type = (u8)first;
    setup->b_request = (u8)(first >> 8);
    setup->w_value = (u16)(first >> 16);
    setup->w_index = (u16)second;
    setup->w_length = (u16)(second >> 16);
}

static void drain_out_fifo(u32 byte_count) {
    u32 words;
    volatile u32 discard;
    words = (byte_count + 3u) >> 2;
    while (words-- != 0u) discard = REG32(USB_EP0_OUT_FIFO);
    (void)discard;
}

static void fifo_read_bytes(u8 *out, u32 byte_count) {
    u32 word;
    while (byte_count >= 4u) {
        word = REG32(USB_EP0_OUT_FIFO);
        out[0] = (u8)word;
        out[1] = (u8)(word >> 8);
        out[2] = (u8)(word >> 16);
        out[3] = (u8)(word >> 24);
        out += 4;
        byte_count -= 4u;
    }
    if (byte_count != 0u) {
        word = REG32(USB_EP0_OUT_FIFO);
        while (byte_count-- != 0u) {
            *out++ = (u8)word;
            word >>= 8;
        }
    }
}

static void fifo_write_bytes(const u8 *data, u16 length) {
    u32 word;
    while (length >= 4u) {
        word = (u32)data[0]
            | ((u32)data[1] << 8)
            | ((u32)data[2] << 16)
            | ((u32)data[3] << 24);
        REG32(USB_EP0_IN_FIFO) = word;
        data += 4;
        length = (u16)(length - 4u);
    }
    while (length-- != 0u) REG8(USB_EP0_IN_FIFO) = *data++;
}

static void service_ep0_out(void) {
    usb_setup_packet_t setup;
    u32 csr;
    u32 count;
    volatile u32 status_ack;

    ++g_ep0_out_count;
    csr = REG32(USB_EP0_OUT_CSR);
    REG32(USB_EP0_OUT_CSR) = csr & ~0x30u;
    if ((csr & EP0_OUT_SETUP) != 0u) {
        read_setup_fifo(&setup);
#if USB_PCM_TRANSPORT_PROBE
        status_ack = REG32(USB_EP0_COMMIT);
        (void)status_ack;
#endif
        handle_setup(&setup);
        return;
    }
    if ((csr & EP0_OUT_DATA) == 0u) return;
    count = (csr >> 11) & 0x7ffu;
    if (count == 0u) {
        status_ack = REG32(USB_EP0_COMMIT);
        (void)status_ack;
        ++g_status_out_count;
        return;
    }
#if USB_PCM_CDC_PROBE
    if (g_control_out_action == CDC_CONTROL_OUT_LINE_CODING &&
        count == CDC_LINE_CODING_BYTES &&
        g_expect_out_len == CDC_LINE_CODING_BYTES) {
        fifo_read_bytes(g_cdc_line_coding, count);
        ++g_cdc_set_line_coding_count;
    } else {
        drain_out_fifo(count);
    }
#else
    drain_out_fifo(count);
#endif
    status_ack = REG32(USB_EP0_COMMIT);
    (void)status_ack;
    g_expect_out_len = 0;
#if USB_PCM_CDC_PROBE
    g_control_out_action = CDC_CONTROL_OUT_NONE;
#endif
    g_zlp_pending = 1;
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    cdc_c2_set_ep0_in_irq_enabled(1u);
#endif
}

static void service_ep0_in(void) {
    u32 csr;
    u16 remaining;
    u16 packet_length;

    ++g_ep0_in_count;
    csr = REG32(USB_EP0_IN_CSR);
    if ((csr & EP_CSR_TOKEN) == 0u) return;
#if USB_PCM_TRANSPORT_PROBE
    /* JZ4730 clears the IN-token status before filling and confirming FIFO. */
    REG32(USB_EP0_IN_CSR) = csr & ~EP_CSR_TOKEN;
#endif

    packet_length = 0;
    if (g_tx_active != 0u) {
        if (g_tx_pos < g_tx_len) {
            remaining = (u16)(g_tx_len - g_tx_pos);
            packet_length = min_u16(remaining, (u16)EP0_MAX_PACKET);
            fifo_write_bytes(g_tx_data + g_tx_pos, packet_length);
            g_tx_pos = (u16)(g_tx_pos + packet_length);
            if (g_tx_pos >= g_tx_len && g_tx_need_zlp == 0u) g_tx_active = 0;
        } else if (g_tx_need_zlp != 0u) {
            g_tx_need_zlp = 0;
            g_tx_active = 0;
        } else {
            g_tx_active = 0;
        }
        REG32(USB_EP0_COMMIT) = 0;
        ++g_ep0_packet_count;
        g_ep0_byte_count += packet_length;
    } else if (g_zlp_pending != 0u) {
        g_zlp_pending = 0;
        REG32(USB_EP0_COMMIT) = 0;
        ++g_ep0_packet_count;
    }
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    if (cdc_c2_ep0_response_queued() == 0u) {
        cdc_c2_set_ep0_in_irq_enabled(0u);
    }
#endif
#if !USB_PCM_TRANSPORT_PROBE
    REG32(USB_EP0_IN_CSR) = csr & ~EP_CSR_TOKEN;
#endif
}

static void service_audio_in(void) {
    u32 csr;
#if USB_MIC_ACT10_PROBE || USB_MIC_ACT13_PROBE
    default_alt_audio_token_trace_t *trace;
#endif
#if USB_MIC_P14_PROBE
    const u32 *packet;
    u32 i;
#endif
#if USB_PCM_TRANSPORT_PROBE
    u32 sequence;
    u32 flags;
#endif
    ++g_ep1_in_count;
    csr = REG32(USB_AUDIO_IN_CSR);
    g_ep1_csr_or |= csr;
    if ((csr & EP_CSR_TOKEN) != 0u) {
#if USB_MIC_ACT10_PROBE
        if (g_a10_first_audio_token_loop == 0u) {
            g_a10_first_audio_token_loop = g_loop_count;
        }
        if (g_a10_audio_token_trace_count < AUDIO_TOKEN_TRACE_CAPACITY) {
            trace =
                &g_a10_audio_token_trace[g_a10_audio_token_trace_count++];
            trace->ordinal = g_ep1_packet_count;
            trace->loop_count = g_loop_count;
            trace->device_status = REG32(USB_DEVICE_STATUS);
            trace->ep_intr = REG32(USB_EP_INTR);
            trace->audio_control = REG32(USB_AUDIO_IN_CONTROL);
            trace->audio_csr = csr;
            trace->audio_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
        }
#endif
#if USB_MIC_ACT13_PROBE
        if (g_act13_first_audio_token_loop == 0u) {
            g_act13_first_audio_token_loop = g_loop_count;
        }
        if (g_act13_audio_token_trace_count < AUDIO_TOKEN_TRACE_CAPACITY) {
            trace =
                &g_act13_audio_token_trace[g_act13_audio_token_trace_count++];
            trace->ordinal = g_ep1_packet_count;
            trace->loop_count = g_loop_count;
            trace->device_status = REG32(USB_DEVICE_STATUS);
            trace->ep_intr = REG32(USB_EP_INTR);
            trace->audio_control = REG32(USB_AUDIO_IN_CONTROL);
            trace->audio_csr = csr;
            trace->audio_ne = read_endpoint_csr(USB_AUDIO_IN_NE);
        }
#endif
#if USB_MIC_ACT6_PROBE
        if (g_act6_first_audio_token_loop == 0u) {
            g_act6_first_audio_token_loop = g_loop_count;
        }
#endif
#if USB_MIC_STATIC_CSR_ACT_PROBE
        if (g_act7_first_audio_token_loop == 0u) {
            g_act7_first_audio_token_loop = g_loop_count;
        }
#endif
#if USB_MIC_ACT9_PROBE
        if (g_act9_first_audio_token_loop == 0u) {
            g_act9_first_audio_token_loop = g_loop_count;
        }
#endif
#if USB_MIC_ACT11_PROBE
        if (g_act11_first_audio_token_loop == 0u) {
            g_act11_first_audio_token_loop = g_loop_count;
        }
#endif
#if USB_PCM_CDC_PROBE
        if (g_pcm_block_count == 0u) {
            ++g_cdc_empty_token_count;
            cdc_set_ep2_irq_enabled(0u);
        } else {
            /* This ordering is the one used by jz4730_udc.c. */
            REG32(USB_AUDIO_IN_CSR) = csr & ~EP_CSR_TOKEN;
            sequence = g_cdc_sequence++;
            flags = 1u;
            REG32(USB_AUDIO_IN_FIFO) = CDC_PCM_MAGIC;
            REG32(USB_AUDIO_IN_FIFO) = sequence;
            REG32(USB_AUDIO_IN_FIFO) = g_loop_count;
            REG32(USB_AUDIO_IN_FIFO) =
                CDC_PCM_SAMPLES | (flags << 16);
            packet = &g_pcm_ring[g_pcm_read_slot][g_pcm_read_offset >> 2];
            for (i = 0u; i < EP1_PACKET_BYTES / 4u; ++i) {
                REG32(USB_AUDIO_IN_FIFO) = packet[i];
            }
            g_pcm_read_offset += EP1_PACKET_BYTES;
            ++g_pcm_packet_count;
            g_pcm_byte_count += EP1_PACKET_BYTES;
            ++g_cdc_valid_pcm_frame_count;
            REG32(USB_AUDIO_IN_FIFO) = g_ep1_packet_count + 1u;
            REG32(USB_AUDIO_IN_FIFO) = g_capture_read_count;
            REG32(USB_AUDIO_IN_FIFO) = g_pcm_underrun_count;
            REG32(USB_AUDIO_IN_FIFO) = ~sequence;
            REG32(USB_AUDIO_IN_CONFIRM) = 0u;
            ++g_ep1_packet_count;
            g_ep1_byte_count += CDC_FRAME_BYTES;
            if (g_pcm_read_offset >= PCM_BLOCK_BYTES) {
                g_pcm_read_offset = 0u;
                g_pcm_read_slot ^= 1u;
                __asm__ volatile("sync" ::: "memory");
                --g_pcm_block_count;
            }
            if (g_pcm_block_count == 0u) {
                cdc_set_ep2_irq_enabled(0u);
            }
        }
        g_audio_streaming = g_configured == 1u;
#elif USB_PCM_HID_PROBE
        sequence = g_hid_sequence++;
        flags = g_pcm_block_count != 0u ? 1u : 2u;
        REG32(USB_AUDIO_IN_FIFO) = HID_PCM_MAGIC;
        REG32(USB_AUDIO_IN_FIFO) = sequence;
        REG32(USB_AUDIO_IN_FIFO) = g_loop_count;
        REG32(USB_AUDIO_IN_FIFO) =
            HID_PCM_SAMPLES | (flags << 16);
        if (g_pcm_block_count != 0u) {
            packet = &g_pcm_ring[g_pcm_read_slot][g_pcm_read_offset >> 2];
            for (i = 0u; i < EP1_PACKET_BYTES / 4u; ++i) {
                REG32(USB_AUDIO_IN_FIFO) = packet[i];
            }
            g_pcm_read_offset += EP1_PACKET_BYTES;
            ++g_pcm_packet_count;
            g_pcm_byte_count += EP1_PACKET_BYTES;
            ++g_hid_valid_pcm_report_count;
            if (g_pcm_read_offset >= PCM_BLOCK_BYTES) {
                g_pcm_read_offset = 0u;
                g_pcm_read_slot ^= 1u;
                __asm__ volatile("sync" ::: "memory");
                --g_pcm_block_count;
            }
        } else {
            for (i = 0u; i < EP1_PACKET_BYTES / 4u; ++i) {
                REG32(USB_AUDIO_IN_FIFO) = 0u;
            }
            ++g_silence_packet_count;
            ++g_pcm_underrun_count;
            ++g_hid_silence_report_count;
        }
        REG32(USB_AUDIO_IN_FIFO) = g_ep1_packet_count + 1u;
        REG32(USB_AUDIO_IN_FIFO) = g_capture_read_count;
        REG32(USB_AUDIO_IN_FIFO) = g_pcm_underrun_count;
        REG32(USB_AUDIO_IN_FIFO) = ~sequence;
        REG32(USB_AUDIO_IN_CONFIRM) = 0u;
        ++g_ep1_packet_count;
        g_ep1_byte_count += HID_REPORT_BYTES;
        g_audio_streaming = 1u;
#elif USB_MIC_P14_PROBE
        g_audio_streaming = 1u;
        if (g_pcm_block_count != 0u) {
            packet = &g_pcm_ring[g_pcm_read_slot][g_pcm_read_offset >> 2];
            for (i = 0u; i < EP1_PACKET_BYTES / 4u; ++i) {
                REG32(USB_AUDIO_IN_FIFO) = packet[i];
            }
            g_pcm_read_offset += EP1_PACKET_BYTES;
            ++g_pcm_packet_count;
            g_pcm_byte_count += EP1_PACKET_BYTES;
            if (g_pcm_read_offset >= PCM_BLOCK_BYTES) {
                g_pcm_read_offset = 0u;
                g_pcm_read_slot ^= 1u;
#if USB_MIC_INTERRUPT_TIMED_PROBE
                __asm__ volatile("sync" ::: "memory");
#endif
                --g_pcm_block_count;
            }
        } else {
            for (i = 0u; i < EP1_PACKET_BYTES / 4u; ++i) {
                REG32(USB_AUDIO_IN_FIFO) = 0u;
            }
            ++g_silence_packet_count;
            ++g_pcm_underrun_count;
        }
        REG32(USB_AUDIO_IN_CONFIRM) = 0u;
        ++g_ep1_packet_count;
        g_ep1_byte_count += EP1_PACKET_BYTES;
#elif USB_MIC_SILENCE_PROBE
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_FIFO) = 0u;
        REG32(USB_AUDIO_IN_CONFIRM) = 0u;
        ++g_ep1_packet_count;
        g_ep1_byte_count += EP1_PACKET_BYTES;
#endif
#if !USB_PCM_CDC_PROBE
        REG32(USB_AUDIO_IN_CSR) = csr & ~EP_CSR_TOKEN;
#endif
    }
}

#if USB_PCM_CDC_PROBE
static void service_cdc_out(void) {
    u32 csr;
    u32 count;
    u32 copied;
    volatile u32 receive_confirm;

    ++g_cdc_out_irq_count;
    csr = REG32(USB_CDC_OUT_CSR);
    REG32(USB_CDC_OUT_CSR) = csr & ~0x30u;
    if ((csr & EP0_OUT_DATA) == 0u) {
        return;
    }

    count = (csr >> 11) & 0x7ffu;
    copied = count < (u32)sizeof(g_cdc_out_buffer)
        ? count
        : (u32)sizeof(g_cdc_out_buffer);
    if (copied != 0u) {
        fifo_read_bytes(g_cdc_out_buffer, copied);
    }
    if (count > copied) {
        drain_out_fifo(count - copied);
    }
    receive_confirm = REG32(USB_EP0_COMMIT);
    (void)receive_confirm;
    g_cdc_out_last_count = copied;
    ++g_cdc_out_packet_count;
    g_cdc_out_byte_count += count;
}
#endif

#if USB_MIC_INTERRUPT_TIMED_PROBE
typedef int (*act14_irq_register_fn_t)(
    u32 irq,
    void (*handler)(void *),
    void *argument
);
typedef void (*act14_irq_unregister_fn_t)(u32 irq);

static u32 act14_irq_handler_slot(void) {
    if (!g_active_profile) return 0u;
    return g_active_profile->irq_vector_base +
        g_active_profile->udc_irq * 8u;
}

static void act14_queue_device_event(
    u32 intrusb,
    u32 status_pre,
    u32 status_post
) {
    u32 write = g_act14_event_write;
    u32 depth = write - g_act14_event_read;
    u32 slot;

    if (depth >= ACT14_DEVICE_EVENT_CAPACITY) {
        ++g_act14_event_overflow_count;
        return;
    }
    slot = write & (ACT14_DEVICE_EVENT_CAPACITY - 1u);
    g_act14_event_intr[slot] = intrusb;
    g_act14_event_status_pre[slot] = status_pre;
    g_act14_event_status_post[slot] = status_post;
    __asm__ volatile("sync" ::: "memory");
    g_act14_event_write = write + 1u;
    ++depth;
    if (depth > g_act14_event_queue_high_water) {
        g_act14_event_queue_high_water = depth;
    }
}

static int act14_take_device_event(
    u32 *intrusb,
    u32 *status_pre,
    u32 *status_post
) {
    u32 read = g_act14_event_read;
    u32 slot;

    if (read == g_act14_event_write) return 0;
    slot = read & (ACT14_DEVICE_EVENT_CAPACITY - 1u);
    *intrusb = g_act14_event_intr[slot];
    *status_pre = g_act14_event_status_pre[slot];
    *status_post = g_act14_event_status_post[slot];
    __asm__ volatile("sync" ::: "memory");
    g_act14_event_read = read + 1u;
    return 1;
}

#if USB_MIC_ACT16_PROBE
static u32 act16_complete_hardware_standard_request(
    u32 intrusb,
    u32 status_pre_done
) {
    u32 control_before;
    u32 control_after;
    u32 status_post_done;
    u32 ep0_in_control;

    control_before = REG32(USB_DEVICE_CONTROL);
    if (g_act16_csr_done_count == 0u) {
        g_act16_csr_done_control_before_first = control_before;
        g_act16_status_pre_done_first = status_pre_done;
    }
    g_act16_csr_done_control_before_last = control_before;
    g_act16_status_pre_done_last = status_pre_done;

    /*
     * Synopsys UDC handles SET_CONFIGURATION and SET_INTERFACE in hardware.
     * After accepting either request, CSR_DONE completes its status stage.
     * Keep this in IRQ12, immediately after the device-event W1C, because the
     * Windows xHCI trace gives the device less than one millisecond to reply.
     */
    REG32(USB_DEVICE_CONTROL) =
        control_before | USB_DEVICE_CONTROL_CSR_DONE;
    __asm__ volatile("sync" ::: "memory");
    control_after = REG32(USB_DEVICE_CONTROL);
    status_post_done = REG32(USB_DEVICE_STATUS);

    if (g_act16_csr_done_count == 0u) {
        g_act16_csr_done_control_after_first = control_after;
        g_act16_status_post_done_first = status_post_done;
    }
    g_act16_csr_done_control_after_last = control_after;
    g_act16_status_post_done_last = status_post_done;
    ++g_act16_csr_done_count;
    ++g_csr_done_count;
    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        ++g_act16_sc_csr_done_count;
    }
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
        ++g_act16_si_csr_done_count;
    }

    /*
     * The controller normally handles the zero-length IN response itself.
     * Mirror the upstream driver's fallback only when EP0 IN is still NAKed.
     */
    ep0_in_control = REG32(USB_EP0_IN_CONTROL);
    g_act16_ep0_in_control_before_last = ep0_in_control;
    if ((ep0_in_control & USB_EP_CONTROL_NAK) != 0u) {
        ++g_act16_ep0_in_nak_seen_count;
        REG32(USB_EP0_IN_CONTROL) =
            ep0_in_control | USB_EP_CONTROL_CNAK;
        ++g_act16_ep0_in_cnak_write_count;
    }
    g_act16_ep0_in_control_after_last = REG32(USB_EP0_IN_CONTROL);
    return status_post_done;
}
#endif

#if USB_MIC_ACT18_PROBE
static u32 act18_complete_si_after_w1c(
    u32 status_pre,
    u32 status_after_w1c
) {
    act18_post_w1c_trace_t *trace = 0;
    u32 configured = status_pre & 0x0Fu;
    u32 interface_number = (status_pre >> 4) & 0x0Fu;
    u32 effective_alt = 0u;
    u32 decision = 0u;
    u32 ep0_info_before;
    u32 ep0_info_written;
    u32 ep0_info_readback;
    u32 audio_info_before;
    u32 audio_info_written;
    u32 audio_info_readback;
    u32 control_before_done;
    u32 control_after_done;
    u32 status_after_done;

    ++g_act18_si_complete_count;
    if (g_act18_post_w1c_trace_count <
        ACT18_POST_W1C_TRACE_CAPACITY) {
        trace = &g_act18_post_w1c_trace[g_act18_post_w1c_trace_count++];
        trace->ordinal = g_act18_si_complete_count;
        trace->status_pre = status_pre;
        trace->status_after_w1c = status_after_w1c;
    }

    ep0_info_before = REG32(USB_EP0_OUT_NE);
    audio_info_before = REG32(USB_AUDIO_IN_NE);
    control_before_done = REG32(USB_DEVICE_CONTROL);

    if (configured != 1u || interface_number != 1u) {
        ++g_act18_unexpected_si_state_count;
        if (trace != 0) {
            trace->decision = decision;
            trace->effective_alt = effective_alt;
            trace->ep0_info_before = ep0_info_before;
            trace->ep0_info_written = ep0_info_before;
            trace->ep0_info_readback = ep0_info_before;
            trace->audio_info_before = audio_info_before;
            trace->audio_info_written = audio_info_before;
            trace->audio_info_readback = audio_info_before;
            trace->control_before_done = control_before_done;
            trace->control_after_done = control_before_done;
            trace->status_after_done = status_after_w1c;
        }
        return status_after_w1c;
    }

    /*
     * DEVSTS reports alt zero for all observed SI events. The host sequence is
     * idle-alt0, start-alt1, stop-alt0, so infer only that ordered transition.
     */
    if (g_act18_irq_streaming != 0u) {
        decision = USB_ACT5_DECISION_STOP;
        g_act18_irq_streaming = 0u;
        ++g_act18_stop_si_count;
    } else if (g_act18_irq_idle_primed == 0u) {
        decision = USB_ACT5_DECISION_IDLE;
        g_act18_irq_idle_primed = 1u;
        ++g_act18_idle_si_count;
    } else {
        effective_alt = AUDIO_STREAMING_ALT;
        decision = USB_ACT5_DECISION_START;
        g_act18_irq_streaming = 1u;
        ++g_act18_start_si_count;
    }

    ep0_info_written =
        (ep0_info_before &
         ~(USB_CSR_NE_CFG_MASK |
           USB_CSR_NE_INTF_MASK |
           USB_CSR_NE_ALT_MASK)) |
        (configured << USB_CSR_NE_CFG_SHIFT) |
        (interface_number << USB_CSR_NE_INTF_SHIFT) |
        (effective_alt << USB_CSR_NE_ALT_SHIFT);

    /*
     * EP4 remains the static alt-1 endpoint for every SI. Rewriting the same
     * record after W1C tests the controller's endpoint-programming strobe
     * without changing the descriptor, endpoint number, type, or max packet.
     */
    audio_info_written =
        (audio_info_before &
         ~(USB_CSR_NE_CFG_MASK |
           USB_CSR_NE_INTF_MASK |
           USB_CSR_NE_ALT_MASK)) |
        (1u << USB_CSR_NE_CFG_SHIFT) |
        (1u << USB_CSR_NE_INTF_SHIFT) |
        (AUDIO_STREAMING_ALT << USB_CSR_NE_ALT_SHIFT);

    REG32(USB_EP0_OUT_NE) = ep0_info_written;
    REG32(USB_AUDIO_IN_NE) = audio_info_written;
    __asm__ volatile("sync" ::: "memory");
    ep0_info_readback = REG32(USB_EP0_OUT_NE);
    audio_info_readback = REG32(USB_AUDIO_IN_NE);

    ++g_act18_ep0_info_write_count;
    if (ep0_info_readback != ep0_info_written) {
        ++g_act18_ep0_info_readback_mismatch_count;
    }
    ++g_act18_audio_info_rewrite_count;
    if (audio_info_readback != audio_info_written) {
        ++g_act18_audio_info_readback_mismatch_count;
    }

    control_before_done = REG32(USB_DEVICE_CONTROL);
    REG32(USB_DEVICE_CONTROL) =
        control_before_done | USB_DEVICE_CONTROL_CSR_DONE;
    __asm__ volatile("sync" ::: "memory");
    control_after_done = REG32(USB_DEVICE_CONTROL);
    status_after_done = REG32(USB_DEVICE_STATUS);

    ++g_act18_csr_done_write_count;
    ++g_csr_done_count;
    g_act18_ep0_info_last = ep0_info_readback;
    g_act18_audio_info_last = audio_info_readback;
    g_act18_control_before_done_last = control_before_done;
    g_act18_control_after_done_last = control_after_done;
    g_act18_status_after_w1c_last = status_after_w1c;
    g_act18_status_after_done_last = status_after_done;

    if (trace != 0) {
        trace->decision = decision;
        trace->effective_alt = effective_alt;
        trace->ep0_info_before = ep0_info_before;
        trace->ep0_info_written = ep0_info_written;
        trace->ep0_info_readback = ep0_info_readback;
        trace->audio_info_before = audio_info_before;
        trace->audio_info_written = audio_info_written;
        trace->audio_info_readback = audio_info_readback;
        trace->control_before_done = control_before_done;
        trace->control_after_done = control_after_done;
        trace->status_after_done = status_after_done;
    }
    return status_after_done;
}
#endif

#if USB_MIC_ACT17_PROBE
static u32 act17_prepare_si_before_w1c(u32 status_pre) {
    act17_pre_w1c_trace_t *trace = 0;
    u32 trace_index = ACT17_TRACE_INVALID;
    u32 configured = status_pre & 0x0Fu;
    u32 interface_number = (status_pre >> 4) & 0x0Fu;
    u32 effective_alt = 0u;
    u32 decision = 0u;
    u32 ep0_info;
    u32 ep0_info_readback;

    ++g_act17_si_prepare_count;
    if (g_act17_pre_w1c_trace_count <
        ACT17_PRE_W1C_TRACE_CAPACITY) {
        trace_index = g_act17_pre_w1c_trace_count++;
        trace = &g_act17_pre_w1c_trace[trace_index];
        trace->ordinal = g_act17_si_prepare_count;
        trace->status_pre = status_pre;
        trace->status_post = ACT17_TRACE_INVALID;
    }

    if (configured != 1u || interface_number != 1u) {
        ++g_act17_unexpected_si_state_count;
        if (trace != 0) {
            trace->decision = decision;
            trace->effective_alt = effective_alt;
            trace->ep0_info_before = REG32(USB_EP0_OUT_NE);
            trace->ep0_info_written = trace->ep0_info_before;
            trace->ep0_info_readback = trace->ep0_info_before;
            trace->audio_info = REG32(USB_AUDIO_IN_NE);
            trace->audio_control = REG32(USB_AUDIO_IN_CONTROL);
        }
        return trace_index;
    }

    /*
     * DEVSTS reports alt zero for every observed JZ4730 SI. Windows issues
     * idle-alt0, start-alt1, then stop-alt0 in order, so keep an IRQ-local
     * state machine that cannot be delayed by the main loop.
     */
    if (g_act17_irq_streaming != 0u) {
        effective_alt = 0u;
        decision = USB_ACT5_DECISION_STOP;
        g_act17_irq_streaming = 0u;
        ++g_act17_stop_si_count;
    } else if (g_act17_irq_idle_primed == 0u) {
        effective_alt = 0u;
        decision = USB_ACT5_DECISION_IDLE;
        g_act17_irq_idle_primed = 1u;
        ++g_act17_idle_si_count;
    } else {
        effective_alt = AUDIO_STREAMING_ALT;
        decision = USB_ACT5_DECISION_START;
        g_act17_irq_streaming = 1u;
        ++g_act17_start_si_count;
    }

    ep0_info = REG32(USB_EP0_OUT_NE);
    if (trace != 0) trace->ep0_info_before = ep0_info;
    ep0_info =
        (ep0_info &
         ~(USB_CSR_NE_CFG_MASK |
           USB_CSR_NE_INTF_MASK |
           USB_CSR_NE_ALT_MASK)) |
        (configured << USB_CSR_NE_CFG_SHIFT) |
        (interface_number << USB_CSR_NE_INTF_SHIFT) |
        (effective_alt << USB_CSR_NE_ALT_SHIFT);

    /*
     * A17 tested a direct EP0InfR update before clearing DevIntR. EP4InfR
     * remains the static alt-1 record.
     */
    REG32(USB_EP0_OUT_NE) = ep0_info;
    __asm__ volatile("sync" ::: "memory");
    ep0_info_readback = REG32(USB_EP0_OUT_NE);
    ++g_act17_ep0_info_write_count;
    if (ep0_info_readback != ep0_info) {
        ++g_act17_ep0_info_readback_mismatch_count;
    }
    g_act17_ep0_info_last = ep0_info_readback;
    g_act17_audio_info_last = REG32(USB_AUDIO_IN_NE);

    if (trace != 0) {
        trace->decision = decision;
        trace->effective_alt = effective_alt;
        trace->ep0_info_written = ep0_info;
        trace->ep0_info_readback = ep0_info_readback;
        trace->audio_info = g_act17_audio_info_last;
        trace->audio_control = REG32(USB_AUDIO_IN_CONTROL);
    }

    /* Match the short pre-W1C settling interval in the JZ4730 BSP path. */
    tiny_delay();
    ++g_act17_pre_w1c_delay_count;
    return trace_index;
}
#endif

static void act14_usb_irq(void *argument) {
    u32 intrusb;
    u32 ep_intr;
    u32 status_pre = 0u;
    u32 status_post = 0u;
    u32 handled = 0u;
#if USB_MIC_ACT17_PROBE
    u32 act17_trace_index = ACT17_TRACE_INVALID;
#endif

    (void)argument;
    ++g_act14_irq_entry_count;

    intrusb = REG32(USB_INTRUSB);
    if (intrusb != 0u) {
        status_pre = REG32(USB_DEVICE_STATUS);
#if USB_PCM_CDC_PROBE
        /*
         * Match the JZ4730 BSP: activate function endpoints while handling
         * the hardware-intercepted SET_CONFIGURATION event.
         */
        if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
            cdc_apply_hardware_configuration(status_pre);
            tiny_delay();
        }
#endif
#if USB_MIC_ACT17_PROBE
        if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
            act17_trace_index =
                act17_prepare_si_before_w1c(status_pre);
        }
#endif
        /* C200knl's stock IRQ handler performs this W1C before any branch. */
        REG32(USB_INTRUSB) = intrusb;
        __asm__ volatile("sync" ::: "memory");
        status_post = REG32(USB_DEVICE_STATUS);
#if USB_MIC_ACT18_PROBE
        if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
            status_post = act18_complete_si_after_w1c(
                status_pre,
                status_post
            );
        }
#endif
#if USB_MIC_ACT17_PROBE
        if (act17_trace_index != ACT17_TRACE_INVALID) {
            g_act17_pre_w1c_trace[act17_trace_index].status_post =
                status_post;
        }
#endif
#if USB_MIC_ACT16_PROBE
        if ((intrusb & (USB_INTR_SERVICE_CONFIG |
                        USB_INTR_SERVICE_INTERFACE)) != 0u) {
            status_post = act16_complete_hardware_standard_request(
                intrusb,
                status_post
            );
        }
#endif
        ++g_act14_device_irq_count;
        if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
            ++g_act14_si_ack_count;
            if (g_act14_first_si_irq_loop == 0u) {
                g_act14_first_si_irq_loop = g_loop_count;
                g_act14_si_status_pre_first = status_pre;
                g_act14_si_status_post_first = status_post;
            }
            g_act14_si_status_post_last = status_post;
            if (((status_post >> 8) & 0x0fu) == AUDIO_STREAMING_ALT) {
                ++g_act14_post_ack_alt1_count;
            }
        }
        if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
            ++g_act14_sc_ack_count;
        }
        if ((intrusb & USB_INTR_RESET) != 0u) {
            ++g_act14_reset_ack_count;
#if USB_MIC_EP0_IRQ_PROBE
            /*
             * The host may issue GET_DESCRIPTOR before the main loop consumes
             * the reset event. Reset EP0 transfer state here so that deferred
             * runtime reset work cannot erase the first response.
             */
            reset_control_bus_state();
#if USB_PCM_CDC_PROBE
            cdc_rearm_after_bus_reset();
#endif
#if !USB_PCM_CDC_PROBE
            REG32(USB_BASE + 0x418u) = INTERRUPT_TIMED_EP_MASK;
#endif
            g_act15_endpoint_mask_last = REG32(USB_BASE + 0x418u);
            ++g_act15_reset_fast_count;
#endif
        }
        act14_queue_device_event(intrusb, status_pre, status_post);
        handled = 1u;
    }

    ep_intr = REG32(USB_EP_INTR);
#if USB_MIC_EP0_IRQ_PROBE
    g_act15_ep_intr_or |= ep_intr;
    if ((intrusb & USB_INTR_RESET) != 0u) {
        if (ep_intr != 0u) {
            REG32(USB_EP_INTR) = ep_intr;
            ++g_act15_reset_ep_discard_count;
        }
    } else {
        if ((ep_intr & EP0_OUT_INTERRUPT) != 0u) {
            REG32(USB_EP_INTR) = EP0_OUT_INTERRUPT;
            service_ep0_out();
            ep_intr &= ~EP0_OUT_INTERRUPT;
            ++g_act15_ep0_out_irq_count;
            handled = 1u;
        }
        if ((ep_intr & EP0_IN_INTERRUPT) != 0u) {
#if USB_PCM_CDC_EP0_QUEUE_PROBE
            if (cdc_c2_ep0_response_queued() != 0u) {
                REG32(USB_EP_INTR) = EP0_IN_INTERRUPT;
                ++g_cdc_ep0_w1c_before_service_count;
                service_ep0_in();
            } else {
                cdc_c2_set_ep0_in_irq_enabled(0u);
                ++g_cdc_ep0_empty_irq_count;
            }
#else
            service_ep0_in();
            REG32(USB_EP_INTR) = EP0_IN_INTERRUPT;
#endif
            ep_intr &= ~EP0_IN_INTERRUPT;
            ++g_act15_ep0_in_irq_count;
            handled = 1u;
        }
#if USB_PCM_CDC_PROBE
        if ((ep_intr & CDC_OUT_INTERRUPT) != 0u) {
            service_cdc_out();
            REG32(USB_EP_INTR) = CDC_OUT_INTERRUPT;
            ep_intr &= ~CDC_OUT_INTERRUPT;
            handled = 1u;
        }
#endif
        if ((ep_intr & AUDIO_IN_INTERRUPT) != 0u) {
            ++g_act14_audio_irq_count;
            service_audio_in();
            REG32(USB_EP_INTR) = AUDIO_IN_INTERRUPT;
            ep_intr &= ~AUDIO_IN_INTERRUPT;
            handled = 1u;
        }
        if (ep_intr != 0u) {
            g_other_ep_intr_or |= ep_intr;
            REG32(USB_EP_INTR) = ep_intr;
            ++g_act15_other_ep_irq_count;
            handled = 1u;
        }
    }
#else
    if ((ep_intr & AUDIO_IN_INTERRUPT) != 0u) {
        ++g_act14_audio_irq_count;
        if ((intrusb & USB_INTR_RESET) == 0u) {
            service_audio_in();
        }
        REG32(USB_EP_INTR) = AUDIO_IN_INTERRUPT;
        handled = 1u;
    }
#endif

    if (handled == 0u) ++g_act14_spurious_irq_count;
}

static int act14_install_usb_irq(void) {
    act14_irq_register_fn_t register_irq;
    act14_irq_unregister_fn_t unregister_irq;
    u32 handler_slot = act14_irq_handler_slot();
    u32 argument_slot = handler_slot + 4u;

    if (!g_active_profile || handler_slot == 0u) return 0;
    register_irq =
        (act14_irq_register_fn_t)g_active_profile->irq_register;
    unregister_irq =
        (act14_irq_unregister_fn_t)g_active_profile->irq_unregister;
    g_act14_irq_handler_before = REG32(handler_slot);
    g_act14_irq_argument_before = REG32(argument_slot);
    g_act14_device_mask_readback = REG32(USB_BASE + 0x410u);
    g_act14_endpoint_mask_readback = REG32(USB_BASE + 0x418u);
    if (g_act14_irq_handler_before != g_active_profile->stock_usb_irq ||
        g_act14_device_mask_readback != INTERRUPT_TIMED_DEVICE_MASK ||
        g_act14_endpoint_mask_readback != INTERRUPT_TIMED_EP_MASK) {
        return 0;
    }

    unregister_irq(UDC_IRQ_NUMBER);
    g_act14_irq_register_result = (u32)register_irq(
        UDC_IRQ_NUMBER,
        act14_usb_irq,
        0
    );
    g_act14_irq_handler_after_install = REG32(handler_slot);
    g_act14_irq_argument_after_install = REG32(argument_slot);
    g_act14_intc_mask_after_install = REG32(INTC_MASK_REG);
    g_act14_irq_install_ok =
        g_act14_irq_handler_after_install == (u32)act14_usb_irq &&
        g_act14_irq_argument_after_install == 0u &&
        (g_act14_intc_mask_after_install & IRQ12_BIT) == 0u;
    if (g_act14_irq_install_ok == 0u) {
        REG32(INTC_MASK_SET) = IRQ12_BIT;
        REG32(handler_slot) = g_act14_irq_handler_before;
        REG32(argument_slot) = g_act14_irq_argument_before;
        return 0;
    }
    g_act14_irq_installed = 1u;
    return 1;
}

static void act14_remove_usb_irq(void) {
    act14_irq_unregister_fn_t unregister_irq;
    u32 handler_slot = act14_irq_handler_slot();
    u32 argument_slot = handler_slot + 4u;

    if (!g_active_profile || handler_slot == 0u) return;
    unregister_irq =
        (act14_irq_unregister_fn_t)g_active_profile->irq_unregister;
    REG32(INTC_MASK_SET) = IRQ12_BIT;
    if (g_act14_irq_installed != 0u) {
        unregister_irq(UDC_IRQ_NUMBER);
    }
    REG32(handler_slot) = g_act14_irq_handler_before;
    REG32(argument_slot) = g_act14_irq_argument_before;
    __asm__ volatile("sync" ::: "memory");
    g_act14_irq_handler_after_restore = REG32(handler_slot);
    g_act14_irq_argument_after_restore = REG32(argument_slot);
    g_act14_irq_restore_ok =
        g_act14_irq_handler_after_restore == g_act14_irq_handler_before &&
        g_act14_irq_argument_after_restore == g_act14_irq_argument_before &&
        (REG32(INTC_MASK_REG) & IRQ12_BIT) != 0u;
    g_act14_irq_installed = 0u;
}
#endif

static void observe_device_status(u32 status) {
    g_device_status_or |= status;
    if (status != g_device_status_last) {
        g_device_status_last = status;
        ++g_device_status_change_count;
    }
}

static void sample_device_status(void) {
    observe_device_status(REG32(USB_DEVICE_STATUS));
}

#if USB_MIC_ACT10_PROBE
static void a10_service_cnak_erratum(void) {
    u32 control;
    u32 status;

    if (g_configured != 1u) {
        g_a10_cnak_pending = 0u;
        return;
    }

    control = REG32(USB_AUDIO_IN_CONTROL);
    g_a10_control_last = control;
    g_a10_csr_last = REG32(USB_AUDIO_IN_CSR);
    g_a10_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
    if ((control & USB_EP_CONTROL_NAK) == 0u) {
        if (g_a10_cnak_pending != 0u) {
            ++g_a10_cnak_success_count;
        }
        g_a10_cnak_pending = 0u;
        return;
    }

    g_a10_cnak_pending = 1u;
    status = REG32(USB_DEVICE_STATUS);
    g_a10_cnak_status_last = status;
    if ((status & USB_A10_LEGACY_DEVICE_STATUS_RXFIFO_GUARD) == 0u) {
        ++g_a10_cnak_deferred_count;
        return;
    }
    if (g_a10_cnak_write_count >= 8u) {
        if (g_a10_cnak_retry_limit_count == 0u) {
            ++g_a10_cnak_retry_limit_count;
        }
        return;
    }

    REG32(USB_AUDIO_IN_CONTROL) = control | USB_EP_CONTROL_CNAK;
    ++g_a10_cnak_write_count;
    tiny_delay();
    g_a10_control_last = REG32(USB_AUDIO_IN_CONTROL);
    if ((g_a10_control_last & USB_EP_CONTROL_NAK) == 0u) {
        g_a10_cnak_pending = 0u;
        ++g_a10_cnak_success_count;
    }
}
#endif

#if USB_MIC_P16_PROBE
static void record_service_trace(u32 intrusb, u32 status) {
#if USB_MIC_PROVEN_IN_PROBE
    usb_service_trace_t *slot;
    if (g_service_trace_count >= SERVICE_TRACE_CAPACITY) return;
    slot = &g_service_trace[g_service_trace_count++];
    slot->intrusb = intrusb;
    slot->device_status = status;
    slot->ep_intr = REG32(USB_EP_INTR);
    slot->audio_control = REG32(USB_AUDIO_IN_CONTROL);
    slot->audio_csr = REG32(USB_AUDIO_IN_CSR);
    slot->audio_ne = REG32(USB_AUDIO_IN_NE);
#else
    (void)intrusb;
    (void)status;
#endif
}

static void service_hardware_standard_request(u32 intrusb, u32 status) {
    u8 configured = (u8)(status & 0x0fu);
    u8 interface_number = (u8)((status >> 4) & 0x0fu);
    u8 interface_alt = (u8)((status >> 8) & 0x0fu);
#if USB_MIC_ACT6_PROBE
    u32 deferred_decision = 0u;
#endif
#if USB_MIC_ACT11_PROBE
    u32 post_csr_decision = 0u;
#endif
#if USB_MIC_SERVICE_CSR_PROBE
    u32 ep0_out_ne;
#endif

    record_service_trace(intrusb, status);
    g_service_status_or |= status;
    g_service_status_last = status;
    g_service_config_last = configured;
    g_service_interface_last = interface_number;
    g_service_alt_last = interface_alt;

    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        g_configured = configured;
        g_interface_alt = 0u;
    }
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
        g_interface_alt = interface_number == 1u ? interface_alt : 0u;
    }
    if (configured > 1u || interface_number > 1u || interface_alt > 1u) {
        ++g_service_invalid_count;
    }

#if USB_MIC_DYNAMIC_CSR_PROBE
    g_device_config_last_service = REG32(USB_DEVICE_CONFIG);
#endif
#if USB_MIC_SERVICE_CSR_PROBE
    ep0_out_ne = read_endpoint_csr(USB_EP0_OUT_NE);
    g_ep0_out_ne_before_service = ep0_out_ne;
#if USB_MIC_STATIC_CSR_ACT_PROBE
    g_ep0_out_ne_after_service = ep0_out_ne;
    g_act7_ep0_ne_last = ep0_out_ne;
    g_act7_audio_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#if USB_MIC_ACT8_PROBE
    g_act8_ep1_out_slot_last = read_endpoint_csr(USB_EP1_OUT_NE);
    g_act8_ep2_in_slot_last = read_endpoint_csr(USB_EP2_IN_NE);
#endif
    ++g_act7_static_service_count;
    ++g_act7_ne_write_suppressed_count;
#else
#if USB_MIC_ACT6_PROBE
    if (!((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u &&
          (intrusb & USB_INTR_SERVICE_CONFIG) == 0u &&
          configured == 1u &&
          interface_number == 1u)) {
#endif
        if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
            ep0_out_ne =
                (ep0_out_ne & ~USB_CSR_NE_CFG_MASK) |
                ((u32)configured << USB_CSR_NE_CFG_SHIFT);
            ++g_dynamic_config_program_count;
        }
        if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
            ep0_out_ne =
                (ep0_out_ne &
                 ~(USB_CSR_NE_INTF_MASK | USB_CSR_NE_ALT_MASK)) |
                ((u32)interface_number << USB_CSR_NE_INTF_SHIFT) |
                ((u32)interface_alt << USB_CSR_NE_ALT_SHIFT);
            ++g_dynamic_interface_program_count;
        }
        write_endpoint_csr(USB_EP0_OUT_NE, ep0_out_ne);
        g_ep0_out_ne_after_service = read_endpoint_csr(USB_EP0_OUT_NE);
#if USB_MIC_ACT6_PROBE
    } else {
        g_ep0_out_ne_after_service = ep0_out_ne;
    }
#endif
#endif
#endif

#if USB_PCM_CDC_PROBE
    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        cdc_apply_hardware_configuration(status);
    }
#elif USB_PCM_HID_PROBE
    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        g_audio_streaming = configured == 1u;
        if (g_audio_streaming != 0u) {
            REG32(USB_AUDIO_IN_CONTROL) =
                REG32(USB_AUDIO_IN_CONTROL) | USB_EP_CONTROL_CNAK;
            ++g_hid_cnak_count;
        }
    }
#elif USB_MIC_ACTIVATION_PROBE
    ++g_act1_service_count;
    act1_update_audio_ne(
        intrusb,
        configured,
        interface_number,
        interface_alt
    );
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
        if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
            ++g_act1_combined_si_count;
        } else {
            ++g_act1_standalone_si_count;
        }
        if (interface_alt == 0u) {
            ++g_act1_alt0_count;
        } else {
            ++g_act1_alt1_count;
        }
    }

    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u &&
        configured != 1u &&
        g_audio_streaming != 0u) {
#if USB_MIC_STATIC_CSR_ACT_PROBE
        g_audio_streaming = 0u;
        ++g_act7_software_stop_count;
#else
        act1_disable_audio_endpoint();
#endif
    }
#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u &&
        (intrusb & USB_INTR_SERVICE_CONFIG) == 0u &&
        configured == 1u &&
        interface_number == 1u) {
        u8 effective_alt;
        u32 decision;

#if USB_MIC_ACT9_PROBE
        if (interface_alt == AUDIO_STREAMING_ALT) {
            effective_alt = AUDIO_STREAMING_ALT;
            decision = USB_ACT5_DECISION_START;
            g_act5_idle_primed = 1u;
            ++g_act5_start_si_count;
        } else if (g_audio_streaming != 0u) {
            effective_alt = 0u;
            decision = USB_ACT5_DECISION_STOP;
            ++g_act5_stop_si_count;
        } else if (g_act5_idle_primed == 0u) {
            effective_alt = 0u;
            decision = USB_ACT5_DECISION_IDLE;
            g_act5_idle_primed = 1u;
            ++g_act5_idle_si_count;
        } else {
            effective_alt = AUDIO_STREAMING_ALT;
            decision = USB_ACT5_DECISION_START;
            ++g_act5_start_si_count;
            ++g_act4_inferred_alt1_count;
        }
#else
        if (g_audio_streaming != 0u) {
            effective_alt = 0u;
            decision = USB_ACT5_DECISION_STOP;
            ++g_act5_stop_si_count;
        } else if (g_act5_idle_primed == 0u) {
            effective_alt = 0u;
            decision = USB_ACT5_DECISION_IDLE;
            g_act5_idle_primed = 1u;
            ++g_act5_idle_si_count;
        } else {
            effective_alt = AUDIO_STREAMING_ALT;
            decision = USB_ACT5_DECISION_START;
            ++g_act5_start_si_count;
        }
#endif

        g_interface_alt = effective_alt;
#if USB_MIC_ACT11_PROBE
        act11_prepare_set_interface(effective_alt);
        post_csr_decision = decision;
#elif USB_MIC_STATIC_CSR_ACT_PROBE
        if (decision == USB_ACT5_DECISION_START) {
            g_audio_streaming = 1u;
            g_act1_activation_reason = USB_ACT1_REASON_ALT0_FALLBACK;
            g_act1_fallback_armed = 1u;
            ++g_act1_activation_count;
            ++g_act1_fallback_activation_count;
            ++g_act7_software_start_count;
            g_act7_audio_control_at_start =
                REG32(USB_AUDIO_IN_CONTROL);
        } else if (decision == USB_ACT5_DECISION_STOP) {
            g_audio_streaming = 0u;
            g_act1_activation_reason = 0u;
            g_act1_fallback_armed = 0u;
            ++g_act1_disable_count;
            ++g_act7_software_stop_count;
        }
        g_act1_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
        g_act1_control_last = REG32(USB_AUDIO_IN_CONTROL);
        g_act7_ep0_ne_last = read_endpoint_csr(USB_EP0_OUT_NE);
        g_act7_audio_ne_last = g_act1_ne_last;
#else
        act4_prepare_set_interface(effective_alt);
#if USB_MIC_ACT6_PROBE
        deferred_decision = decision;
#else
        if (decision == USB_ACT5_DECISION_START) {
#if USB_MIC_ACT9_PROBE
            if (g_audio_streaming == 0u) {
#endif
            act1_activate_audio_endpoint(
                interface_alt == AUDIO_STREAMING_ALT
                    ? USB_ACT1_REASON_NORMAL
                    : USB_ACT1_REASON_ALT0_FALLBACK
            );
#if USB_MIC_ACT9_PROBE
            }
#endif
        } else if (decision == USB_ACT5_DECISION_STOP) {
            act1_disable_audio_endpoint();
        }
#endif
#endif
        act5_record_si_trace(status, decision, effective_alt);
    }
#elif USB_MIC_ACT4_PROBE
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u &&
        (intrusb & USB_INTR_SERVICE_CONFIG) == 0u &&
        configured == 1u &&
        interface_number == 1u) {
        u8 effective_alt = interface_alt;

        if (interface_alt == 0u) {
            if (g_audio_streaming != 0u) {
                ++g_act4_inferred_alt0_count;
            } else {
                effective_alt = AUDIO_STREAMING_ALT;
                ++g_act4_inferred_alt1_count;
            }
        }
        g_interface_alt = effective_alt;
        act4_prepare_set_interface(effective_alt);
        if (effective_alt == AUDIO_STREAMING_ALT) {
            if (g_audio_streaming == 0u) {
                act1_activate_audio_endpoint(
                    interface_alt == AUDIO_STREAMING_ALT
                        ? USB_ACT1_REASON_NORMAL
                        : USB_ACT1_REASON_ALT0_FALLBACK
                );
            }
        } else if (g_audio_streaming != 0u) {
            act1_disable_audio_endpoint();
        }
    }
#else
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u &&
        configured == 1u &&
        interface_number == 1u) {
        if (interface_alt == AUDIO_STREAMING_ALT) {
            act1_activate_audio_endpoint(USB_ACT1_REASON_NORMAL);
        } else if ((intrusb & USB_INTR_SERVICE_CONFIG) == 0u) {
            if (g_act1_activation_reason == USB_ACT1_REASON_NORMAL &&
                g_audio_streaming != 0u) {
                act1_disable_audio_endpoint();
            } else if (g_act1_fallback_armed == 0u &&
                       g_audio_streaming == 0u) {
                /*
                 * C200 reports alt 0 for every observed standalone SI.
                 * Arm once after configuration so the same run also tests
                 * whether that field is stale while the host starts capture.
                 */
                act1_activate_audio_endpoint(
                    USB_ACT1_REASON_ALT0_FALLBACK
                );
            }
        }
    }
#endif
#elif USB_MIC_ACT13_PROBE
    ++g_act13_static_service_count;
    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) {
        g_audio_streaming = configured == 1u;
    }
    g_act13_control_last = REG32(USB_AUDIO_IN_CONTROL);
    g_act13_csr_last = REG32(USB_AUDIO_IN_CSR);
    g_act13_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#elif USB_MIC_ACT10_PROBE
    ++g_a10_static_service_count;
    if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u &&
        configured != 1u) {
        g_audio_streaming = 0u;
    }
    g_a10_control_last = REG32(USB_AUDIO_IN_CONTROL);
    g_a10_csr_last = REG32(USB_AUDIO_IN_CSR);
    g_a10_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#elif USB_MIC_P14_PROBE
    g_audio_streaming =
        configured == 1u && interface_number == 1u
        && interface_alt == AUDIO_STREAMING_ALT;
    if (g_audio_streaming != 0u) {
        REG32(USB_AUDIO_IN_CONTROL) =
            REG32(USB_AUDIO_IN_CONTROL) | USB_EP_CONTROL_CNAK;
        ++g_ep4_cnak_count;
    }
#endif

#if USB_MIC_ACT9_PROBE
    /*
     * Dynamic SC/SI acknowledgement uses EP0. Clear only the control
     * endpoint handshake state here; EP2 remains SNAK until alt 1 starts.
     */
    g_act4_ep0_in_control_before = REG32(USB_BASE + 0x000u);
    act4_clear_stall_and_nak(USB_BASE + 0x000u);
    act4_clear_stall_and_nak(USB_BASE + 0x200u);
    g_act4_ep0_in_control_after = REG32(USB_BASE + 0x000u);
#endif

    /* This UDC handles SET_CONFIGURATION/SET_INTERFACE in hardware. */
#if USB_PCM_CDC_PROBE
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
    ++g_cdc_csr_done_suppressed_count;
#elif USB_PCM_HID_PROBE
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
    ++g_hid_csr_done_suppressed_count;
#elif USB_MIC_ACT13_PROBE
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
    ++g_act13_csr_done_suppressed_count;
#elif USB_MIC_ACT10_PROBE
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
    ++g_a10_csr_done_suppressed_count;
#elif USB_MIC_STATIC_CSR_ACT_PROBE
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
#if !USB_MIC_ACT16_PROBE
    ++g_act7_csr_done_suppressed_count;
#endif
    g_act7_ep0_ne_last = read_endpoint_csr(USB_EP0_OUT_NE);
    g_act7_audio_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#if USB_MIC_ACT8_PROBE
    g_act8_ep1_out_slot_last = read_endpoint_csr(USB_EP1_OUT_NE);
    g_act8_ep2_in_slot_last = read_endpoint_csr(USB_EP2_IN_NE);
#endif
#else
#if USB_MIC_ORDERED_SI_ACT_PROBE
    wait_csr_idle();
    g_act5_csr_busy_before_done = REG32(USB_CSR_BUSY);
#endif
#if USB_MIC_ACT6_PROBE
    g_act6_device_control_before_done = REG32(USB_DEVICE_CONTROL);
#endif
#if USB_MIC_ACT11_PROBE
    g_act11_device_control_before_done = REG32(USB_DEVICE_CONTROL);
#endif
    REG32(USB_DEVICE_CONTROL) =
        REG32(USB_DEVICE_CONTROL) | USB_DEVICE_CONTROL_CSR_DONE;
    g_device_control_after_service = REG32(USB_DEVICE_CONTROL);
    ++g_csr_done_count;
#if USB_MIC_ACT11_PROBE
    g_act11_device_control_after_done = g_device_control_after_service;
    g_act11_csr_done_sequence = ++g_act11_sequence_serial;
    act11_arm_status_after_csr_done(post_csr_decision);
#endif
#if USB_MIC_ACT6_PROBE
    g_act6_device_control_after_done = g_device_control_after_service;
    if (deferred_decision == USB_ACT5_DECISION_START ||
        deferred_decision == USB_ACT5_DECISION_STOP) {
        tiny_delay();
        ++g_act6_status_guard_count;
        if (deferred_decision == USB_ACT5_DECISION_START) {
            act1_activate_audio_endpoint(USB_ACT1_REASON_ALT0_FALLBACK);
            ++g_act6_deferred_start_count;
        } else {
            act1_disable_audio_endpoint();
            ++g_act6_deferred_stop_count;
        }
        g_act6_audio_control_after_done = REG32(USB_AUDIO_IN_CONTROL);
        g_act6_audio_ne_after_done = read_endpoint_csr(USB_AUDIO_IN_NE);
    }
#endif
#if USB_MIC_ACTIVATION_PROBE
    g_act1_status_after_csr_done = REG32(USB_DEVICE_STATUS);
    g_act1_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
    g_act1_control_last = REG32(USB_AUDIO_IN_CONTROL);
#endif
#endif
}
#endif

static void usb_service_once(void) {
    u32 intrusb;
    u32 device_status;
    u32 service_status_pre;
    u32 ep_intr;
    u32 high;
    u32 low;
    u32 other;

    sample_device_status();
#if USB_MIC_INTERRUPT_TIMED_PROBE
    if (!act14_take_device_event(
            &intrusb,
            &service_status_pre,
            &device_status
        )) {
        intrusb = 0u;
        device_status = REG32(USB_DEVICE_STATUS);
        service_status_pre = device_status;
    }
#else
    device_status = REG32(USB_DEVICE_STATUS);
    service_status_pre = device_status;
    intrusb = REG32(USB_INTRUSB);
#endif
#if USB_MIC_ACT13_PROBE
    if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) {
        ++g_act13_masked_si_poll_count;
        if (g_act13_masked_si_seen == 0u) {
            g_act13_masked_si_seen = 1u;
            g_act13_masked_si_first_loop = g_loop_count;
            g_act13_masked_si_status_first = device_status;
        }
        g_act13_masked_si_status_last = device_status;
    }
    intrusb &= ~USB_INTR_SERVICE_INTERFACE;
#endif
    if (intrusb != 0u) {
#if !USB_MIC_INTERRUPT_TIMED_PROBE
        REG32(USB_INTRUSB) = intrusb;
#endif
#if USB_MIC_POST_IRQ_STATUS_PROBE
        if ((intrusb & (USB_INTR_SERVICE_CONFIG |
                        USB_INTR_SERVICE_INTERFACE)) != 0u) {
            g_service_status_pre_clear_or |= service_status_pre;
            g_service_status_pre_clear_last = service_status_pre;
#if !USB_MIC_INTERRUPT_TIMED_PROBE
            device_status = REG32(USB_DEVICE_STATUS);
#endif
            g_service_status_post_clear_or |= device_status;
            g_service_status_post_clear_last = device_status;
            if (((device_status >> 8) & 0x0fu) == AUDIO_STREAMING_ALT) {
                ++g_service_post_clear_alt1_count;
            }
            observe_device_status(device_status);
        }
#endif
        ++g_intrusb_count;
        g_intrusb_or |= intrusb;
        if ((intrusb & USB_INTR_CONNECT) != 0u) ++g_connect_count;
        if ((intrusb & USB_INTR_SERVICE_CONFIG) != 0u) ++g_service_config_count;
        if ((intrusb & USB_INTR_SERVICE_INTERFACE) != 0u) ++g_service_interface_count;
        if ((intrusb & USB_INTR_RESET) != 0u) {
            ++g_reset_count;
#if USB_MIC_EP0_IRQ_PROBE
            reset_runtime_bus_state();
#else
            reset_bus_state();
#endif
        }
#if USB_MIC_P16_PROBE
        if ((intrusb & (USB_INTR_SERVICE_CONFIG |
                        USB_INTR_SERVICE_INTERFACE)) != 0u) {
            service_hardware_standard_request(intrusb, device_status);
        }
#endif
    }

#if USB_MIC_ACT10_PROBE
    a10_service_cnak_erratum();
#endif
#if USB_MIC_ACT11_PROBE
    act11_service_status_ack();
#endif
#if USB_MIC_EP0_IRQ_PROBE
    return;
#endif
    ep_intr = REG32(USB_EP_INTR);
    if (ep_intr == 0u) return;
    high = ep_intr & 0xffff0000u;
    low = ep_intr & 0x0000ffffu;

    if ((high & EP0_OUT_INTERRUPT) != 0u) {
        REG32(USB_EP_INTR) = EP0_OUT_INTERRUPT;
        service_ep0_out();
        high &= ~EP0_OUT_INTERRUPT;
    }
    if (high != 0u) {
        g_other_ep_intr_or |= high;
        REG32(USB_EP_INTR) = high;
    }

    if ((low & EP0_IN_INTERRUPT) != 0u) {
        service_ep0_in();
        REG32(USB_EP_INTR) = EP0_IN_INTERRUPT;
        low &= ~EP0_IN_INTERRUPT;
    }
#if !USB_MIC_INTERRUPT_TIMED_PROBE
    if ((low & AUDIO_IN_INTERRUPT) != 0u) {
        service_audio_in();
        REG32(USB_EP_INTR) = AUDIO_IN_INTERRUPT;
        low &= ~AUDIO_IN_INTERRUPT;
    }
#else
    low &= ~AUDIO_IN_INTERRUPT;
#endif
    other = low;
    if (other != 0u) {
        g_other_ep_intr_or |= other;
        REG32(USB_EP_INTR) = other;
    }
}

static void run_usb_loop(void) {
    u32 start;
    start = bda_gui_tick_count_25ms_like();
    while ((bda_gui_tick_count_25ms_like() - start) < PROBE_RUN_TICKS) {
        ++g_loop_count;
        usb_service_once();
#if USB_PCM_CDC_C3_PROBE
        cdc_c3_service_config_fallback();
#endif
#if USB_MIC_P14_PROBE
        capture_service_once();
#endif
#if USB_MIC_P18_PROBE
        ui_service_once();
        usb_service_once();
        if (g_ui_exit_reason != UI_EXIT_NONE) break;
#endif
        tiny_delay();
    }
#if USB_MIC_P18_PROBE
    if (g_ui_exit_reason == UI_EXIT_NONE) {
        g_ui_exit_reason = UI_EXIT_TIMEOUT;
    }
#endif
}

static void log_setup_trace(void) {
    u32 i;
    usb_setup_packet_t *setup;
    for (i = 0; i < g_setup_trace_count; ++i) {
        char line[160];
        char *out = line;
        char *end = line + sizeof(line) - 1u;
        setup = &g_setup_trace[i];
        out = log_append_text(out, end, PROBE_PREFIX "setup bm ");
        out = log_append_hex8(out, end, setup->bm_request_type);
        out = log_append_text(out, end, " req ");
        out = log_append_hex8(out, end, setup->b_request);
        out = log_append_text(out, end, " value ");
        out = log_append_hex8(out, end, setup->w_value);
        out = log_append_text(out, end, " index ");
        out = log_append_hex8(out, end, setup->w_index);
        out = log_append_text(out, end, " length ");
        out = log_append_hex8(out, end, setup->w_length);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}

#if USB_MIC_PROVEN_IN_PROBE
static void log_service_trace(void) {
    u32 i;
    for (i = 0u; i < g_service_trace_count; ++i) {
        usb_service_trace_t *slot = &g_service_trace[i];
        char line[224];
        char *out = line;
        char *end = line + sizeof(line) - 1u;
        out = log_append_text(out, end, PROBE_PREFIX "service intr ");
        out = log_append_hex8(out, end, slot->intrusb);
        out = log_append_text(out, end, " status ");
        out = log_append_hex8(out, end, slot->device_status);
        out = log_append_text(out, end, " ep_intr ");
        out = log_append_hex8(out, end, slot->ep_intr);
        out = log_append_text(out, end, " audio_ctl ");
        out = log_append_hex8(out, end, slot->audio_control);
        out = log_append_text(out, end, " audio_csr ");
        out = log_append_hex8(out, end, slot->audio_csr);
        out = log_append_text(out, end, " ne ");
        out = log_append_hex8(out, end, slot->audio_ne);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}
#endif

#if USB_MIC_ACT10_PROBE || USB_MIC_ACT13_PROBE
static void log_default_alt_audio_token_trace(
    default_alt_audio_token_trace_t *slots,
    u32 count
) {
    u32 i;

    for (i = 0u; i < count; ++i) {
        default_alt_audio_token_trace_t *slot = &slots[i];
        char line[224];
        char *out = line;
        char *end = line + sizeof(line) - 1u;

        out = log_append_text(out, end, PROBE_PREFIX "token ordinal ");
        out = log_append_hex8(out, end, slot->ordinal);
        out = log_append_text(out, end, " loop ");
        out = log_append_hex8(out, end, slot->loop_count);
        out = log_append_text(out, end, " status ");
        out = log_append_hex8(out, end, slot->device_status);
        out = log_append_text(out, end, " ep_intr ");
        out = log_append_hex8(out, end, slot->ep_intr);
        out = log_append_text(out, end, " audio_ctl ");
        out = log_append_hex8(out, end, slot->audio_control);
        out = log_append_text(out, end, " audio_csr ");
        out = log_append_hex8(out, end, slot->audio_csr);
        out = log_append_text(out, end, " ne ");
        out = log_append_hex8(out, end, slot->audio_ne);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}
#endif

#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
static void log_act5_si_trace(void) {
    u32 i;

    for (i = 0u; i < g_act5_si_trace_count; ++i) {
        act5_si_trace_t *slot = &g_act5_si_trace[i];
        char line[224];
        char *out = line;
        char *end = line + sizeof(line) - 1u;

        out = log_append_text(out, end, PROBE_PREFIX "si ordinal ");
        out = log_append_hex8(out, end, slot->ordinal);
        out = log_append_text(out, end, " loop ");
        out = log_append_hex8(out, end, slot->loop_count);
        out = log_append_text(out, end, " status ");
        out = log_append_hex8(out, end, slot->device_status);
        out = log_append_text(out, end, " ep_intr ");
        out = log_append_hex8(out, end, slot->ep_intr);
        out = log_append_text(out, end, " decision ");
        out = log_append_hex8(out, end, slot->decision);
        out = log_append_text(out, end, " effective_alt ");
        out = log_append_hex8(out, end, slot->effective_alt);
        out = log_append_text(out, end, " ep0_ne ");
        out = log_append_hex8(out, end, slot->ep0_out_ne);
        out = log_append_text(out, end, " audio_ctl ");
        out = log_append_hex8(out, end, slot->audio_control);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}
#endif

#if USB_MIC_ACT18_PROBE
static void log_act18_post_w1c_trace(void) {
    u32 i;

    for (i = 0u; i < g_act18_post_w1c_trace_count; ++i) {
        act18_post_w1c_trace_t *trace =
            &g_act18_post_w1c_trace[i];
        char line[512];
        char *out = line;
        char *end = line + sizeof(line) - 1u;

        out = log_append_text(
            out,
            end,
            PROBE_PREFIX "post_w1c_si ordinal "
        );
        out = log_append_hex8(out, end, trace->ordinal);
        out = log_append_text(out, end, " status_pre ");
        out = log_append_hex8(out, end, trace->status_pre);
        out = log_append_text(out, end, " status_w1c ");
        out = log_append_hex8(out, end, trace->status_after_w1c);
        out = log_append_text(out, end, " decision ");
        out = log_append_hex8(out, end, trace->decision);
        out = log_append_text(out, end, " effective_alt ");
        out = log_append_hex8(out, end, trace->effective_alt);
        out = log_append_text(out, end, " ep0_before ");
        out = log_append_hex8(out, end, trace->ep0_info_before);
        out = log_append_text(out, end, " ep0_written ");
        out = log_append_hex8(out, end, trace->ep0_info_written);
        out = log_append_text(out, end, " ep0_readback ");
        out = log_append_hex8(out, end, trace->ep0_info_readback);
        out = log_append_text(out, end, " audio_before ");
        out = log_append_hex8(out, end, trace->audio_info_before);
        out = log_append_text(out, end, " audio_written ");
        out = log_append_hex8(out, end, trace->audio_info_written);
        out = log_append_text(out, end, " audio_readback ");
        out = log_append_hex8(out, end, trace->audio_info_readback);
        out = log_append_text(out, end, " control_before ");
        out = log_append_hex8(out, end, trace->control_before_done);
        out = log_append_text(out, end, " control_after ");
        out = log_append_hex8(out, end, trace->control_after_done);
        out = log_append_text(out, end, " status_done ");
        out = log_append_hex8(out, end, trace->status_after_done);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}
#endif

#if USB_MIC_ACT17_PROBE
static void log_act17_pre_w1c_trace(void) {
    u32 i;

    for (i = 0u; i < g_act17_pre_w1c_trace_count; ++i) {
        act17_pre_w1c_trace_t *trace =
            &g_act17_pre_w1c_trace[i];
        char line[320];
        char *out = line;
        char *end = line + sizeof(line) - 1u;

        out = log_append_text(
            out,
            end,
            PROBE_PREFIX "pre_w1c_si ordinal "
        );
        out = log_append_hex8(out, end, trace->ordinal);
        out = log_append_text(out, end, " status_pre ");
        out = log_append_hex8(out, end, trace->status_pre);
        out = log_append_text(out, end, " decision ");
        out = log_append_hex8(out, end, trace->decision);
        out = log_append_text(out, end, " effective_alt ");
        out = log_append_hex8(out, end, trace->effective_alt);
        out = log_append_text(out, end, " ep0_before ");
        out = log_append_hex8(out, end, trace->ep0_info_before);
        out = log_append_text(out, end, " ep0_written ");
        out = log_append_hex8(out, end, trace->ep0_info_written);
        out = log_append_text(out, end, " ep0_readback ");
        out = log_append_hex8(out, end, trace->ep0_info_readback);
        out = log_append_text(out, end, " audio_info ");
        out = log_append_hex8(out, end, trace->audio_info);
        out = log_append_text(out, end, " audio_ctl ");
        out = log_append_hex8(out, end, trace->audio_control);
        out = log_append_text(out, end, " status_post ");
        out = log_append_hex8(out, end, trace->status_post);
        out = log_append_text(out, end, "\r\n");
        *out = 0;
        (void)log_puts(line);
    }
}
#endif

static void log_results(void) {
    log_value("original_phy", g_original_phy);
    log_value("original_intc_mask", g_original_intc_mask);
    log_value("loop_count", g_loop_count);
    log_value("intrusb_or", g_intrusb_or);
    log_value("intrusb_count", g_intrusb_count);
    log_value("connect_count", g_connect_count);
    log_value("reset_count", g_reset_count);
    log_value("service_config_count", g_service_config_count);
    log_value("service_interface_count", g_service_interface_count);
    log_value("device_status_or", g_device_status_or);
    log_value("device_status_last", g_device_status_last);
    log_value("device_status_change_count", g_device_status_change_count);
    log_value("hw_configured", g_device_status_last & 0x0fu);
    log_value("hw_interface", (g_device_status_last >> 4) & 0x0fu);
    log_value("hw_interface_alt", (g_device_status_last >> 8) & 0x0fu);
    log_value("ep0_out_count", g_ep0_out_count);
    log_value("ep0_in_count", g_ep0_in_count);
    log_value(AUDIO_COUNT_NAME, g_ep1_in_count);
    log_value(AUDIO_CSR_OR_NAME, g_ep1_csr_or);
    log_value(AUDIO_PACKET_NAME, g_ep1_packet_count);
    log_value(AUDIO_BYTE_NAME, g_ep1_byte_count);
    log_value("ep0_packet_count", g_ep0_packet_count);
    log_value("ep0_byte_count", g_ep0_byte_count);
    log_value("status_out_count", g_status_out_count);
    log_value("setup_count", g_setup_count);
    log_value("setup_trace_count", g_setup_trace_count);
    log_value("unsupported_count", g_unsupported_count);
    log_value("configured", g_configured);
    log_value("interface_alt", g_interface_alt);
    log_value("address", g_address);
    log_value("other_ep_intr_or", g_other_ep_intr_or);
    log_value("stop_intrusb", g_stop_intrusb);
    log_value("stop_ep_intr", g_stop_ep_intr);
#if USB_PCM_CDC_PROBE
    log_value("cdc_transport_enabled", 1u);
    log_value("cdc_frame_bytes", CDC_FRAME_BYTES);
    log_value("cdc_pcm_samples_per_frame", CDC_PCM_SAMPLES);
    log_value("cdc_pcm_magic", CDC_PCM_MAGIC);
    log_value("cdc_sequence_next", g_cdc_sequence);
    log_value("cdc_valid_pcm_frame_count", g_cdc_valid_pcm_frame_count);
    log_value("cdc_empty_token_count", g_cdc_empty_token_count);
    log_value("cdc_class_request_count", g_cdc_class_request_count);
    log_value("cdc_set_line_coding_count", g_cdc_set_line_coding_count);
    log_value("cdc_get_line_coding_count", g_cdc_get_line_coding_count);
    log_value(
        "cdc_set_control_line_count",
        g_cdc_set_control_line_count
    );
    log_value("cdc_send_break_count", g_cdc_send_break_count);
    log_value("cdc_control_line_state", g_cdc_control_line_state);
    log_value("cdc_break_value", g_cdc_break_value);
    log_value(
        "cdc_line_baud",
        (u32)g_cdc_line_coding[0] |
        ((u32)g_cdc_line_coding[1] << 8) |
        ((u32)g_cdc_line_coding[2] << 16) |
        ((u32)g_cdc_line_coding[3] << 24)
    );
    log_value("cdc_line_stop", g_cdc_line_coding[4]);
    log_value("cdc_line_parity", g_cdc_line_coding[5]);
    log_value("cdc_line_bits", g_cdc_line_coding[6]);
    log_value("cdc_jz4730_bsp_pio_controls", 1u);
    log_value(
        "cdc_csr_done_suppressed_count",
        g_cdc_csr_done_suppressed_count
    );
    log_value("cdc_out_irq_count", g_cdc_out_irq_count);
    log_value("cdc_out_packet_count", g_cdc_out_packet_count);
    log_value("cdc_out_byte_count", g_cdc_out_byte_count);
    log_value("cdc_out_last_count", g_cdc_out_last_count);
    log_value("cdc_ep2_irq_enable_count", g_cdc_ep2_irq_enable_count);
    log_value("cdc_ep2_irq_disable_count", g_cdc_ep2_irq_disable_count);
    log_value("cdc_ep2_irq_mask_last", g_cdc_ep2_irq_mask_last);
#if USB_PCM_CDC_EP0_QUEUE_PROBE
    log_value("cdc_ep0_irq_enable_count", g_cdc_ep0_irq_enable_count);
    log_value("cdc_ep0_irq_disable_count", g_cdc_ep0_irq_disable_count);
    log_value("cdc_ep0_empty_irq_count", g_cdc_ep0_empty_irq_count);
    log_value(
        "cdc_ep0_w1c_before_service_count",
        g_cdc_ep0_w1c_before_service_count
    );
    log_value("cdc_ep0_irq_mask_last", g_cdc_ep0_irq_mask_last);
#endif
#if USB_PCM_CDC_C3_PROBE
    log_value("cdc_c3_iad_enabled", 1u);
    log_value("cdc_c3_device_class_triplet", 0x00EF0201u);
    log_value("cdc_c3_config_descriptor_bytes", sizeof(k_config_desc));
    log_value(
        "cdc_c3_config_source",
        g_act14_sc_ack_count != g_cdc_c3_bus_sc_base
            ? (g_cdc_c3_fallback_trigger_count != 0u ? 3u : 1u)
            : (g_cdc_c3_fallback_trigger_count != 0u ? 2u : 0u)
    );
    log_value("cdc_c3_bus_setup_base", g_cdc_c3_bus_setup_base);
    log_value("cdc_c3_bus_sc_base", g_cdc_c3_bus_sc_base);
    log_value("cdc_c3_wait_armed", g_cdc_c3_wait_armed);
    log_value(
        "cdc_c3_fallback_done_on_bus",
        g_cdc_c3_fallback_done_on_bus
    );
    log_value("cdc_c3_wait_armed_count", g_cdc_c3_wait_armed_count);
    log_value(
        "cdc_c3_wait_cancelled_by_sc_count",
        g_cdc_c3_wait_cancelled_by_sc_count
    );
    log_value("cdc_c3_cfg1_poll_count", g_cdc_c3_cfg1_poll_count);
    log_value(
        "cdc_c3_fallback_trigger_count",
        g_cdc_c3_fallback_trigger_count
    );
    log_value("cdc_c3_wait_start_tick", g_cdc_c3_wait_start_tick);
    log_value("cdc_c3_wait_start_loop", g_cdc_c3_wait_start_loop);
    log_value(
        "cdc_c3_fallback_last_status",
        g_cdc_c3_fallback_last_status
    );
    log_value(
        "cdc_c3_fallback_last_intr_before",
        g_cdc_c3_fallback_last_intr_before
    );
    log_value(
        "cdc_c3_fallback_last_intr_after",
        g_cdc_c3_fallback_last_intr_after
    );
    log_value(
        "cdc_c3_fallback_last_loop",
        g_cdc_c3_fallback_last_loop
    );
    log_value(
        "cdc_c3_fallback_last_setup_count",
        g_cdc_c3_fallback_last_setup_count
    );
#endif
    log_value(
        "cdc_out_last_word",
        (u32)g_cdc_out_buffer[0] |
        ((u32)g_cdc_out_buffer[1] << 8) |
        ((u32)g_cdc_out_buffer[2] << 16) |
        ((u32)g_cdc_out_buffer[3] << 24)
    );
    log_value("cdc_bus_reset_rearm_count", g_cdc_bus_reset_rearm_count);
    log_value("cdc_notify_info", read_endpoint_csr(USB_CDC_NOTIFY_NE));
    log_value("cdc_notify_control", REG32(USB_CDC_NOTIFY_CONTROL));
    log_value("cdc_ep2_address", AUDIO_ENDPOINT_ADDR);
    log_value("cdc_ep2_info", read_endpoint_csr(USB_AUDIO_IN_NE));
    log_value("cdc_ep2_control", REG32(USB_AUDIO_IN_CONTROL));
    log_value("cdc_ep2_status", REG32(USB_AUDIO_IN_CSR));
    log_value("cdc_ep2_buffer", REG32(USB_AUDIO_IN_BUFFER));
    log_value("cdc_ep2_max_packet", REG32(USB_AUDIO_IN_MAX_PKT));
    log_value("cdc_ep5_info", read_endpoint_csr(USB_CDC_OUT_NE));
    log_value("cdc_ep5_control", REG32(USB_CDC_OUT_CONTROL));
    log_value("cdc_ep5_status", REG32(USB_CDC_OUT_CSR));
    log_value("cdc_ep5_max_packet", REG32(USB_CDC_OUT_MAX_PKT));
#elif USB_PCM_HID_PROBE
    log_value("hid_transport_enabled", 1u);
    log_value("hid_report_bytes", HID_REPORT_BYTES);
    log_value("hid_pcm_samples_per_report", HID_PCM_SAMPLES);
    log_value("hid_pcm_magic", HID_PCM_MAGIC);
    log_value("hid_sequence_next", g_hid_sequence);
    log_value("hid_valid_pcm_report_count", g_hid_valid_pcm_report_count);
    log_value("hid_silence_report_count", g_hid_silence_report_count);
    log_value("hid_class_request_count", g_hid_class_request_count);
    log_value("hid_set_idle_count", g_hid_set_idle_count);
    log_value("hid_get_idle_count", g_hid_get_idle_count);
    log_value("hid_set_protocol_count", g_hid_set_protocol_count);
    log_value("hid_get_protocol_count", g_hid_get_protocol_count);
    log_value(
        "hid_report_descriptor_count",
        g_hid_report_descriptor_count
    );
    log_value("hid_cnak_count", g_hid_cnak_count);
    log_value(
        "hid_csr_done_suppressed_count",
        g_hid_csr_done_suppressed_count
    );
    log_value("hid_endpoint_address", AUDIO_ENDPOINT_ADDR);
    log_value("hid_endpoint_info", read_endpoint_csr(USB_AUDIO_IN_NE));
    log_value("hid_endpoint_control", REG32(USB_AUDIO_IN_CONTROL));
    log_value("hid_endpoint_status", REG32(USB_AUDIO_IN_CSR));
    log_value("hid_endpoint_buffer", REG32(USB_AUDIO_IN_BUFFER));
    log_value("hid_endpoint_max_packet", REG32(USB_AUDIO_IN_MAX_PKT));
#endif
#if USB_MIC_P14_PROBE
    log_value("capture_firmware", g_capture_firmware);
    log_value("capture_open_result", g_capture_open_result);
    log_value("capture_preroll_result", g_capture_preroll_result);
    log_value("capture_last_read_result", g_capture_last_read_result);
    log_value("capture_stop_result", g_capture_stop_result);
    log_value("capture_started", g_capture_started);
    log_value("capture_read_count", g_capture_read_count);
    log_value("capture_read_error_count", g_capture_read_error_count);
    log_value("capture_ready_count", g_capture_ready_count);
    log_value("capture_slow_read_count", g_capture_slow_read_count);
    log_value("capture_backpressure_count", g_capture_backpressure_count);
    log_value("capture_idle_drop_count", g_capture_idle_drop_count);
    log_value("audio_streaming", g_audio_streaming);
    log_value("pcm_ring_block_count", g_pcm_block_count);
    log_value("pcm_ring_high_water", g_pcm_ring_high_water);
    log_value("pcm_packet_count", g_pcm_packet_count);
    log_value("pcm_byte_count", g_pcm_byte_count);
    log_value("silence_packet_count", g_silence_packet_count);
    log_value("pcm_underrun_count", g_pcm_underrun_count);
    log_value("preroll_nonzero_samples", g_preroll_nonzero_samples);
    log_value("preroll_peak", g_preroll_peak);
#endif
#if USB_MIC_P15_PROBE
    log_value("recovery_attempted", g_recovery_attempted);
    log_value("recovery_precheck_ok", g_recovery_precheck_ok);
    log_value("recovery_ok", g_recovery_ok);
    log_value("mass_storage_restore_verified", 0u);
    log_value("recovery_mask_before", g_recovery_mask_before);
    log_value("recovery_mask_after", g_recovery_mask_after);
    log_value("recovery_phy_after", g_recovery_phy_after);
    log_value("recovery_core_after", g_recovery_core_after);
    log_value("recovery_control_after", g_recovery_control_after);
    log_value("recovery_ep4_ne_after", g_recovery_ep4_ne_after);
    log_value("recovery_intrusb_after", g_recovery_intrusb_after);
    log_value("recovery_ep_intr_after", g_recovery_ep_intr_after);
#endif
#if USB_MIC_P16_PROBE
    log_value("service_status_or", g_service_status_or);
    log_value("service_status_last", g_service_status_last);
    log_value("service_config_last", g_service_config_last);
    log_value("service_interface_last", g_service_interface_last);
    log_value("service_alt_last", g_service_alt_last);
    log_value("service_invalid_count", g_service_invalid_count);
    log_value("csr_done_count", g_csr_done_count);
#if !USB_PCM_TRANSPORT_PROBE
    log_value("ep4_cnak_count", g_ep4_cnak_count);
#endif
    log_value("device_control_after_service", g_device_control_after_service);
#endif
#if USB_MIC_POST_IRQ_STATUS_PROBE
    log_value("service_status_pre_clear_or", g_service_status_pre_clear_or);
    log_value("service_status_pre_clear_last", g_service_status_pre_clear_last);
    log_value("service_status_post_clear_or", g_service_status_post_clear_or);
    log_value(
        "service_status_post_clear_last",
        g_service_status_post_clear_last
    );
    log_value(
        "service_post_clear_alt1_count",
        g_service_post_clear_alt1_count
    );
#endif
#if USB_MIC_PROVEN_IN_PROBE
    log_value("service_trace_count", g_service_trace_count);
#endif
#if USB_MIC_DYNAMIC_CSR_PROBE
    log_value("device_config_after_init", g_device_config_after_init);
    log_value("device_config_last_service", g_device_config_last_service);
    log_value("device_config_at_stop", g_device_config_at_stop);
    log_value("csr_wait_count", g_csr_wait_count);
    log_value("csr_wait_timeout_count", g_csr_wait_timeout_count);
    log_value("csr_wait_spin_max", g_csr_wait_spin_max);
    log_value("csr_write_count", g_csr_write_count);
    log_value("csr_busy_last", g_csr_busy_last);
#endif
#if USB_MIC_SERVICE_CSR_PROBE
    log_value("ep0_out_ne_before_service", g_ep0_out_ne_before_service);
    log_value("ep0_out_ne_after_service", g_ep0_out_ne_after_service);
    log_value(
        "dynamic_config_program_count",
        g_dynamic_config_program_count
    );
    log_value(
        "dynamic_interface_program_count",
        g_dynamic_interface_program_count
    );
#endif
#if USB_MIC_SOFT_RESET_CSR_PROBE
    log_value("soft_reset_before", g_soft_reset_before);
    log_value("soft_reset_after", g_soft_reset_after);
    log_value("soft_reset_count", g_soft_reset_count);
    log_value("device_config_before_write", g_device_config_before_write);
    log_value("device_config_write_value", g_device_config_write_value);
#endif
#if USB_MIC_ACTIVATION_PROBE
    log_value(
        ACTIVATION_LOG_LABEL("device_config_after_init"),
        g_act1_device_config_after_init
    );
    log_value(
        ACTIVATION_LOG_LABEL("device_config_at_stop"),
        g_act1_device_config_at_stop
    );
    log_value(ACTIVATION_LOG_LABEL("initial_control"), g_act1_initial_control);
    log_value(ACTIVATION_LOG_LABEL("initial_ne"), g_act1_initial_ne);
    log_value(ACTIVATION_LOG_LABEL("initial_buffer"), g_act1_initial_buffer);
    log_value(
        ACTIVATION_LOG_LABEL("initial_max_packet"),
        g_act1_initial_max_packet
    );
    log_value(
        ACTIVATION_LOG_LABEL("idle_reset_count"),
        g_act1_idle_reset_count
    );
    log_value(ACTIVATION_LOG_LABEL("service_count"), g_act1_service_count);
    log_value(
        ACTIVATION_LOG_LABEL("combined_si_count"),
        g_act1_combined_si_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("standalone_si_count"),
        g_act1_standalone_si_count
    );
    log_value(ACTIVATION_LOG_LABEL("alt0_count"), g_act1_alt0_count);
    log_value(ACTIVATION_LOG_LABEL("alt1_count"), g_act1_alt1_count);
    log_value(
        ACTIVATION_LOG_LABEL("activation_count"),
        g_act1_activation_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("normal_activation_count"),
        g_act1_normal_activation_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("fallback_activation_count"),
        g_act1_fallback_activation_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("disable_count"),
        g_act1_disable_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("fallback_armed"),
        g_act1_fallback_armed
    );
    log_value(
        ACTIVATION_LOG_LABEL("activation_reason"),
        g_act1_activation_reason
    );
    log_value(
        ACTIVATION_LOG_LABEL("ne_before_activation"),
        g_act1_ne_before_activation
    );
    log_value(
        ACTIVATION_LOG_LABEL("ne_after_activation"),
        g_act1_ne_after_activation
    );
    log_value(ACTIVATION_LOG_LABEL("ne_last"), g_act1_ne_last);
    log_value(
        ACTIVATION_LOG_LABEL("control_before_activation"),
        g_act1_control_before_activation
    );
    log_value(
        ACTIVATION_LOG_LABEL("control_after_activation"),
        g_act1_control_after_activation
    );
    log_value(
        ACTIVATION_LOG_LABEL("control_last"),
        g_act1_control_last
    );
    log_value(
        ACTIVATION_LOG_LABEL("status_after_activation"),
        g_act1_status_after_activation
    );
    log_value(
        ACTIVATION_LOG_LABEL("ep_intr_after_activation"),
        g_act1_ep_intr_after_activation
    );
    log_value(
        ACTIVATION_LOG_LABEL("cnak_write_count"),
        g_act1_cnak_write_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("cnak_retry_count"),
        g_act1_cnak_retry_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("nak_after_cnak_count"),
        g_act1_nak_after_cnak_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("status_after_csr_done"),
        g_act1_status_after_csr_done
    );
#endif
#if USB_MIC_ACT4_PROBE || USB_MIC_ORDERED_SI_ACT_PROBE
    log_value(
        ACTIVATION_LOG_LABEL("effective_alt_last"),
        g_act4_effective_alt_last
    );
    log_value(
        ACTIVATION_LOG_LABEL("inferred_alt1_count"),
        g_act4_inferred_alt1_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("inferred_alt0_count"),
        g_act4_inferred_alt0_count
    );
    log_value(
        ACTIVATION_LOG_LABEL("ep0_out_ne_before"),
        g_act4_ep0_out_ne_before
    );
    log_value(
        ACTIVATION_LOG_LABEL("ep0_out_ne_after"),
        g_act4_ep0_out_ne_after
    );
    log_value(
        ACTIVATION_LOG_LABEL("ep0_in_control_before"),
        g_act4_ep0_in_control_before
    );
    log_value(
        ACTIVATION_LOG_LABEL("ep0_in_control_after"),
        g_act4_ep0_in_control_after
    );
    log_value(
        ACTIVATION_LOG_LABEL("clear_stall_nak_count"),
        g_act4_clear_stall_nak_count
    );
#endif
#if USB_MIC_ACT11_PROBE
    log_value("act11_si_trace_count", g_act5_si_trace_count);
    log_value("act11_idle_primed", g_act5_idle_primed);
    log_value("act11_idle_si_count", g_act5_idle_si_count);
    log_value("act11_start_si_count", g_act5_start_si_count);
    log_value("act11_stop_si_count", g_act5_stop_si_count);
    log_value(
        "act11_csr_busy_before_done",
        g_act5_csr_busy_before_done
    );
#elif USB_MIC_ACT9_PROBE
    log_value("act9_si_trace_count", g_act5_si_trace_count);
    log_value("act9_idle_primed", g_act5_idle_primed);
    log_value("act9_idle_si_count", g_act5_idle_si_count);
    log_value("act9_start_si_count", g_act5_start_si_count);
    log_value("act9_stop_si_count", g_act5_stop_si_count);
    log_value(
        "act9_csr_busy_before_done",
        g_act5_csr_busy_before_done
    );
#elif USB_MIC_ACT5_PROBE
    log_value("act5_si_trace_count", g_act5_si_trace_count);
    log_value("act5_idle_primed", g_act5_idle_primed);
    log_value("act5_idle_si_count", g_act5_idle_si_count);
    log_value("act5_start_si_count", g_act5_start_si_count);
    log_value("act5_stop_si_count", g_act5_stop_si_count);
    log_value(
        "act5_csr_busy_before_done",
        g_act5_csr_busy_before_done
    );
#elif USB_MIC_STATIC_CSR_ACT_PROBE
    log_value(STATIC_ACT_LOG_LABEL("si_trace_count"), g_act5_si_trace_count);
    log_value(STATIC_ACT_LOG_LABEL("idle_primed"), g_act5_idle_primed);
    log_value(STATIC_ACT_LOG_LABEL("idle_si_count"), g_act5_idle_si_count);
    log_value(STATIC_ACT_LOG_LABEL("start_si_count"), g_act5_start_si_count);
    log_value(STATIC_ACT_LOG_LABEL("stop_si_count"), g_act5_stop_si_count);
#endif
#if USB_MIC_ACT6_PROBE
    log_value("act6_deferred_start_count", g_act6_deferred_start_count);
    log_value("act6_deferred_stop_count", g_act6_deferred_stop_count);
    log_value("act6_status_guard_count", g_act6_status_guard_count);
    log_value(
        "act6_audio_preack_skip_count",
        g_act6_audio_preack_skip_count
    );
    log_value(
        "act6_device_control_before_done",
        g_act6_device_control_before_done
    );
    log_value(
        "act6_device_control_after_done",
        g_act6_device_control_after_done
    );
    log_value(
        "act6_audio_control_before_done",
        g_act6_audio_control_before_done
    );
    log_value(
        "act6_audio_control_after_done",
        g_act6_audio_control_after_done
    );
    log_value("act6_audio_ne_after_done", g_act6_audio_ne_after_done);
    log_value(
        "act6_first_audio_token_loop",
        g_act6_first_audio_token_loop
    );
#endif
#if USB_MIC_STATIC_CSR_ACT_PROBE
    log_value(
        STATIC_ACT_LOG_LABEL("static_service_count"),
        g_act7_static_service_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("ne_write_suppressed_count"),
        g_act7_ne_write_suppressed_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("csr_done_suppressed_count"),
        g_act7_csr_done_suppressed_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("software_start_count"),
        g_act7_software_start_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("software_stop_count"),
        g_act7_software_stop_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("first_audio_token_loop"),
        g_act7_first_audio_token_loop
    );
    log_value(
        STATIC_ACT_LOG_LABEL("device_config_write_value"),
        g_act7_device_config_write_value
    );
    log_value(
        STATIC_ACT_LOG_LABEL("device_config_readback"),
        g_act7_device_config_readback
    );
    log_value(
        STATIC_ACT_LOG_LABEL("ep0_ne_initial"),
        g_act7_ep0_ne_initial
    );
    log_value(STATIC_ACT_LOG_LABEL("ep0_ne_last"), g_act7_ep0_ne_last);
    log_value(
        STATIC_ACT_LOG_LABEL("audio_ne_initial"),
        g_act7_audio_ne_initial
    );
    log_value(
        STATIC_ACT_LOG_LABEL("audio_ne_last"),
        g_act7_audio_ne_last
    );
    log_value(
        STATIC_ACT_LOG_LABEL("audio_control_at_start"),
        g_act7_audio_control_at_start
    );
    log_value(STATIC_ACT_LOG_LABEL("prearm_count"), g_act7_prearm_count);
    log_value(
        STATIC_ACT_LOG_LABEL("prearm_cnak_write_count"),
        g_act7_prearm_cnak_write_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("prearm_nak_after_count"),
        g_act7_prearm_nak_after_count
    );
    log_value(
        STATIC_ACT_LOG_LABEL("audio_control_before_prearm"),
        g_act7_audio_control_before_prearm
    );
    log_value(
        STATIC_ACT_LOG_LABEL("audio_control_after_prearm"),
        g_act7_audio_control_after_prearm
    );
#endif
#if USB_MIC_ACT8_PROBE
    log_value("act8_ep1_out_slot_address", (u32)USB_EP1_OUT_NE);
    log_value("act8_ep1_out_slot_initial", g_act8_ep1_out_slot_initial);
    log_value("act8_ep1_out_slot_last", g_act8_ep1_out_slot_last);
    log_value("act8_ep2_in_slot_address", (u32)USB_EP2_IN_NE);
    log_value("act8_ep2_in_slot_initial", g_act8_ep2_in_slot_initial);
    log_value("act8_ep2_in_slot_last", g_act8_ep2_in_slot_last);
#endif
#if USB_MIC_ACT9_PROBE
    log_value("act9_devcfg_before_reset", g_act9_devcfg_before_reset);
    log_value("act9_devcfg_after_reset", g_act9_devcfg_after_reset);
    log_value("act9_devcfg_write_value", g_act9_devcfg_write_value);
    log_value("act9_devcfg_readback", g_act9_devcfg_readback);
    log_value("act9_hard_reset_count", g_act9_hard_reset_count);
    log_value("act9_dynamic_ready", g_act9_dynamic_ready);
    log_value("act9_epreg_reset_count", g_act9_epreg_reset_count);
    log_value(
        "act9_epreg_clear_write_count",
        g_act9_epreg_clear_write_count
    );
    log_value(
        "act9_epreg_nonzero_after_clear",
        g_act9_epreg_nonzero_after_clear
    );
    log_value("act9_ep0_ne_after_reset", g_act9_ep0_ne_after_reset);
    log_value("act9_audio_ne_after_reset", g_act9_audio_ne_after_reset);
    log_value(
        "act9_first_audio_token_loop",
        g_act9_first_audio_token_loop
    );
#endif
#if USB_MIC_ACT10_PROBE
    log_value("a10_default_alt_descriptor", 1u);
    log_value("a10_descriptor_stream_interface", 1u);
    log_value("a10_descriptor_stream_alt", 0u);
    log_value("a10_descriptor_sample_rate", 16000u);
    log_value("a10_hardware_static_interface", 0u);
    log_value("a10_hardware_static_alt", 0u);
    log_value("a10_bus_reset_epreg_write_count", 0u);
    log_value(
        "a10_device_config_after_init",
        g_a10_device_config_after_init
    );
    log_value("a10_initial_control", g_a10_initial_control);
    log_value("a10_initial_csr", g_a10_initial_csr);
    log_value("a10_initial_ne", g_a10_initial_ne);
    log_value("a10_initial_buffer", g_a10_initial_buffer);
    log_value("a10_initial_max_packet", g_a10_initial_max_packet);
    log_value("a10_control_last", g_a10_control_last);
    log_value("a10_csr_last", g_a10_csr_last);
    log_value("a10_ne_last", g_a10_ne_last);
    log_value("a10_software_reset_count", g_a10_software_reset_count);
    log_value("a10_static_service_count", g_a10_static_service_count);
    log_value(
        "a10_csr_done_suppressed_count",
        g_a10_csr_done_suppressed_count
    );
    log_value(
        "a10_first_audio_token_loop",
        g_a10_first_audio_token_loop
    );
    log_value(
        "a10_audio_token_trace_count",
        g_a10_audio_token_trace_count
    );
    log_value("a10_cnak_pending", g_a10_cnak_pending);
    log_value("a10_cnak_write_count", g_a10_cnak_write_count);
    log_value("a10_cnak_deferred_count", g_a10_cnak_deferred_count);
    log_value("a10_cnak_success_count", g_a10_cnak_success_count);
    log_value(
        "a10_cnak_retry_limit_count",
        g_a10_cnak_retry_limit_count
    );
    log_value("a10_cnak_status_last", g_a10_cnak_status_last);
    log_value("a10_hot_restore_attempted", 0u);
    log_value("a10_reboot_required", 1u);
#endif
#if USB_MIC_INTERRUPT_TIMED_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("udc_identity_jz4730"), 1u);
#if USB_PCM_CDC_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("descriptor_interface_count"), 2u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("descriptor_alt0_only"), 1u);
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_ep1_interrupt_in"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_ep2_bulk_in"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_ep5_bulk_out"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_frame_bytes"),
        CDC_FRAME_BYTES
    );
    log_value(INTERRUPT_TIMED_LOG_LABEL("pcm_sample_rate"), 16000u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("hardware_physical_ep"), 2u);
#elif USB_PCM_HID_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("descriptor_interface_count"), 1u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("descriptor_alt0_only"), 1u);
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_ep1_interrupt_in"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_report_bytes"),
        HID_REPORT_BYTES
    );
    log_value(INTERRUPT_TIMED_LOG_LABEL("pcm_sample_rate"), 16000u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("hardware_physical_ep"), 1u);
#else
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_alt0_zero_bandwidth"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("descriptor_alt1_ep4_iso_in"),
        1u
    );
    log_value(INTERRUPT_TIMED_LOG_LABEL("descriptor_sample_rate"), 16000u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("hardware_physical_ep"), 4u);
#endif
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("device_interrupt_mask_expected"),
        INTERRUPT_TIMED_DEVICE_MASK
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("endpoint_interrupt_mask_expected"),
        INTERRUPT_TIMED_EP_MASK
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("device_mask_readback"),
        g_act14_device_mask_readback
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("endpoint_mask_readback"),
        g_act14_endpoint_mask_readback
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_handler_before"),
        g_act14_irq_handler_before
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_argument_before"),
        g_act14_irq_argument_before
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_register_result"),
        g_act14_irq_register_result
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_handler_after_install"),
        g_act14_irq_handler_after_install
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_argument_after_install"),
        g_act14_irq_argument_after_install
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("intc_mask_after_install"),
        g_act14_intc_mask_after_install
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_install_ok"),
        g_act14_irq_install_ok
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_handler_after_restore"),
        g_act14_irq_handler_after_restore
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_argument_after_restore"),
        g_act14_irq_argument_after_restore
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_restore_ok"),
        g_act14_irq_restore_ok
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_entry_count"),
        g_act14_irq_entry_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("device_irq_count"),
        g_act14_device_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("audio_irq_count"),
        g_act14_audio_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("spurious_irq_count"),
        g_act14_spurious_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("event_write"),
        g_act14_event_write
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("event_read"),
        g_act14_event_read
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("event_overflow_count"),
        g_act14_event_overflow_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("event_queue_high_water"),
        g_act14_event_queue_high_water
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_ack_count"),
        g_act14_si_ack_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("sc_ack_count"),
        g_act14_sc_ack_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("reset_ack_count"),
        g_act14_reset_ack_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("first_si_irq_loop"),
        g_act14_first_si_irq_loop
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_status_pre_first"),
        g_act14_si_status_pre_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_status_post_first"),
        g_act14_si_status_post_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_status_post_last"),
        g_act14_si_status_post_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("post_ack_alt1_count"),
        g_act14_post_ack_alt1_count
    );
#if USB_MIC_EP0_IRQ_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep0_irq_driven"), 1u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep0_polled"), 0u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep_intr_or"), g_act15_ep_intr_or);
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_out_irq_count"),
        g_act15_ep0_out_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_in_irq_count"),
        g_act15_ep0_in_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("other_ep_irq_count"),
        g_act15_other_ep_irq_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("reset_ep_discard_count"),
        g_act15_reset_ep_discard_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("reset_fast_count"),
        g_act15_reset_fast_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("endpoint_mask_last"),
        g_act15_endpoint_mask_last
    );
#if USB_MIC_ACT16_PROBE
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_irq_driven"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_count"),
        g_act16_csr_done_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("sc_csr_done_count"),
        g_act16_sc_csr_done_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_csr_done_count"),
        g_act16_si_csr_done_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_control_before_first"),
        g_act16_csr_done_control_before_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_control_before_last"),
        g_act16_csr_done_control_before_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_control_after_first"),
        g_act16_csr_done_control_after_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_control_after_last"),
        g_act16_csr_done_control_after_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_pre_done_first"),
        g_act16_status_pre_done_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_pre_done_last"),
        g_act16_status_pre_done_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_post_done_first"),
        g_act16_status_post_done_first
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_post_done_last"),
        g_act16_status_post_done_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_in_control_before_last"),
        g_act16_ep0_in_control_before_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_in_control_after_last"),
        g_act16_ep0_in_control_after_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_in_nak_seen_count"),
        g_act16_ep0_in_nak_seen_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_in_cnak_write_count"),
        g_act16_ep0_in_cnak_write_count
    );
#endif
#if USB_MIC_ACT18_PROBE
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("post_w1c_ne_plus_csr_done"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_complete_count"),
        g_act18_si_complete_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("post_w1c_trace_count"),
        g_act18_post_w1c_trace_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_idle_primed"),
        g_act18_irq_idle_primed
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_streaming"),
        g_act18_irq_streaming
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("idle_si_count"),
        g_act18_idle_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("start_si_count"),
        g_act18_start_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("stop_si_count"),
        g_act18_stop_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("unexpected_si_state_count"),
        g_act18_unexpected_si_state_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_write_count"),
        g_act18_ep0_info_write_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_readback_mismatch_count"),
        g_act18_ep0_info_readback_mismatch_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("audio_info_rewrite_count"),
        g_act18_audio_info_rewrite_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("audio_info_readback_mismatch_count"),
        g_act18_audio_info_readback_mismatch_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_write_count"),
        g_act18_csr_done_write_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_last"),
        g_act18_ep0_info_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("audio_info_last"),
        g_act18_audio_info_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("control_before_done_last"),
        g_act18_control_before_done_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("control_after_done_last"),
        g_act18_control_after_done_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_after_w1c_last"),
        g_act18_status_after_w1c_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("status_after_done_last"),
        g_act18_status_after_done_last
    );
    log_act18_post_w1c_trace();
#endif
#if USB_MIC_ACT17_PROBE
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("jz4730_native_si_path"),
        1u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("csr_done_write_count"),
        0u
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("si_prepare_count"),
        g_act17_si_prepare_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("pre_w1c_trace_count"),
        g_act17_pre_w1c_trace_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_idle_primed"),
        g_act17_irq_idle_primed
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("irq_streaming"),
        g_act17_irq_streaming
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("idle_si_count"),
        g_act17_idle_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("start_si_count"),
        g_act17_start_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("stop_si_count"),
        g_act17_stop_si_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("unexpected_si_state_count"),
        g_act17_unexpected_si_state_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_write_count"),
        g_act17_ep0_info_write_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_readback_mismatch_count"),
        g_act17_ep0_info_readback_mismatch_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("pre_w1c_delay_count"),
        g_act17_pre_w1c_delay_count
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("ep0_info_last"),
        g_act17_ep0_info_last
    );
    log_value(
        INTERRUPT_TIMED_LOG_LABEL("audio_info_last"),
        g_act17_audio_info_last
    );
    log_act17_pre_w1c_trace();
#endif
#else
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep0_irq_driven"), 0u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep0_polled"), 1u);
#endif
#if USB_PCM_CDC_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep2_interrupt_driven"), 1u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep5_interrupt_driven"), 1u);
#elif USB_PCM_HID_PROBE
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep1_interrupt_driven"), 1u);
#else
    log_value(INTERRUPT_TIMED_LOG_LABEL("ep4_interrupt_driven"), 1u);
#endif
    log_value(INTERRUPT_TIMED_LOG_LABEL("hot_restore_attempted"), 0u);
    log_value(INTERRUPT_TIMED_LOG_LABEL("reboot_required"), 1u);
#endif
#if USB_MIC_ACT13_PROBE
    log_value("act13_udc_identity_jz4730", 1u);
    log_value("act13_descriptor_alt0_zero_bandwidth", 1u);
    log_value("act13_descriptor_alt1_ep4_iso_in", 1u);
    log_value("act13_descriptor_stream_interface", 1u);
    log_value("act13_descriptor_stream_alt", 1u);
    log_value("act13_descriptor_sample_rate", 16000u);
    log_value("act13_hardware_physical_ep", 4u);
    log_value("act13_hardware_static_interface", 1u);
    log_value("act13_hardware_static_alt", 1u);
    log_value("act13_static_ep4_info_expected", USB_AUDIO_IN_NE_VALUE);
    log_value("act13_device_interrupt_mask_expected", 0x00000072u);
    log_value(
        "act13_device_config_after_init",
        g_act13_device_config_after_init
    );
    log_value("act13_initial_control", g_act13_initial_control);
    log_value("act13_initial_csr", g_act13_initial_csr);
    log_value("act13_initial_ne", g_act13_initial_ne);
    log_value("act13_initial_buffer", g_act13_initial_buffer);
    log_value("act13_initial_max_packet", g_act13_initial_max_packet);
    log_value("act13_control_last", g_act13_control_last);
    log_value("act13_csr_last", g_act13_csr_last);
    log_value("act13_ne_last", g_act13_ne_last);
    log_value(
        "act13_software_reset_count",
        g_act13_software_reset_count
    );
    log_value(
        "act13_static_service_count",
        g_act13_static_service_count
    );
    log_value(
        "act13_csr_done_suppressed_count",
        g_act13_csr_done_suppressed_count
    );
    log_value(
        "act13_first_audio_token_loop",
        g_act13_first_audio_token_loop
    );
    log_value(
        "act13_audio_token_trace_count",
        g_act13_audio_token_trace_count
    );
    log_value("act13_masked_si_seen", g_act13_masked_si_seen);
    log_value(
        "act13_masked_si_poll_count",
        g_act13_masked_si_poll_count
    );
    log_value(
        "act13_masked_si_first_loop",
        g_act13_masked_si_first_loop
    );
    log_value(
        "act13_masked_si_status_first",
        g_act13_masked_si_status_first
    );
    log_value(
        "act13_masked_si_status_last",
        g_act13_masked_si_status_last
    );
    log_value("act13_hot_restore_attempted", 0u);
    log_value("act13_reboot_required", 1u);
#endif
#if USB_MIC_ACT12_PROBE && !USB_MIC_INTERRUPT_TIMED_PROBE
    log_value("act12_descriptor_alt0_zero_bandwidth", 1u);
    log_value("act12_descriptor_alt1_ep4_iso_in", 1u);
    log_value("act12_descriptor_sample_rate", 16000u);
    log_value("act12_static_auto_switch", 1u);
    log_value("act12_set_interface_epreg_write_count", 0u);
    log_value("act12_csr_done_write_count", 0u);
    log_value("act12_ep0_status_touch_count", 0u);
    log_value("act12_hot_restore_attempted", 0u);
    log_value("act12_reboot_required", 1u);
#endif
#if USB_MIC_ACT11_PROBE
    log_value("act11_descriptor_alt0_zero_bandwidth", 1u);
    log_value("act11_descriptor_alt1_ep4_iso_in", 1u);
    log_value("act11_descriptor_sample_rate", 16000u);
    log_value("act11_status_pending", g_act11_status_pending);
    log_value("act11_pending_decision", g_act11_pending_decision);
    log_value("act11_status_arm_count", g_act11_status_arm_count);
    log_value(
        "act11_status_overwrite_count",
        g_act11_status_overwrite_count
    );
    log_value("act11_status_attempt_count", g_act11_status_attempt_count);
    log_value(
        "act11_status_deferred_count",
        g_act11_status_deferred_count
    );
    log_value(
        "act11_status_retry_limit_count",
        g_act11_status_retry_limit_count
    );
    log_value(
        "act11_ep0_cnak_write_count",
        g_act11_ep0_cnak_write_count
    );
    log_value(
        "act11_ep0_cnak_success_count",
        g_act11_ep0_cnak_success_count
    );
    log_value(
        "act11_ep0_nak_after_count",
        g_act11_ep0_nak_after_count
    );
    log_value(
        "act11_ep0_out_changed_count",
        g_act11_ep0_out_changed_count
    );
    log_value(
        "act11_device_control_before_done",
        g_act11_device_control_before_done
    );
    log_value(
        "act11_device_control_after_done",
        g_act11_device_control_after_done
    );
    log_value(
        "act11_ep0_in_control_before",
        g_act11_ep0_in_control_before
    );
    log_value(
        "act11_ep0_in_control_after",
        g_act11_ep0_in_control_after
    );
    log_value(
        "act11_ep0_out_control_before",
        g_act11_ep0_out_control_before
    );
    log_value(
        "act11_ep0_out_control_after",
        g_act11_ep0_out_control_after
    );
    log_value(
        "act11_ep0_ne_before_status",
        g_act11_ep0_ne_before_status
    );
    log_value(
        "act11_ep0_ne_after_status",
        g_act11_ep0_ne_after_status
    );
    log_value(
        "act11_audio_ne_before_status",
        g_act11_audio_ne_before_status
    );
    log_value(
        "act11_audio_ne_after_status",
        g_act11_audio_ne_after_status
    );
    log_value(
        "act11_audio_start_after_ack_count",
        g_act11_audio_start_after_ack_count
    );
    log_value(
        "act11_audio_stop_after_ack_count",
        g_act11_audio_stop_after_ack_count
    );
    log_value("act11_idle_ack_count", g_act11_idle_ack_count);
    log_value(
        "act11_first_audio_token_loop",
        g_act11_first_audio_token_loop
    );
    log_value("act11_csr_done_sequence", g_act11_csr_done_sequence);
    log_value("act11_ep0_cnak_sequence", g_act11_ep0_cnak_sequence);
    log_value(
        "act11_audio_activation_sequence",
        g_act11_audio_activation_sequence
    );
    log_value(
        "act11_order_violation_count",
        g_act11_order_violation_count
    );
    log_value("act11_ep0_out_write_count", 0u);
    log_value("act11_hot_restore_attempted", 0u);
    log_value("act11_reboot_required", 1u);
#endif
#if USB_MIC_P17_PROBE
    log_value("audio_endpoint_address", AUDIO_ENDPOINT_ADDR);
    log_value("audio_control_address", (u32)USB_AUDIO_IN_CONTROL);
    log_value("audio_status_address", (u32)USB_AUDIO_IN_CSR);
    log_value("audio_confirm_address", (u32)USB_AUDIO_IN_CONFIRM);
    log_value("audio_fifo_address", (u32)USB_AUDIO_IN_FIFO);
    log_value("audio_ne_address", (u32)USB_AUDIO_IN_NE);
    log_value("audio_ne_value", USB_AUDIO_IN_NE_VALUE);
#endif
#if USB_MIC_P18_PROBE
    log_value("ui_open_ok", g_ui_open_ok);
    log_value("ui_exit_reason", g_ui_exit_reason);
    log_value("wave_generation", g_wave_generation);
    log_value("ui_wave_generation", g_ui_wave_generation);
    log_value("ui_wave_frame_count", g_ui_wave_frame_count);
    log_value("ui_input_poll_count", g_ui_input_poll_count);
    log_value("ui_raw_event_count", g_ui_raw_event_count);
    log_value("ui_touch_down_count", g_ui_touch_down_count);
    log_value("ui_touch_up_count", g_ui_touch_up_count);
    log_value("ui_initial_draw_result", g_ui_initial_draw_result);
    log_value("ui_initial_copy_result", g_ui_initial_copy_result);
    log_value("ui_wave_draw_error_count", g_ui_wave_draw_error_count);
#endif
#if USB_MIC_P13_PROBE
    log_value("stop_device_status", g_stop_device_status);
#if USB_PCM_CDC_PROBE
    log_value("stop_cdc_ep2_control", g_stop_audio_control);
    log_value("stop_cdc_ep2_status", g_stop_audio_csr);
    log_value("stop_cdc_ep2_buffer_size", g_stop_audio_buffer_size);
    log_value("stop_cdc_ep2_max_packet", g_stop_audio_max_packet);
    log_value("stop_cdc_ep2_endpoint_info", g_stop_audio_ne);
#elif USB_PCM_HID_PROBE
    log_value("stop_hid_control", g_stop_audio_control);
    log_value("stop_hid_status", g_stop_audio_csr);
    log_value("stop_hid_buffer_size", g_stop_audio_buffer_size);
    log_value("stop_hid_max_packet", g_stop_audio_max_packet);
    log_value("stop_hid_endpoint_info", g_stop_audio_ne);
#elif USB_MIC_PROVEN_IN_PROBE
    log_value("stop_audio_control", g_stop_audio_control);
    log_value("stop_audio_csr", g_stop_audio_csr);
    log_value("stop_audio_buffer_size", g_stop_audio_buffer_size);
    log_value("stop_audio_max_packet", g_stop_audio_max_packet);
    log_value("stop_audio_ne", g_stop_audio_ne);
#else
    log_value("stop_ep4_control", g_stop_audio_control);
    log_value("stop_ep4_csr", g_stop_audio_csr);
    log_value("stop_ep4_buffer_size", g_stop_audio_buffer_size);
    log_value("stop_ep4_max_packet", g_stop_audio_max_packet);
    log_value("stop_ep4_ne", g_stop_audio_ne);
#endif
#endif
#if USB_MIC_PROVEN_IN_PROBE
    log_service_trace();
#endif
#if USB_MIC_ACT10_PROBE
    log_default_alt_audio_token_trace(
        g_a10_audio_token_trace,
        g_a10_audio_token_trace_count
    );
#endif
#if USB_MIC_ACT13_PROBE
    log_default_alt_audio_token_trace(
        g_act13_audio_token_trace,
        g_act13_audio_token_trace_count
    );
#endif
#if USB_MIC_ORDERED_SI_ACT_PROBE || USB_MIC_STATIC_CSR_ACT_PROBE
    log_act5_si_trace();
#endif
    log_setup_trace();
}

#if USB_PCM_RELEASE_UI
static void log_release_summary(void) {
    log_key_stage("session_end");
    log_key_value("exit_reason", g_ui_exit_reason);
    log_key_value("capture_reads", g_capture_read_count);
    log_key_value("capture_errors", g_capture_read_error_count);
    log_key_value("usb_connects", g_connect_count);
    log_key_value("usb_configured", g_configured);
    log_key_value("pcm_packets", g_ep1_packet_count);
    log_key_value("pcm_bytes", g_ep1_byte_count);
    log_key_value("reboot_required", 1u);
}
#endif

static int usb_cdc_pcm_pch_run(void) {
    int baseline_written;
    u32 pending;

#if USB_PCM_RELEASE_UI
    baseline_written = log_reset();
    if (baseline_written) {
        baseline_written =
            log_puts(PROBE_PREFIX PROBE_BEGIN_TEXT "\r\n");
    }
#else
    bda_msgbox(PROBE_TITLE, PROBE_ENTRY_TEXT);
    (void)bda_fs_mkdir_like(APP_DATA_DIR "\\debug");
    baseline_written = log_puts("\r\n" PROBE_PREFIX PROBE_BEGIN_TEXT "\r\n");
#endif
    if (!baseline_written) {
        bda_msgbox(PROBE_TITLE, "Log write failed. Recording was not started.");
        return 2;
    }
    if (!signature_ok()) {
        log_key_stage("unsupported_firmware");
        bda_msgbox(PROBE_TITLE, "Unsupported firmware. See 9588usbmic.log.");
        return 1;
    }
#if USB_MIC_P14_PROBE
    g_capture_firmware = bda_audio_capture_firmware();
    log_value("capture_firmware_before", g_capture_firmware);
    log_key_value("capture_profile", g_capture_firmware);
    if (g_capture_firmware == BDA_AUDIO_CAPTURE_FIRMWARE_NONE) {
        log_key_stage("unsupported_capture");
        bda_msgbox(PROBE_TITLE, "Audio capture is not supported by this firmware.");
        return 1;
    }
#endif

    g_original_phy = REG32(SYS_USB_PHY);
    g_original_intc_mask = REG32(INTC_MASK_REG);
    log_value("original_phy_before", g_original_phy);
    log_value("original_intc_mask_before", g_original_intc_mask);
    log_stage("baseline_ready_no_b304_read");
#if USB_MIC_P15_PROBE
    if ((g_original_intc_mask & IRQ12_BIT) != 0u ||
        (g_original_phy & 0x20000000u) != 0u) {
        log_stage("recovery_baseline_not_clean");
        log_key_stage("restart_required_before_start");
        bda_msgbox(
            PROBE_TITLE,
#if USB_PCM_CDC_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running CDC PCM."
#elif USB_PCM_HID_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running HID PCM."
#elif USB_MIC_ACT18_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act18."
#elif USB_MIC_ACT17_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act17."
#elif USB_MIC_ACT16_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act16."
#elif USB_MIC_ACT15_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act15."
#elif USB_MIC_ACT14_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act14."
#elif USB_MIC_ACT13_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act13."
#elif USB_MIC_ACT12_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act12."
#elif USB_MIC_ACT11_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act11."
#elif USB_MIC_ACT10_PROBE
            "USB baseline is not clean. Reboot with USB disconnected before running Act10."
#else
            "USB baseline is not clean. Reboot with USB disconnected before running P15."
#endif
        );
        return 4;
    }
#endif

#if !USB_PCM_RELEASE_UI
    if (bda_confirm(PROBE_TITLE, PROBE_CONFIRM_TEXT) != BDA_DIALOG_RESULT_YES) {
        log_stage("cancelled");
        return 0;
    }
#endif

#if USB_MIC_P19_PROBE
    log_stage("about_to_open_live_ui_before_capture");
    if (!ui_open()) {
        log_value("ui_initial_draw_result", g_ui_initial_draw_result);
        log_value("ui_initial_copy_result", g_ui_initial_copy_result);
        log_stage("live_ui_open_failed");
        log_key_stage("ui_open_failed");
        bda_msgbox(PROBE_TITLE, "The recording screen could not be opened.");
        return 5;
    }
    log_stage("live_ui_ready_about_to_open_capture");
#endif
#if USB_MIC_P14_PROBE
    log_stage("about_to_open_and_preroll_capture");
    if (!capture_prepare()) {
        log_value("capture_open_result", g_capture_open_result);
        log_value("capture_preroll_result", g_capture_preroll_result);
        log_value("capture_stop_result", g_capture_stop_result);
        log_stage("capture_prepare_failed");
        log_key_stage("capture_start_failed");
#if USB_MIC_P19_PROBE
        ui_close();
#endif
        bda_msgbox(PROBE_TITLE, "Microphone capture could not be started.");
        return 3;
    }
    log_value("capture_open_result", g_capture_open_result);
    log_value("capture_preroll_result", g_capture_preroll_result);
    log_value("preroll_nonzero_samples", g_preroll_nonzero_samples);
    log_value("preroll_peak", g_preroll_peak);
    log_stage("capture_primed_about_to_take_usb");
    log_key_stage("recording_started");
    log_key_value("preroll_peak", g_preroll_peak);
#if USB_MIC_P19_PROBE
    ui_update_record_indicator(1u);
#endif
#endif
#if USB_MIC_P18_PROBE && !USB_MIC_P19_PROBE
    log_stage("about_to_open_live_ui");
    if (!ui_open()) {
        log_value("ui_initial_draw_result", g_ui_initial_draw_result);
        log_value("ui_initial_copy_result", g_ui_initial_copy_result);
        log_stage("live_ui_open_failed");
        log_key_stage("ui_open_failed");
        if (g_capture_started != 0u) {
            g_capture_stop_result = (u32)bda_audio_capture_stop(&g_capture);
            g_capture_started = 0u;
        }
        bda_msgbox(PROBE_TITLE, "The recording screen could not be opened.");
        return 5;
    }
    log_stage("live_ui_ready");
    ui_update_record_indicator(1u);
#endif

    REG32(INTC_MASK_SET) = IRQ12_BIT;
    log_stage("irq12_masked_about_to_initialize_udc");
    delay_ticks(20u);
    init_usb_core_group();
    init_usb_endpoint_group();
    init_usb_dma_group();
    reset_bus_state();

    pending = REG32(USB_INTRUSB);
    if (pending != 0u) REG32(USB_INTRUSB) = pending;
    pending = REG32(USB_EP_INTR);
    if (pending != 0u) REG32(USB_EP_INTR) = pending;
    log_stage("udc_initialized_about_to_connect_no_fs");
    delay_ticks(20u);

#if USB_MIC_INTERRUPT_TIMED_PROBE
    log_stage("about_to_install_interrupt_timed_usb_handler");
    if (!act14_install_usb_irq()) {
        log_stage("interrupt_timed_usb_handler_install_failed_no_connect");
        log_key_stage("usb_irq_start_failed");
        disconnect_usb_direct();
#if USB_MIC_P14_PROBE
        if (g_capture_started != 0u) {
#if USB_MIC_P18_PROBE
            ui_update_record_indicator(0u);
#endif
            g_capture_stop_result = (u32)bda_audio_capture_stop(&g_capture);
            g_capture_started = 0u;
        }
#endif
#if USB_MIC_P18_PROBE
        ui_close();
#endif
#if USB_PCM_RELEASE_UI
        log_release_summary();
#else
        log_results();
#endif
        bda_msgbox(
            PROBE_TITLE,
            "IRQ handler install failed. Reboot before reconnecting USB."
        );
        return 6;
    }
    log_stage("interrupt_timed_usb_handler_ready_about_to_connect");
#endif

#if USB_MIC_P19_PROBE
    ui_arm_runtime_input();
#endif
#if USB_MIC_ACT9_PROBE
    if (g_act9_dynamic_ready != 0u) {
        log_stage("dynamic_csr_latched_about_to_connect");
        connect_usb_direct();
        run_usb_loop();
    } else {
        log_stage("dynamic_csr_latch_failed_no_pc_connect");
    }
#else
    connect_usb_direct();
    run_usb_loop();
#endif
#if USB_MIC_INTERRUPT_TIMED_PROBE
    act14_remove_usb_irq();
#endif
    g_stop_intrusb = REG32(USB_INTRUSB);
    g_stop_ep_intr = REG32(USB_EP_INTR);
#if USB_MIC_DYNAMIC_CSR_PROBE
    g_device_config_at_stop = REG32(USB_DEVICE_CONFIG);
#endif
#if USB_MIC_ACTIVATION_PROBE
    g_act1_device_config_at_stop = REG32(USB_DEVICE_CONFIG);
#endif
#if USB_MIC_ACT8_PROBE
    g_act8_ep1_out_slot_last = read_endpoint_csr(USB_EP1_OUT_NE);
    g_act8_ep2_in_slot_last = read_endpoint_csr(USB_EP2_IN_NE);
#endif
#if USB_MIC_ACT10_PROBE
    g_a10_control_last = REG32(USB_AUDIO_IN_CONTROL);
    g_a10_csr_last = REG32(USB_AUDIO_IN_CSR);
    g_a10_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#endif
#if USB_MIC_ACT13_PROBE
    g_act13_control_last = REG32(USB_AUDIO_IN_CONTROL);
    g_act13_csr_last = REG32(USB_AUDIO_IN_CSR);
    g_act13_ne_last = read_endpoint_csr(USB_AUDIO_IN_NE);
#endif
#if USB_MIC_P13_PROBE
    g_stop_device_status = REG32(USB_DEVICE_STATUS);
    g_stop_audio_control = REG32(USB_AUDIO_IN_CONTROL);
    g_stop_audio_csr = REG32(USB_AUDIO_IN_CSR);
    g_stop_audio_buffer_size = REG32(USB_AUDIO_IN_BUFFER);
    g_stop_audio_max_packet = REG32(USB_AUDIO_IN_MAX_PKT);
    g_stop_audio_ne = REG32(USB_AUDIO_IN_NE);
#endif
    disconnect_usb_direct();
#if USB_MIC_P14_PROBE
    if (g_capture_started != 0u) {
#if USB_MIC_P18_PROBE
        ui_update_record_indicator(0u);
#endif
        g_capture_stop_result = (u32)bda_audio_capture_stop(&g_capture);
        g_capture_started = 0u;
    }
#endif
#if USB_MIC_P15_PROBE && !USB_PCM_TRANSPORT_PROBE && \
    !USB_MIC_ACT10_PROBE && \
    !USB_MIC_ACT11_PROBE && !USB_MIC_ACT12_PROBE && \
    !USB_MIC_ACT13_PROBE
    attempt_system_usb_register_restore();
#endif
    delay_ticks(40u);
#if USB_MIC_P18_PROBE
    ui_close();
#endif

    log_stage("disconnected");
#if USB_PCM_RELEASE_UI
    log_release_summary();
#else
    log_results();
#endif
#if USB_MIC_P15_PROBE
    log_value(
        "irq12_left_masked",
        (REG32(INTC_MASK_REG) & IRQ12_BIT) != 0u
    );
#if USB_PCM_CDC_PROBE
    log_stage("cdc_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_PCM_HID_PROBE
    log_stage("hid_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT18_PROBE
    log_stage("act18_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT17_PROBE
    log_stage("act17_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT16_PROBE
    log_stage("act16_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT15_PROBE
    log_stage("act15_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT14_PROBE
    log_stage("act14_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT13_PROBE
    log_stage("act13_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT12_PROBE
    log_stage("act12_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT11_PROBE
    log_stage("act11_udc_left_disconnected_irq12_masked_reboot_required");
#elif USB_MIC_ACT10_PROBE
    log_stage("a10_udc_left_disconnected_irq12_masked_reboot_required");
#else
    if (g_recovery_ok != 0u) {
        log_stage("system_usb_register_restore_check_ok_host_unverified");
    } else {
        log_stage("system_usb_register_restore_check_failed");
    }
#endif
#else
    log_value("irq12_left_masked", 1u);
#endif
#if USB_MIC_P15_PROBE
    log_stage("complete_reboot_required_before_usb");
    bda_msgbox(PROBE_TITLE, PROBE_COMPLETE_TEXT);
#else
    log_stage("complete_reboot_before_next_usb_test");
    bda_msgbox(PROBE_TITLE, PROBE_COMPLETE_TEXT);
#endif
    return 0;
}
