
cmake_minimum_required(VERSION 3.5)
project(a)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(a main1.cpp ready.cpp)





target_link_libraries(a ${OpenCV_LIBS})

