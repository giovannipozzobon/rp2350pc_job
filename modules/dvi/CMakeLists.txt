cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(dvi_out_hstx_encoder)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON)

if (USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)
include_directories(${MODULEDIR}/usb/include)

file(GLOB_RECURSE APP_SOURCES "app/*.[cs]")
file(GLOB_RECURSE DVI_C_SOURCES "library/*.[cs]")
file(GLOB_RECURSE USB_MODULE_SOURCES "${MODULEDIR}/usb/library/*.[cs]")
file(GLOB_RECURSE COMMON_MODULE "${MODULEDIR}common/library/*.[cs]")

add_executable(dvi_out_hstx_encoder
        ${APP_SOURCES} ${DVI_C_SOURCES} 
        ${COMMON_MODULE} ${USB_MODULE_SOURCES}
)

# pull in common dependencies
target_link_libraries(dvi_out_hstx_encoder
        pico_stdlib
        pico_multicore
        hardware_dma
        pico_sync
        tinyusb_host
        tinyusb_board    
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dvi_out_hstx_encoder)
