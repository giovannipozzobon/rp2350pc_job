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
include_directories(${MODULEDIR}/console/include)
include_directories(${MODULEDIR}/dvi/include)
include_directories(${MODULEDIR}/modes/include)
include_directories(${MODULEDIR}/graphics/include)

file(GLOB_RECURSE APP_SOURCES "app/*.[cs]")
file(GLOB_RECURSE C_SOURCES "library/*.[cs]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[cs]")
file(GLOB_RECURSE CONMON_MODULE_SOURCES "${MODULEDIR}/console/library/*.[cs]")
file(GLOB_RECURSE DVI_MODULE_SOURCES "${MODULEDIR}/dvi/library/*.[cs]")
file(GLOB_RECURSE MODES_MODULE_SOURCES "${MODULEDIR}/modes/library/*.[cs]")
file(GLOB_RECURSE GRAPHICS_MODULE_SOURCES "${MODULEDIR}/graphics/library/*.[cs]")

add_executable(test6502
	${APP_SOURCES} ${C_SOURCES}
	${COMMON_MODULE_SOURCES} ${CONMON_MODULE_SOURCES} ${DVI_MODULE_SOURCES} ${MODES_MODULE_SOURCES} ${GRAPHICS_MODULE_SOURCES} ${INPUT_MODULE_SOURCES}
)

target_link_libraries(test6502 PUBLIC
	pico_stdlib
	pico_sync
	hardware_dma
)

pico_add_extra_outputs(test6502)
