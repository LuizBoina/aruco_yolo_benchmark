cmake_minimum_required(VERSION 3.16)
project(untitled2)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(/usr/local/include/aruco/)
link_directories(/usr/local/lib/)

add_executable(untitled3 main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(untitled3 ${OpenCV_LIBS} aruco) # replace binary_name