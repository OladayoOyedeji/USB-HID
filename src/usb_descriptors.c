// File: usb_descriptors.c
// Author: Oladayo Oyedeji

tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,

    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 64,

    .idVendor = 0xCafe,

    .idProduct = 0x4004,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x00,
    .iProduct = 0x00,
    .iSerialNumber = 0x00,

    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void)
{
    return (uint8_t const *) &desc_device;
}

uint8_t const desc_hid_report[] = {
    0x06, 0x00, 0xFF,
    0x09, 0x01,
    0xA1, 0x01,

    0x09, 0x02,
    0x16, 0x00, 0x80,
    0x26, 0xFF, 0x7F,
    0x75, 0x10,
    0x95, 0x01,
    0x81, 0x02,

    0x09, 0x03,
    0x81, 0x02,

    0x09, 0x04,
    0x81, 0x02,

    0xC0
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x80, 100),

    TUD_HID_DESCIPTOR(0, 0, HID_ITF_PROTOCOL_NONE,
                      sizeof(desc_hid_report), 0x81, 64, 1)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    return desc_configuration;
}

uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const * buffer, uint16_t bufsize)
{
    (void) itf; (void) report_id; (void) report_type; (void) bufsize;
}
