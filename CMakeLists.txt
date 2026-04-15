cmake_minimum_required(VERSION 3.23)

project(serial VERSION 1.0.0)

add_library(serial
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serial.cpp
)

target_include_directories(serial
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(serial PUBLIC cxx_std_17)

add_subdirectory(examples)