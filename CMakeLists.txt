cmake_minimum_required(VERSION 3.20.0)
project(gps2pic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(json argparse)


add_executable(gps2pic 
	src/main.cpp
)

find_package(exiv2 REQUIRED)
find_package(date REQUIRED)

target_link_libraries(gps2pic PRIVATE nlohmann_json::nlohmann_json exiv2lib date::date-tz argparse)
target_include_directories(gps2pic PRIVATE ${date_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

