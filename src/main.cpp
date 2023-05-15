#include "header.hpp"


int main(int argc, char *argv[])
{


	get_unix_time("2014-11-12T19:12:14.505Z");


	exit(1);
  // get path list of image files
  auto paths = arg2path(argc, argv);

  // load Records.json and parse file
	std::ifstream f("Records.json");
  auto jobj = json::parse(f);
	auto locations = jobj["locations"];
	for(auto loc:locations) std::cout << loc << std::endl;


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
