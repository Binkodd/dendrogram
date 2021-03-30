cmake_minimum_required(VERSION 3.10)

project(dendogram)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(dendogram dendogram.cpp dendogram.h main.cpp)
target_link_libraries(opencv_app ${OpenCV_LIBS})