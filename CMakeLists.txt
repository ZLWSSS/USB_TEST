cmake_minimum_required(VERSION 3.5)
project(USBTEST)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

set(CMAKE_CXX_STANDARD 14)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
add_executable(USBTEST main.cpp usb_f407.cpp)
target_link_libraries(USBTEST libusb-1.0.so)

