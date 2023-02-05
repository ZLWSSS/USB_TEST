#ifndef USB_TEST_H_
#define USB_TEST_H_
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "libusb-1.0/libusb.h"

const int K_CHECKSIZE_RECEIVED_FROM_STM = 10;
const int K_RX_WORDS_PER_MESSAGE = 44;
const int K_8_2_32_SIZE = 11;
const int K_MEMCPY_SIZE = 40;

typedef struct {
    float q[4];
    float gyro[3];
    float accel[3];
    uint32_t checksum;
} usb_imu_rx_cmd_t;

typedef struct {
    float q[4];
    float gyro[3];
    float accel[3];
} usb_imu_rx_data_t;

class my_usb_imu{
public:
    my_usb_imu(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint);
    ~my_usb_imu();
    void init_usb();
    void Deal_transfer_data(const uint8_t* rx_buff);
    void usb_transfer_syc(uint8_t* rx_buff);
    void usb_transfer_asy(uint8_t* rx_buff);
    void print_rx_data();
    static void callbackReceive(struct libusb_transfer* transfer);
    const usb_imu_rx_data_t* get_imu_rx_data();

    uint8_t* rx_buff;
    libusb_context *ctx{};
private:
    usb_imu_rx_cmd_t* imu_cmd;
    usb_imu_rx_data_t* imu_data;
    int actual_length;
    uint16_t vendor_id;
    uint16_t product_id;
    unsigned char endpoint;
    libusb_device_handle *device_Handle{};
    libusb_transfer* transfer{};
};
#endif


