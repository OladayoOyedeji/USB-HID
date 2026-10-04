// File: tusb_config.h
// Author: Oladayo Oyedeji

#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif
    
#define CFG_TUSB_MCU  OPT_MCU_STM32F4
#define CFG_TUSB_OS
#define CFG_TUSB_RHPORTO_MODE

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN
#endif

#define CFG_TUD_ENDPOINT0_SIZE

#define CFG_TUD_HID
#define CFG_TUD_CDC
#define CFG_TUD_MSC
#define CFG_TUD_MIDI
#define CFG_TUD_VENDOR
    
#define CFG_TUD_HID_EP_BUFSIZE
    
#ifdef __cplusplus
}
#endif

#endif
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),  // 18
    .bDescriptorType    = TUSB_DESC_DEVICE,             // 0x01
    .bcdUSB             = 0x0200,                        // USB 2.0

    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = 64,

    .idVendor           = 0xCafe,
    .idProduct          = 0x4004,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x80, 100),

    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE,
                        sizeof(desc_hid_report), 0x81, 64, 1)
    //                  ^report desc size      ^EP addr ^size ^interval(ms)
};

uint8_t const desc_hid_report[] = {
    0x06, 0x00, 0xFF,        // Usage Page (Vendor Defined)
    0x09, 0x01,              // Usage 1
    0xA1, 0x01,              // Collection (Application)

    0x09, 0x02,              //   Usage (X)
    0x16, 0x00, 0x80,        //   Logical Minimum (-32768)
    0x26, 0xFF, 0x7F,        //   Logical Maximum (32767)
    0x75, 0x10,              //   Report Size (16 bits)
    0x95, 0x01,              //   Report Count (1)
    0x81, 0x02,              //   Input (X axis)

    0x09, 0x03,              //   Usage (Y)
    0x81, 0x02,              //   Input (Y axis) — reuses min/max/size/count above

    0x09, 0x04,              //   Usage (Z)
    0x81, 0x02,              //   Input (Z axis)

    0xC0                      // End Collection
};
