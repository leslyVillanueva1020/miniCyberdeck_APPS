cmake_minimum_required(VERSION 3.16)

project(MiniNotes)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(
    MiniNotes
    main.cpp
    NotesWindow.cpp
    NotesWindow.h
    NotebookTextEdit.cpp
    NotebookTextEdit.h
)

target_link_libraries(
    MiniNotes
    PRIVATE
    Qt6::Widgets
)