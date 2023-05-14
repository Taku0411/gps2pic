#include "header.hpp"



int main(int argc, char *argv[])
{
  auto paths = arg2path(argc, argv);
  for (auto &item: paths)
  {
    if (item.extension() == ".jpeg") {}
    else if (item.extension() == ".CR2" || item.extension() == ".JPG")
    {

      Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(item.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();

      exifData["Exif.GPSInfo.GPSAltitude"] = std::string("4/1 15/1 33/1");
      exifData["Exif.GPSInfo.GPSLatitude"] = std::string("4/1 15/1 33/1");
      exifData["Exif.GPSInfo.GPSLongitude"] = std::string("4/1 15/1 33/1");

      image->writeMetadata();
    }
  }
  return 0;
}
