#include "usb_motor.h"

my_usb_motor::my_usb_motor(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in, unsigned char _endpoint_out):
        vendor_id(_vendor_id),product_id(_product_id),endpoint_in(_endpoint_in),endpoint_out(_endpoint_out),info_id(0){
    actual_length = 0;
    usb_in_cmd = new USB_IN_CMD_T ;
    usb_out_cmd = new USB_OUT_CMD_T;
    usb_out_data = new USB_OUT_DATA_T;
    usb_in_data = new USB_IN_DATA_T;
    rx_buff = new uint8_t[this->usb_message_in_length];
    tx_buff = new uint8_t[this->usb_message_out_length];
    transfer_tx = libusb_alloc_transfer(0);
    transfer_rx = libusb_alloc_transfer(0);

    libusb_init(&ctx);

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
    std::cout << "[MOTOR USB]: INITIALIZATION SUCCESS!" << std::endl;
}

my_usb_motor::~my_usb_motor() {
    delete [] tx_buff;
    delete [] rx_buff;
    delete usb_out_cmd;
    delete usb_out_data;
    delete usb_in_cmd;
    delete usb_in_data;
    std::cout << "[Release devices]\n";
    libusb_free_transfer(transfer_tx);
    libusb_free_transfer(transfer_rx);
    libusb_release_interface(device_Handle, 0);
    libusb_close(device_Handle);
    libusb_exit(nullptr);
}

void my_usb_motor::Deal_in_transfer_data() {
    for(int i = 0; i < (this->usb_message_8_2_32); i++)
    {
        ((uint32_t*)usb_in_cmd)[i] = (rx_buff[4 * i + 3] << 24) + (rx_buff[4 * i + 2] << 16) + (rx_buff[4 * i + 1] << 8) + rx_buff[4 * i];
    }

    auto* temp_cmd = (uint32_t*)usb_in_cmd;
    uint32_t t = 0;
    for (size_t i = 0; i < (this->usb_message_in_checklength); i++)
        t = t ^ temp_cmd[i];
    if(usb_in_cmd->checksum == t)
    {
        memcpy(usb_in_data, usb_in_cmd, 4*usb_message_in_checklength);
    } else {
        std::cout << "[ERROR] USB RX CMD CHECKSUM ERROR!\n";
    }
}

void my_usb_motor::Deal_out_transfer_data() {
    memcpy(usb_out_cmd, usb_out_data, 4*usb_message_out_checklength);
    uint32_t t_temp = 0;
    for (size_t i = 0; i < (this->usb_message_out_checklength); i++)
        t_temp = t_temp ^ ((uint32_t*)usb_out_data)[i];
    usb_out_cmd->checksum = t_temp;

    for(int i = 0; i < (this->usb_message_32_2_8); i++)
    {
        tx_buff[4 * i] = ((uint32_t*)usb_out_cmd)[i];
        tx_buff[4 * i + 1] = ((uint32_t*)usb_out_cmd)[i] >> 8;
        tx_buff[4 * i + 2] = ((uint32_t*)usb_out_cmd)[i] >> 16;
        tx_buff[4 * i + 3] = ((uint32_t*)usb_out_cmd)[i] >> 24;
    }

    auto* temp_cmd = (uint32_t*)usb_in_cmd;
    uint32_t t = 0;
    for (size_t i = 0; i < (this->usb_message_in_checklength); i++)
        t = t ^ temp_cmd[i];
    if(usb_in_cmd->checksum == t)
    {
        memcpy(usb_in_data, usb_in_cmd, 4*usb_message_in_checklength);
    } else {
        std::cout << "[ERROR] USB RX CMD CHECKSUM ERROR!\n";
    }

}

void my_usb_motor::usb_transfer_asy() {
    libusb_fill_bulk_transfer(transfer_tx, device_Handle, endpoint_out, tx_buff, usb_message_out_length,
                              my_usb_motor::out_callbackReceive, nullptr, 0);
    libusb_submit_transfer(transfer_tx);
    if(transfer_tx->status == 0)
    {
        std::cout << "[Start Asynchronously transmitting!]" << std::endl;
    }
    else {
        std::cout << "[ERROR] Cannot start transmitting" << std::endl;
        exit(EXIT_FAILURE);
    }

    libusb_fill_bulk_transfer(transfer_rx, device_Handle, endpoint_in, rx_buff, usb_message_in_length,
                              my_usb_motor::in_callbackReceive, nullptr, 0);
    libusb_submit_transfer(transfer_rx);
    if(transfer_tx->status == 0)
    {
        std::cout << "[Start Asynchronously receiving!]" << std::endl;
    }
    else {
        std::cout << "[ERROR] Cannot start receiving" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void my_usb_motor::print_rx_data() {
    info_id++;
    std::cout << "[Motor DATA " << info_id <<"] [" << usb_in_data->q_abad[0] << " " << usb_in_data->q_abad[1] << " " <<
    usb_in_data->q_hip[0] << " " << usb_in_data->q_hip[1] << usb_in_data->q_knee[0] << " " << usb_in_data->q_knee[1] << "] [" <<
    usb_in_data->qd_abad[0] << " " << usb_in_data->qd_abad[1] << " " <<
    usb_in_data->qd_hip[0] << " "
    << usb_in_data->qd_hip[1] << " " << usb_in_data->qd_knee[0] << " " << usb_in_data->qd_knee[1] << "]\n";
}

void my_usb_motor::in_callbackReceive(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cout << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        //deal_data();
        libusb_submit_transfer(transfer);
    }
}

void my_usb_motor::out_callbackReceive(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cout << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        //deal_data();
        libusb_submit_transfer(transfer);
    }
}

const USB_IN_DATA_T *my_usb_motor::get_usb_in_data() const {
    return usb_in_data;
}
