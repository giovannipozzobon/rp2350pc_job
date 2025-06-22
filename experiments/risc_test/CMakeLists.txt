cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(risc_test)

pico_sdk_init()

include_directories(../include)

add_executable(risc_test
        risc_test.c
        )

target_link_libraries(risc_test pico_stdlib)

pico_add_extra_outputs(risc_test)

