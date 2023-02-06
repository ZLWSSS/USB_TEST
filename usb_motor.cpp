#include "usb_motor.h"

int8_t stop_experiment = 0;

my_usb_motor::my_usb_motor(uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in):
        vendor_id(_vendor_id),product_id(_product_id),endpoint_in(_endpoint_in),info_id(0){
    actual_length = 0;
    usb_in_cmd = new USB_IN_CMD_T() ;
    usb_in_data = new USB_IN_DATA_T();
    rx_buff = new uint8_t[this->usb_message_in_length];
    transfer_rx = libusb_alloc_transfer(0);

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

    out_txt_file.open("motor_feedback_data.txt", std::ios::out | std::ios::trunc);
    out_txt_file << std::setprecision(6);
    std::cerr << "[MOTOR USB]: INITIALIZATION SUCCESS!" << std::endl;
}

my_usb_motor::~my_usb_motor() {
    delete [] rx_buff;
    delete usb_in_cmd;
    delete usb_in_data;
    std::cerr << "[Release devices]\n";
    libusb_free_transfer(transfer_rx);
    libusb_release_interface(device_Handle, 0);
    libusb_close(device_Handle);
    libusb_exit(nullptr);
    out_txt_file.close();
}

void my_usb_motor::Deal_in_transfer_data() {
    for(int i = 0; i < (this->usb_message_8_2_32); i++)
    {
        ((uint32_t*)usb_in_cmd)[i] = (rx_buff[4 * i + 3] << 24) | (rx_buff[4 * i + 2] << 16) | (rx_buff[4 * i + 1] << 8) | rx_buff[4 * i];
    }
    uint32_t t = 0;
    for (size_t i = 0; i < (this->usb_message_in_checklength); i++)
        t = t ^ ((uint32_t*)usb_in_cmd)[i];
    if(usb_in_cmd->checksum == t)
    {
        memcpy(usb_in_data, usb_in_cmd, 4*usb_message_in_checklength);
    } else {
        std::cerr << "[ERROR] USB RX CMD CHECKSUM ERROR!\n";
    }
}

void my_usb_motor::usb_transfer_asy() {
    libusb_fill_interrupt_transfer(transfer_rx, device_Handle, endpoint_in, rx_buff, usb_message_in_length,
                              motor_cbf_wrapper, this, 0);
    libusb_submit_transfer(transfer_rx);
    if(transfer_rx->status == 0)
    {
        std::cerr << "[Start Asynchronously receiving!]" << std::endl;
    }
    else {
        std::cerr << "[ERROR] Cannot start receiving" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void my_usb_motor::print_rx_data() {
    info_id++;
    std::cout << "[Motor DATA " << info_id <<"] [" << usb_in_data->q_abad << " " << usb_in_data->q_hip << " " <<
    usb_in_data->q_knee << "] [" << usb_in_data->qd_abad << " "<<usb_in_data->qd_hip << " " << usb_in_data->qd_knee << "] [" <<
    usb_in_data->c_abad << " " << usb_in_data->c_hip << " " <<
    usb_in_data->c_knee << "]\n";
    out_txt_file << info_id <<" " << usb_in_data->q_abad << " " << usb_in_data->q_hip << " " <<
                 usb_in_data->q_knee << " " << usb_in_data->qd_abad << " "<<usb_in_data->qd_hip << " "
                 << usb_in_data->qd_knee << " " << usb_in_data->c_abad << " " << usb_in_data->c_hip << " " <<
                 usb_in_data->c_knee << "\n";
    if(usb_in_data->stop_number)
    {
        stop_experiment = 1;
    }
}

void my_usb_motor::in_callbackReceive(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cerr << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        this->Deal_in_transfer_data();
        this->print_rx_data();
        libusb_submit_transfer(transfer);
    }
}

const USB_IN_DATA_T *my_usb_motor::get_usb_in_data() const {
    return usb_in_data;
}

void motor_cbf_wrapper(struct libusb_transfer* _transfer)
{
    auto* temp = reinterpret_cast<my_usb_motor*>(_transfer->user_data);
    temp->in_callbackReceive(_transfer);
}
