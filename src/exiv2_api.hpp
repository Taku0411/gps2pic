#pragma once

#include <chrono>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
namespace exiv2_api
{

  inline int getTimeZoneCR2(Exiv2::ExifData &exifData)
  {
    if (exifData["Exif.CanonTi.TimeZone"].count() != 0)
      return std::stoi(exifData["Exif.CanonTi.TimeZone"].toString()) / 60;
    else
      return -1000;
  }

  inline auto getTime(Exiv2::ExifData &exifData)
  {
    if (exifData["Exif.Photo.DateTimeOriginal"].count() != 0)
    {
      std::stringstream ss(exifData["Exif.Photo.DateTimeOriginal"].toString());
      std::chrono::sys_seconds time;
      ss >> std::chrono::parse("%Y:%m:%d %T", time);
      return time;
    }
    else
    {
      std::cout << "time stamp not found" << std::endl;
      std::exit(1);
    }
  }

  inline void addKey(const fs::path &path, const std::string &key,
                     const std::string &value)
  {
    auto image = Exiv2::ImageFactory::open(path.string());
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    image->writeMetadata();
  }

  //https://exiv2.org/doc/geotag_8cpp-example.html
}  // namespace exiv2_api
