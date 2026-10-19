cmake_minimum_required(VERSION 3.16)

project(CyberdeckLauncher)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(
    CyberdeckLauncher
    main.cpp
    HomeWindow.cpp
    HomeWindow.h
)

target_link_libraries(
    CyberdeckLauncher
    PRIVATE
    Qt6::Widgets
)