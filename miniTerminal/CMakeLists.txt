cmake_minimum_required(VERSION 3.16)

project(miniTerminal)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(
    miniTerminal
    main.cpp
    terminalWindow.cpp
    terminalWindow.h
)

target_link_libraries(
    miniTerminal
    PRIVATE
    Qt6::Widgets
)