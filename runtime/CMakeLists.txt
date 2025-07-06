cmake_minimum_required(VERSION 3.5)
project(runtime)

option(SDL_LOCATION "Use a specific location" OFF)
#
#       This forces RUNTIME compilation, which stops Pico libraries etc. being
#       installed.
#
add_compile_definitions(RUNTIME)
#
#       Include directory, one for each module.
#
include_directories(include)
include_directories(${MODULEDIR}/common/include)
include_directories(${MODULEDIR}/dvi/include)
include_directories(${MODULEDIR}/usb/include)
include_directories(${MODULEDIR}/input/include)
#
#       Add the SDL2 stuff
#
if(SDL_LOCATION)
    add_subdirectory(somewhere.sdl2.is EXCLUDE_FROM_ALL)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()
#
#       Runtime source.
#
file(GLOB_RECURSE RUNTIME_SOURCES "source/*.c")
#
#       The module/app that one wishes to actually run.
#
file(GLOB_RECURSE APP_SOURCES "${MODULEDIR}/usb/app/*.[cs]")
#
#       Any libraries out of the bottom interface tier than are required.
#
file(GLOB_RECURSE INPUT_LIB "${MODULEDIR}/input/library/*.[cs]")

add_executable(runtime 
    ${APP_SOURCES} ${RUNTIME_SOURCES}
    ${INPUT_LIB}
)

if(TARGET SDL2::SDL2main)
    target_link_libraries(runtime PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(runtime PRIVATE SDL2::SDL2)