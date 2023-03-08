#ifndef USB_F407_H_
#define USB_F407_H_
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "libusb-1.0/libusb.h"

const float Motor_V_Max = 425.f;
const float Motor_V_Min = -425.f;

typedef struct {
    float quaternion[4];
    float gyro[3];
    float accel[3];
    int32_t motor_speed_pack[2];
    int32_t servo_position_pack[2];
    uint32_t checksum;
} usb_f407_in_data;

typedef struct{
    float q[4];
    float gyro[3];
    float accel[3];
} usb_imu_data;

typedef struct{
    float speed_rmp[4];
    int16_t servo_position[4];
} usb_f407_motor_servo;

typedef struct {
    int32_t servo_position_pack[4];
    float motor_speed_pack[4];
    float motor_position_pack[4];
    uint32_t checksum;
} usb_f407_out_data;

class my_f407{
public:
    my_f407(uint16_t _vendor_id, uint16_t _product_id, unsigned char _inendpoint, unsigned char _outendpoint);
    ~my_f407();
    void Deal_in_transfer_data();
    void Deal_out_transfer_data();
    void usb_transfer_asy();
    void print_rx_data();
    void in_callback(struct libusb_transfer* transfer);
    void out_callback(struct libusb_transfer* transfer);
    const usb_f407_in_data* get_in_data();

    uint8_t* rx_buff;
    uint8_t * tx_buff;
    libusb_context *ctx{};
private:
    usb_f407_in_data* in_data;
    usb_f407_out_data* out_data;
    usb_f407_motor_servo* motor_servo_feedback;
    usb_imu_data* imu;
    int actual_length;
    long info_id;
    uint16_t vendor_id;
    uint16_t product_id;
    unsigned char in_endpoint;
    unsigned char out_endpoint;
    libusb_device_handle *device_Handle{};
    libusb_transfer* transfer_out{};
    libusb_transfer* transfer_in{};

    const int usb_in_data_checksize = 14;
    const int usb_in_data_size = 60;
    const int usb_in_data_8_2_32_size = 15;

    const int usb_out_data_checksize = 12;
    const int usb_out_data_size = 52;
    const int usb_out_data_32_2_8_size = 13;
};

void f407_in_cbf_wrapper(struct libusb_transfer* _transfer);
void f407_out_cbf_wrapper(struct libusb_transfer* _transfer);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
#endif


