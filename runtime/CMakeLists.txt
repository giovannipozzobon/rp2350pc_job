cmake_minimum_required(VERSION 3.5)
project(runtime)

option(runtime_VENDORED "Use vendored libraries" OFF)

include_directories(include)
include_directories(${MODULEDIR}/common/include)

if(runtime_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

file(GLOB_RECURSE APP_SOURCES "source/*.c")

add_executable(runtime ${APP_SOURCES})

if(TARGET SDL2::SDL2main)
    target_link_libraries(runtime PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(runtime PRIVATE SDL2::SDL2)