#include "usb_f407.h"
#include <chrono>

const usb_f407_in_data * local_motor_data;
const uint16_t f407_vendor_id = 0x1111;
const uint16_t f407_product_id = 0x2222;
const unsigned char f407_endpoint_in = 0x81;
const unsigned char f407_endpoint_out = 0x01;

int main() {
    my_f407 my_motor_1(f407_vendor_id,f407_product_id,f407_endpoint_in, f407_endpoint_out);
    local_motor_data = my_motor_1.get_in_data();
//    std::chrono::steady_clock::time_point t1_usb = std::chrono::steady_clock::now();
//    my_usb_1.usb_transfer_syc(rx_buf);
//    my_usb_1.uint8touint32(rx_buf);
//    my_usb_1.xor_checksum();
//    my_usb_1.print_rx_data();
    my_motor_1.usb_transfer_asy();

//    my_usb_motor my_usb_motor(motor_vendor_id1, motor_product_id1, motor_endpoint_1, motor_endpoint_2);
//    my_usb_motor.usb_transfer_asy();
//    std::chrono::steady_clock::time_point t2_usb = std::chrono::steady_clock::now();
//    std::chrono::duration<double, std::milli> time_used = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(t2_usb - t1_usb);
//    std::cout << " [Thread USB Cycle Time] " << time_used.count() << " ms" << std::endl;
    //}
    while(true)
    {
//        std::chrono::steady_clock::time_point t1_usb = std::chrono::steady_clock::now();
////        libusb_handle_events(my_usb_motor.ctx);
////        my_usb_motor.Deal_in_transfer_data();
////        my_usb_motor.print_rx_data();
//        user_delay(0,0);
//        std::chrono::steady_clock::time_point t2_usb = std::chrono::steady_clock::now();
//        std::chrono::duration<double, std::milli> time_used = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(t2_usb - t1_usb);
//        std::cout << " [Thread USB Cycle Time] " << time_used.count() << " ms" << std::endl;
        //imu
        libusb_handle_events(my_motor_1.ctx);
    }
    return 0;
}