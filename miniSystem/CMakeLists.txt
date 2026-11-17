cmake_minimum_required(VERSION 3.16)

project(miniSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(
    miniSystem
    main.cpp
    systemWindow.cpp
    systemWindow.h
    miniSystem.qrc
)

target_link_libraries(
    miniSystem
    PRIVATE
    Qt6::Widgets
    Qt6::Network
)