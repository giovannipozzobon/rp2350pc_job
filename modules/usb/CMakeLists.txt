cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(filetest)

pico_sdk_init()

include_directories(include)

file(GLOB_RECURSE USB_C_SOURCES "usb/*.c")

add_executable(filetest
        main.c ${USB_C_SOURCES}
)

target_link_libraries(filetest PUBLIC pico_stdlib tinyusb_host tinyusb_board)

pico_add_extra_outputs(filetest)

# add url via pico_set_program_url
#example_auto_set_url(filetest)

