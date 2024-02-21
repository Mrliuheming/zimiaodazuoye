cmake_minimum_required(VERSION 3.5)
project(a)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(a main.cpp ready.cpp)


target_link_libraries(a ${OpenCV_LIBS})
