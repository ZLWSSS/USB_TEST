#ifndef MY_USB_MOTOR_
#define MY_USB_MOTOR_

#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <cstring>
#include "libusb-1.0/libusb.h"
#include "time.h"
#include "sys/select.h"
#include <fstream>
#include <iomanip>
extern int8_t stop_experiment;
typedef struct{
    float q_abad;
    float q_hip;
    float q_knee;
    float qd_abad;
    float qd_hip;
    float qd_knee;
    float c_abad;
    float c_hip;
    float c_knee;
    int32_t stop_number;
    int32_t checksum;
} USB_IN_CMD_T;

typedef struct{
    float q_abad;
    float q_hip;
    float q_knee;
    float qd_abad;
    float qd_hip;
    float qd_knee;
    float c_abad;
    float c_hip;
    float c_knee;
    int32_t stop_number;
} USB_IN_DATA_T;

void motor_cbf_wrapper(struct libusb_transfer* _transfer);

class my_usb_motor{
public:
    my_usb_motor(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in);
    ~my_usb_motor();
    void Deal_in_transfer_data();
    void usb_transfer_asy();
    void print_rx_data();
    void in_callbackReceive(struct libusb_transfer* transfer);
    const USB_IN_DATA_T* get_usb_in_data() const;
    uint8_t* rx_buff;
    libusb_context *ctx{};
private:
    int actual_length;
    uint16_t vendor_id;
    uint16_t product_id;
    long info_id;
    unsigned char endpoint_in;
    libusb_device_handle *device_Handle{};
    libusb_transfer* transfer_rx{};

    USB_IN_CMD_T * usb_in_cmd;
    USB_IN_DATA_T* usb_in_data;

    const int usb_message_in_length = 44;
    const int usb_message_in_checklength = 10;
    const int usb_message_8_2_32 = 11;
    std::ofstream out_txt_file;
};
inline void user_delay(const int delay_s, const int delay_us)
{
    struct timeval time;
    time.tv_sec = delay_s;
    time.tv_usec = delay_us;
    select(0, nullptr, nullptr, nullptr, &time);
}
#endif
