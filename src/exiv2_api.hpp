#pragma once

#include "header.hpp"


namespace exiv2_api
{

  inline int getTimeZoneCR2(Exiv2::ExifData &exifData)
  {
    if (exifData["Exif.CanonTi.TimeZone"].count() != 0)
      return std::stoi(exifData["Exif.CanonTi.TimeZone"].toString());
    else
      return 0;
  }

  inline void addKey(const fs::path &path, const std::string &key,
                     const std::string &value)
  {
    Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(path);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    image->writeMetadata();
  }

  //https://exiv2.org/doc/geotag_8cpp-example.html
}  // namespace exiv2_api
