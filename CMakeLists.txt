cmake_minimum_required(VERSION 3.20.0)
project(gps2pic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(gps2pic main.cpp IO.cpp)
target_include_directories(gps2pic PRIVATE ~/.local/include)
target_link_libraries(gps2pic PRIVATE )
