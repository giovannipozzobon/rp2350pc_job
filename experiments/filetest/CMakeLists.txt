cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(filetest)

pico_sdk_init()

include_directories(../include)
include_directories(${PICO_SDK_PATH}/lib/tinyusb/lib/fatfs/source)

add_executable(filetest
        hid_app.c
        main.c
        msc_app.c
        ${PICO_SDK_PATH}/lib/tinyusb/lib/fatfs/source/ff.c
        ${PICO_SDK_PATH}/lib/tinyusb/lib/fatfs/source/ffsystem.c
        ${PICO_SDK_PATH}/lib/tinyusb/lib/fatfs/source/ffunicode.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(filetest PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(filetest PUBLIC pico_stdlib tinyusb_host tinyusb_board)

pico_add_extra_outputs(filetest)

# add url via pico_set_program_url
#example_auto_set_url(filetest)

