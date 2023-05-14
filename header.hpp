#include <exiv2/exiv2.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class IMGInfo
{
  std::string time_stamp;
  std::string altitude;
  std::string longigutde;
};

std::vector<fs::path> arg2path(int argc, char *argv[]);
void read_date(Exiv2::Image::UniquePtr image);
