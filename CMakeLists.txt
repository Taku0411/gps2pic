cmake_minimum_required(VERSION 3.20.0)
project(gps2pic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(expat CONFIG REQUIRED)
find_package(exiv2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(argparse REQUIRED)
add_executable(gps2pic
  src/main.cpp
  src/location_history.cpp
  src/geo_api.cpp
  )

target_link_libraries(gps2pic PRIVATE nlohmann_json::nlohmann_json argparse::argparse Exiv2::exiv2lib)
target_include_directories(gps2pic PRIVATE ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src)
