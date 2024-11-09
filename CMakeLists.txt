cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Build and .exe name
project(fluid-sim)
set(TargetName fluid-sim)

# Use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Set the name of the executable we want to build
add_executable(${TargetName})

# Mac or Linux specific setup
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Find SDL2 packages
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
    find_package(SDL2 REQUIRED COMPONENTS main)
    find_package(SDL2_image)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})
    # Link SDL2 packages
    target_link_libraries(${TargetName} PRIVATE ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

# Windows specific setup
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Find SDL2 packages
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image)
    # Link SDL2 packages
    target_link_libraries(${TargetName} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)
endif()

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
    )

target_sources(${TargetName} PRIVATE
    # .cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/SDLScene.cpp
    ${PROJECT_SOURCE_DIR}/src/Fluid.cpp
    ${PROJECT_SOURCE_DIR}/src/KeyboardManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Texture.cpp
    # .h
    ${PROJECT_SOURCE_DIR}/include/SDLScene.h
    ${PROJECT_SOURCE_DIR}/include/Fluid.h
    ${PROJECT_SOURCE_DIR}/include/KeyboardManager.h
    ${PROJECT_SOURCE_DIR}/include/Texture.h
    # ...
)