#include "usb_imu.h"
#include "usb_motor.h"
#include <chrono>
const USB_IN_DATA_T* local_motor_data;
const uint16_t vendor_id1 = 0x1111;
const uint16_t product_id1 = 0x2222;
const unsigned char endpoint_1 = 0x81;

const uint16_t motor_vendor_id1 = 0x1234;
const uint16_t motor_product_id1 = 0x5678;
const unsigned char motor_endpoint_1 = 0x81;
const unsigned char motor_endpoint_2 = 0x01;
int main() {
    my_usb_motor my_motor_1(motor_vendor_id1,motor_product_id1,motor_endpoint_1);
    local_motor_data = my_motor_1.get_usb_in_data();
    std::cout << "run_here\n";
//    std::chrono::steady_clock::time_point t1_usb = std::chrono::steady_clock::now();
//    my_usb_1.usb_transfer_syc(rx_buf);
//    my_usb_1.uint8touint32(rx_buf);
//    my_usb_1.xor_checksum();
//    my_usb_1.print_rx_data();
    my_motor_1.usb_transfer_asy();
    std::cout << "run_here\n";
//    my_usb_motor my_usb_motor(motor_vendor_id1, motor_product_id1, motor_endpoint_1, motor_endpoint_2);
//    my_usb_motor.usb_transfer_asy();
//    std::chrono::steady_clock::time_point t2_usb = std::chrono::steady_clock::now();
//    std::chrono::duration<double, std::milli> time_used = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(t2_usb - t1_usb);
//    std::cout << " [Thread USB Cycle Time] " << time_used.count() << " ms" << std::endl;
    //}
    while(1)
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
        std::cout <<1;
        libusb_handle_events(my_motor_1.ctx);
    }
    return 0;
}
