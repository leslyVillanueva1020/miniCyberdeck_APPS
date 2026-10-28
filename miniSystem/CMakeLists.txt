cmake_minimum_required(VERSION 3.16)

project(miniSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(
    miniSystem
    main.cpp
    systemWindow.cpp 
    systemWindow.h
)

target_link_libraries(
    miniSystem
    PRIVATE
    Qt6::Widgets
)