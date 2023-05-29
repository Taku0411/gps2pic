#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <date/date.h>
#include <date/tz.h>
#include <exiv2/exiv2.hpp>
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using json_value = nlohmann::basic_json<>::value_type;
using tf = date::sys_time<std::chrono::milliseconds>;
namespace fs = std::filesystem;
