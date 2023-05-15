cmake_minimum_required(VERSION 3.20.0)
project(gps2pic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git)
FetchContent_MakeAvailable(json)
FetchContent_Populate(date GIT_REPOSITORY https://github.com/HowardHinnant/date.git)


add_executable(gps2pic 
	src/main.cpp
	src/IO.cpp
	src/googletakeout_parser.cpp)

find_package(exiv2 REQUIRED)
target_link_libraries(gps2pic PRIVATE nlohmann_json::nlohmann_json exiv2lib)
target_include_directories(gps2pic PRIVATE ${date_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

