cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(test6502)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON) 
if (USE_DEBUG)
 add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)


file(GLOB_RECURSE APP_SOURCES "app/*.[cs]")
file(GLOB_RECURSE C_SOURCES "library/*.[cs]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[cs]")


add_executable(test6502
	${APP_SOURCES} ${C_SOURCES}
	${COMMON_MODULE_SOURCES} ${USB_MODULE_SOURCES} ${DVI_MODULE_SOURCES} ${MODES_MODULE_SOURCES} ${MODES/_MODULE_SOURCES} ${GRAPHICS_MODULE_SOURCES} ${INPUT_MODULE_SOURCES}
)

target_link_libraries(test6502 PUBLIC
	pico_stdlib
	pico_sync
)

pico_add_extra_outputs(test6502)
