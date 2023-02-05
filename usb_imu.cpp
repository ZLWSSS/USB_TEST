#include <cstring>
#include "usb_imu.h"

my_usb_imu::my_usb_imu(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint):
vendor_id(_vendor_id), product_id(_product_id), endpoint(_endpoint), actual_length(0) {
    imu_cmd = new usb_imu_rx_cmd_t ;
    imu_data = new usb_imu_rx_data_t ;
    rx_buff = new uint8_t[K_RX_WORDS_PER_MESSAGE];
    libusb_init(&ctx);
}

my_usb_imu::~my_usb_imu() {
    delete imu_cmd;
    delete imu_data;
    delete [] rx_buff;
    std::cout << "[Release devices]\n";
    libusb_free_transfer(transfer);
    libusb_release_interface(device_Handle, 0);
    libusb_close(device_Handle);
    libusb_exit(nullptr);
}

void my_usb_imu::init_usb()
{
    device_Handle = libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);
    if(libusb_kernel_driver_active(device_Handle, 0x00))
    {
        int success = libusb_detach_kernel_driver(device_Handle, 0x00);
        if(success != 0)
        {
            std::cout << "Detach Driver Failed!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    int claim_interface = libusb_claim_interface(device_Handle, 0x00);
    if(claim_interface != 0)
    {
        std::cout << "Claim Driver Failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void my_usb_imu::usb_transfer_syc(uint8_t* _rx_buff)
{
    //TODO:: Synchronous to Asynchronous
    libusb_bulk_transfer(device_Handle, endpoint, _rx_buff, K_RX_WORDS_PER_MESSAGE, &actual_length, 1000);
}

void my_usb_imu::usb_transfer_asy(uint8_t* _rx_buff)
{
    transfer = libusb_alloc_transfer(0);
    libusb_fill_bulk_transfer(transfer, device_Handle, endpoint, _rx_buff, K_RX_WORDS_PER_MESSAGE,
                              my_usb_imu::callbackReceive, nullptr, 0);
    libusb_submit_transfer(transfer);
    if(transfer->status == 0)
    {
        std::cout << "[Start Asynchronously transmitting!]" << std::endl;
    }
    else {
        std::cout << "[ERROR] Cannot start transmitting" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void my_usb_imu::print_rx_data()
{
    std::cout << "[IMU DATA] [" << imu_data->q[0] << " " << imu_data->q[1] << " " <<
    imu_data->q[2] << " " << imu_data->q[3] << "] [" << imu_data->gyro[0] << " " <<
    imu_data->gyro[1] << " " << imu_data->gyro[2] << "] [" << imu_data->accel[0] << " " <<
    imu_data->accel[1] << " " << imu_data->accel[2] << "]" << std::endl;
}

const usb_imu_rx_data_t* my_usb_imu::get_imu_rx_data()
{
    return imu_data;
}

void my_usb_imu::callbackReceive(struct libusb_transfer *transfer) {
    //std::cout << "[Enter CB Function]\n";
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cout << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        libusb_submit_transfer(transfer);
    }
}

void my_usb_imu::Deal_transfer_data(const uint8_t *_rx_buff) {
    for(int i = 0; i < K_8_2_32_SIZE; i++)
    {
        ((uint32_t*)imu_cmd)[i] = (_rx_buff[4 * i + 3] << 24) + (_rx_buff[4 * i + 2] << 16) + (_rx_buff[4 * i + 1] << 8) + _rx_buff[4 * i];
    }

    auto* temp_cmd = (uint32_t*)imu_cmd;
    uint32_t t = 0;
    for (size_t i = 0; i < K_CHECKSIZE_RECEIVED_FROM_STM; i++)
        t = t ^ temp_cmd[i];
    if(imu_cmd->checksum == t)
    {
        memcpy(imu_data, imu_cmd, K_MEMCPY_SIZE);
    } else {
        std::cout << "[ERROR] USB RX CMD CHECKSUM ERROR!\n";
    }
}
