cmake_minimum_required(VERSION 3.20.0)
project(gps2pic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(json URL  https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


add_executable(gps2pic main.cpp IO.cpp)

find_package(exiv2 REQUIRED)
message(STATUS ${exiv2_LIBRARIES})
target_link_libraries(gps2pic PRIVATE nlohmann_json::nlohmann_json exiv2lib)

message(STATUS ${exiv2_LIBRARIES})

