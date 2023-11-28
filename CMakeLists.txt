cmake_minimum_required(VERSION 3.26)
project(WindowsClientTest1)

set(CMAKE_CXX_STANDARD 23)

add_executable(WindowsClientTest1
        ClipboardManager.cpp
        ClipboardManager.h
        main.cpp)
