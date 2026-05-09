# Copyright (c) 2026 yasukioo
# Author: yasukioo <yasukioo@outlook.com>

cmake_minimum_required(VERSION 3.21)

project(QModbusUdp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(QMODBUSUDP_BUILD_TESTS "Build QModbusUdp tests" ON)
option(QMODBUSUDP_BUILD_EXAMPLES "Build QModbusUdp examples" ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

add_library(QModbusUdp SHARED)
add_library(QModbusUdp::QModbusUdp ALIAS QModbusUdp)

target_compile_definitions(QModbusUdp PRIVATE QMODBUSUDP_LIBRARY)
target_include_directories(QModbusUdp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(QModbusUdp PUBLIC Qt6::Core Qt6::Network)

add_subdirectory(src)

if(QMODBUSUDP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(QMODBUSUDP_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 6.5 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()
