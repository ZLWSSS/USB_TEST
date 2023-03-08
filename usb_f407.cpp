#include "usb_f407.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

my_f407::my_f407(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in, unsigned char _endpoint_out):
        vendor_id(_vendor_id),product_id(_product_id),in_endpoint(_endpoint_in), out_endpoint(_endpoint_out),info_id(0)
{
    actual_length = 0;
    in_data = new usb_f407_in_data() ;
    out_data = new usb_f407_out_data ();
    motor_servo_feedback = new usb_f407_motor_servo();
    imu  = new usb_imu_data ();
    rx_buff = new uint8_t[this->usb_in_data_size];
    tx_buff = new uint8_t [this->usb_out_data_size];
    transfer_in = libusb_alloc_transfer(0);
    transfer_out = libusb_alloc_transfer(0);

    libusb_init(&ctx);

    device_Handle = libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);

    if(libusb_kernel_driver_active(device_Handle, 0x00))
    {
        int success = libusb_detach_kernel_driver(device_Handle, 0x00);
        if(success != 0)
        {
            std::cerr << "Detach Driver Failed!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    int claim_interface = libusb_claim_interface(device_Handle, 0x00);
    if(claim_interface != 0)
    {
        std::cerr << "Claim Driver Failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cerr << "[MOTOR USB]: INITIALIZATION SUCCESS!" << std::endl;
}

my_f407::~my_f407() {
    delete [] rx_buff;
    delete [] tx_buff;
    delete in_data;
    delete out_data;
    std::cerr << "[Release devices]\n";
    libusb_free_transfer(transfer_in);
    libusb_free_transfer(transfer_out);
    libusb_release_interface(device_Handle, 0);
    libusb_close(device_Handle);
    libusb_exit(nullptr);
}

void my_f407::Deal_in_transfer_data() {
    for(int i = 0; i < (this->usb_in_data_8_2_32_size); i++)
    {
        ((uint32_t*)in_data)[i] = (rx_buff[4 * i + 3] << 24) | (rx_buff[4 * i + 2] << 16) | (rx_buff[4 * i + 1] << 8) | rx_buff[4 * i];
    }
    uint32_t t = 0;
    for (size_t i = 0; i < (this->usb_in_data_checksize); i++)
        t = t ^ ((uint32_t*)in_data)[i];
    if(in_data->checksum == t)
    {
        std::memcpy(imu, in_data, 40);
        motor_servo_feedback->speed_rmp[0] = uint_to_float((int16_t)(in_data->motor_speed_pack[0] >> 16), Motor_V_Min, Motor_V_Max, 16);
        motor_servo_feedback->speed_rmp[1] = uint_to_float((int16_t )(in_data->motor_speed_pack[0]), Motor_V_Min, Motor_V_Max, 16);
        motor_servo_feedback->speed_rmp[2] = uint_to_float((int16_t )(in_data->motor_speed_pack[1] >> 16), Motor_V_Min, Motor_V_Max, 16);
        motor_servo_feedback->speed_rmp[3] = uint_to_float((int16_t)(in_data->motor_speed_pack[1]), Motor_V_Min, Motor_V_Max, 16);
        motor_servo_feedback->servo_position[0] = (int16_t)(in_data->servo_position_pack[0] >> 16);
        motor_servo_feedback->servo_position[1] = (int16_t)((in_data->servo_position_pack[0]) & 0xffff);
        motor_servo_feedback->servo_position[2] = (int16_t)(in_data->servo_position_pack[1] >> 16);
        motor_servo_feedback->servo_position[3] = (int16_t)((in_data->servo_position_pack[1]) & 0xffff);
    } else {
        std::cerr << "[ERROR] USB RX CMD CHECKSUM ERROR!" << in_data->checksum << "\n";
    }
}

void my_f407::Deal_out_transfer_data()
{
    for(int i = 0; i < 4; i++)
    {
        out_data->motor_position_pack[i] = out_data->motor_speed_pack[i] = 10.f + (float)info_id;
        out_data->servo_position_pack[i] = 10 + (int)info_id;
    }
    uint32_t t = 0;
    for (size_t i = 0; i < (this->usb_out_data_checksize); i++)
        t = t ^ ((uint32_t*)out_data)[i];
    out_data->checksum = t;
    for(int i = 0; i < usb_out_data_32_2_8_size; i++)
    {
        tx_buff[4*i] = ((uint32_t*)out_data)[i];
        tx_buff[4*i + 1] = ((uint32_t*)out_data)[i] >> 8;
        tx_buff[4*i + 2] = ((uint32_t*)out_data)[i] >> 16;
        tx_buff[4*i + 3] = ((uint32_t*)out_data)[i] >> 24;
    }
}

void my_f407::usb_transfer_asy() {
    libusb_fill_interrupt_transfer(transfer_in, device_Handle, in_endpoint, rx_buff, usb_in_data_size,
                                   f407_in_cbf_wrapper, this, 0);
    libusb_fill_interrupt_transfer(transfer_out, device_Handle, out_endpoint, tx_buff, usb_out_data_size,
                                   f407_out_cbf_wrapper, this, 0);
    libusb_submit_transfer(transfer_in);
    if(transfer_in->status == 0)
    {
        std::cerr << "[Start Asynchronously receiving!]" << std::endl;
    }
    else {
        std::cerr << "[ERROR] Cannot start receiving" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void my_f407::print_rx_data() {
    info_id++;
    std::cout << info_id << " [" << imu->q[0] << "," << imu->q[1] << "," << imu->q[2] <<"," << imu->q[3] <<
    "] [" << imu->accel[0] <<"," << imu->accel[1] << "," << imu->accel[2] <<"] [" << imu->gyro[0] <<"," << imu->gyro[1] <<
    "," << imu->gyro[2] <<"] [" << motor_servo_feedback->speed_rmp[0] << " " << motor_servo_feedback->speed_rmp[1] << " " <<
    motor_servo_feedback->speed_rmp[2] << " " << motor_servo_feedback->speed_rmp[3] << "] [" << motor_servo_feedback->servo_position[0] << " " << motor_servo_feedback->servo_position[1] << " " <<
    motor_servo_feedback->servo_position[2] << " " << motor_servo_feedback->servo_position[3] << "]\n";
}

void my_f407::in_callback(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cerr << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        this->Deal_in_transfer_data();
        this->print_rx_data();
        libusb_submit_transfer(transfer_out);
    }
}

void my_f407::out_callback(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cerr << "[ERROR] Asy Out Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        this->Deal_out_transfer_data();
        libusb_submit_transfer(transfer_in);
    }
}

const usb_f407_in_data *my_f407::get_in_data() {
    return in_data;
}

void f407_in_cbf_wrapper(struct libusb_transfer* _transfer)
{
    auto* temp = reinterpret_cast<my_f407*>(_transfer->user_data);
    temp->in_callback(_transfer);
}

void f407_out_cbf_wrapper(struct libusb_transfer* _transfer)
{
    auto* temp = reinterpret_cast<my_f407*>(_transfer->user_data);
    temp->out_callback(_transfer);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}