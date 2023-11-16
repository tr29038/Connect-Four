cmake_minimum_required(VERSION 3.22)
project(connect_four VERSION 1.0)

include_directories(connect_four ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(connect_four
    main.cpp
    src/board.h
    src/board.cpp
)
