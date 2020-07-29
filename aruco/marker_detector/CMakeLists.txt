cmake_minimum_required(VERSION 3.16)
project(untitled2)

set(CMAKE_CXX_STANDARD 98)

find_package(OpenCV REQUIRED)

add_executable(untitled2 main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(untitled2 ${OpenCV_LIBS}) # replace binary_name