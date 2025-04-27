cmake_minimum_required(VERSION 3.30)
project(Lab5_Parallel_Computing)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab5_Parallel_Computing main.cpp HttpServer.cpp)
target_link_libraries(Lab5_Parallel_Computing ws2_32)