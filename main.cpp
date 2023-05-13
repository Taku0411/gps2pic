#include "header.hpp"


std::vector<fs::path> arg2path(int argc, char *argv[]);

int main(int argc, char *argv[])
{
  auto paths = arg2path(argc, argv);
  for (auto &item: paths)
  {
    if (item.extension() == ".jpeg") {}
    else if (item.extension() == ".CR2")
    {

      Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(item.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();

      exifData["Exif.GPSInfo.GPSAltitude"] = std::string("4/1 15/1 33/1");
      exifData["Exif.GPSInfo.GPSAltitudeRef"] = std::string("N");

      image->writeMetadata();
    }
  }
  return 0;
}
