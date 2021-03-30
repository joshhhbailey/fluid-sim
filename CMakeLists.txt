cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Project name
project(fluid-sim)
# This is the name of the Exe change this and it will change everywhere
set(TargetName fluid-sim)

# Use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Find SDL2 packages
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

# Set the name of the executable we want to build
add_executable(${TargetName})

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
    )

target_sources(${TargetName} PRIVATE
    # .cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/SDLScene.cpp
    ${PROJECT_SOURCE_DIR}/src/Fluid.cpp
    # .h
    ${PROJECT_SOURCE_DIR}/include/SDLScene.h
    ${PROJECT_SOURCE_DIR}/include/Fluid.h
    # ...
)

# Link SDL2 packages
target_link_libraries(${TargetName} PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${TargetName} PRIVATE SDL2::SDL2_image)
target_link_libraries(${TargetName} PRIVATE SDL2::SDL2_ttf)