#ifndef HARDWARE_USBDRIVERS_USB_H
#define HARDWARE_USBDRIVERS_USB_H

#define USB_MAX_EP_NUM 16
#define USB_MAX_INTF_NUM 16

typedef struct {
    uint8_t ep_num;
    uint8_t ep_type;
    uint16_t ep_size;
    uint8_t ep_direction;
    uint8_t ep_interval;
} usb_endpoint_t;

typedef struct {
    uint8_t intf_num;
    uint8_t intf_class;
    uint8_t intf_subclass;
    uint8_t intf_protocol;
    uint8_t intf_endpoint_num;
    usb_endpoint_t endpoints[USB_MAX_EP_NUM];
} usb_interface_t;

typedef struct {
    uint8_t vendor_id;
    uint8_t product_id;
    uint16_t bcd_device;
    uint8_t device_class;
    uint8_t device_subclass;
    uint8_t device_protocol;
    uint8_t max_packet_size;
    uint8_t num_configurations;
    uint8_t num_interfaces;
    usb_interface_t interfaces[USB_MAX_INTF_NUM];
} usb_device_descriptor_t;

void usb_init(void);
void usb_poll(void);

#endif