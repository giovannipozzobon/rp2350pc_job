cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(dvi_out_hstx_encoder)

pico_sdk_init()

add_executable(dvi_out_hstx_encoder
        dvi_out_hstx_encoder.c
        )

# pull in common dependencies
target_link_libraries(dvi_out_hstx_encoder
        pico_stdlib
        pico_multicore
        hardware_dma
        pico_sync
        )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dvi_out_hstx_encoder)
