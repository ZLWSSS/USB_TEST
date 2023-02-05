#ifndef MY_USB_MOTOR_
#define MY_USB_MOTOR_

#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <cstring>
#include "libusb-1.0/libusb.h"
#include "time.h"
#include "sys/select.h"


typedef struct{
    float q_abad[2];
    float q_hip[2];
    float q_knee[2];
    float qd_abad[2];
    float qd_hip[2];
    float qd_knee[2];
    int32_t flags[2];
    int32_t checksum;
} USB_IN_CMD_T;

typedef struct{
    float q_abad[2];
    float q_hip[2];
    float q_knee[2];
    float qd_abad[2];
    float qd_hip[2];
    float qd_knee[2];
    int32_t flags[2];
} USB_IN_DATA_T;

typedef struct{
    float q_des_abad[2];
    float q_des_hip[2];
    float q_des_knee[2];
    float qd_des_abad[2];
    float qd_des_hip[2];
    float qd_des_knee[2];
    float kp_abad[2];
    float kp_hip[2];
    float kp_knee[2];
    float kd_abad[2];
    float kd_hip[2];
    float kd_knee[2];
    float tau_abad_ff[2];
    float tau_hip_ff[2];
    float tau_knee_ff[2];
    int32_t flags[2];
    int32_t checksum;
} USB_OUT_CMD_T;

typedef struct{
    float q_des_abad[2];
    float q_des_hip[2];
    float q_des_knee[2];
    float qd_des_abad[2];
    float qd_des_hip[2];
    float qd_des_knee[2];
    float kp_abad[2];
    float kp_hip[2];
    float kp_knee[2];
    float kd_abad[2];
    float kd_hip[2];
    float kd_knee[2];
    float tau_abad_ff[2];
    float tau_hip_ff[2];
    float tau_knee_ff[2];
    int32_t flags[2];
}USB_OUT_DATA_T;

class my_usb_motor{
public:
    my_usb_motor(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in, unsigned char _endpoint_out);
    ~my_usb_motor();
    void Deal_in_transfer_data();
    void Deal_out_transfer_data();
    void usb_transfer_asy();
    void print_rx_data();
    static void in_callbackReceive(struct libusb_transfer* transfer);
    static void out_callbackReceive(struct libusb_transfer* transfer);
    const USB_IN_DATA_T* get_usb_in_data() const;

    uint8_t * tx_buff;
    uint8_t* rx_buff;
    libusb_context *ctx{};
private:
    int actual_length;
    uint16_t vendor_id;
    uint16_t product_id;
    long info_id;
    unsigned char endpoint_in;
    unsigned char endpoint_out;
    libusb_device_handle *device_Handle{};
    libusb_transfer* transfer_tx{};
    libusb_transfer* transfer_rx{};

    USB_IN_CMD_T * usb_in_cmd;
    USB_OUT_CMD_T * usb_out_cmd;
    USB_OUT_DATA_T* usb_out_data;
    USB_IN_DATA_T* usb_in_data;

    const int usb_message_in_length = 60;
    const int usb_message_out_length = 132;
    const int usb_message_in_checklength = 14;
    const int usb_message_out_checklength = 32;
    const int usb_message_32_2_8 = 33;
    const int usb_message_8_2_32 = 15;
};
inline void user_delay(const int delay_s, const int delay_us)
{
    struct timeval time;
    time.tv_sec = delay_s;
    time.tv_usec = delay_us;
    select(0, nullptr, nullptr, nullptr, &time);
}
#endif
