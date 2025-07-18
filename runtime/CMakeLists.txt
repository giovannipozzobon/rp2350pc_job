#
#       To add a module, add it to include_directories and non bottom tier interface libraries
#       Could 'appify' this with a skeleton ?
#
cmake_minimum_required(VERSION 3.5)
project(runtime)

option(SDL_LOCATION "Use a specific location" OFF)
#
#       This forces RUNTIME compilation, which stops Pico libraries etc. being
#       installed.
#
add_compile_definitions(RUNTIME)
add_compile_definitions(DEBUG)
#
#       Include directory, one for each module.
#
include_directories(include)
include_directories(${MODULEDIR}/common/include)
include_directories(${MODULEDIR}/dvi/include)
include_directories(${MODULEDIR}/usb/include)
include_directories(${MODULEDIR}/input/include)
include_directories(${MODULEDIR}/modes/include)
include_directories(${MODULEDIR}/graphics/include)
include_directories(${MODULEDIR}/console/include)
include_directories(${MODULEDIR}/sprites/include)
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
file(GLOB_RECURSE APP_SOURCES "${MODULEDIR}/sprites/app/*.[cs]")
#
#       Libraries out of the bottom interface tier than are required.
#
file(GLOB_RECURSE INPUT_LIB "${MODULEDIR}/input/library/*.[cs]")
file(GLOB_RECURSE MODES_LIB "${MODULEDIR}/modes/library/*.[cs]")
file(GLOB_RECURSE GRAPHICS_LIB "${MODULEDIR}/graphics/library/*.[cs]")
file(GLOB_RECURSE CONSOLE_LIB "${MODULEDIR}/console/library/*.[cs]")
file(GLOB_RECURSE SPRITES_LIB "${MODULEDIR}/console/library/*.[cs]")

add_executable(runtime 
    ${APP_SOURCES} ${RUNTIME_SOURCES}
    ${INPUT_LIB} ${MODES_LIB} ${GRAPHICS_LIB} ${CONSOLE_LIB} ${SPRITESLIB}
)

if(TARGET SDL2::SDL2main)
    target_link_libraries(runtime PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(runtime PRIVATE SDL2::SDL2)