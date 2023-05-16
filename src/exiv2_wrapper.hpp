#pragma once
#include "header.hpp"


void read_date(Exiv2::Image::UniquePtr image)
{
//https://exiv2.org/examples.html
  image->readMetadata();
  Exiv2::ExifData &exifData = image->exifData();
  Exiv2::ExifKey date_key = "Exif.Photo.DateTimeOriginal";
  exifData.findKey(date_key);
}

void addKey(const fs::path &path, const std::string &key,
            const std::string &value)
{
  Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(path);
  image->readMetadata();
  Exiv2::ExifData &exifData = image->exifData();

  image->writeMetadata();
}
//https://exiv2.org/doc/geotag_8cpp-example.html
