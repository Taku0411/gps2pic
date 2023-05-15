#pragma onece

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <date/date.h>
#include <exiv2/exiv2.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

class IMGInfo
{
  std::string time_stamp;
  std::string altitude;
  std::string longigutde;
};

std::vector<fs::path> arg2path(int argc, char *argv[]);
void read_date(Exiv2::Image::UniquePtr image);
date::sys_time<std::chrono::seconds> get_unix_time(const std::string &time_str);
