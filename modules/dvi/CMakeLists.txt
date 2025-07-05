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

file(GLOB_RECURSE APP_SOURCES "app/*.[cs]")
file(GLOB_RECURSE DVI_C_SOURCES "source/*.[cs]")
file(GLOB_RECURSE COMMON_MODULE "${MODULEDIR}common/source/*.[cs]")

add_executable(dvi_out_hstx_encoder
        ${APP_SOURCES} ${DVI_C_SOURCES} 
        ${COMMON_MODULE}
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
