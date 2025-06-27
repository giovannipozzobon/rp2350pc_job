cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(filetest)

pico_sdk_init()

include_directories(include)

add_executable(filetest
        main.c
        usb/usb_handler.c
        usb/hid_handler.c
        usb/msc_handler.c
        usb/support.c
        usb/filedirect.c
        usb/directory.c
        usb/file.c
        usb/ff.c
        usb/ffsystem.c
        usb/ffunicode.c
)

target_link_libraries(filetest PUBLIC pico_stdlib tinyusb_host tinyusb_board)

pico_add_extra_outputs(filetest)

# add url via pico_set_program_url
#example_auto_set_url(filetest)

