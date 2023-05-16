#include "IO.hpp"
#include "date_api.hpp"
#include "header.hpp"
#include <cstdio>


int main(int argc, char *argv[])
{
  // get path list of image files
  std::cout << "Parding arguments ... ";
  std::fflush(stdout);
  auto paths = arg2path(argc, argv);
  std::cout << "OK" << std::endl;

  // check existense of Records.json
  std::cout << "Opening Records.json ... ";
  std::fflush(stdout);
  if (!fs::exists("Records.json"))
  {
    std::cout << "Records.json not found" << std::endl;
    std::abort();
  }
  std::cout << "OK" << std::endl;

  // load Records.json and parse file
  std::cout << "Parsing Records.json ... ";
  std::fflush(stdout);
  std::ifstream f("Records.json");
  auto jobj = json::parse(f);
  auto locations = jobj["locations"];
  std::vector<date_api::time> dates;
  //for (auto loc: locations)
  //{
  //  dates.emplace_back(date_api::time(loc["timestamp"], "%FT%T"));
  //}
  std::cout << "OK" << std::endl;


  for (auto &item: paths)
  {
    if (item.extension() == ".CR2")
    {
      std::cout << item << std::endl;

      Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(item.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();
      if (exifData["Exif.Photo.DateTimeOriginal"].count() != 0)
      {
        std::string time = exifData["Exif.Photo.DateTimeOriginal"].toString();
        std::cout << "EXIF: " << time << std::endl;
				date_api::time t(time, "%Y:%m:%d %T");
				if(exifData["Exif.CanonTi.TimeZone"].count() != 0)
				{
					int tz = std::stoi(exifData["Exif.CanonTi.TimeZone"].toString());
					t.applyTimeZoneM(tz);
				}
      }
			std::string s = "2023:05:12 11:13:57";
      exifData["Exif.GPSInfo.GPSAltitude"] = std::string("4/1 15/1 33/1");
      exifData["Exif.GPSInfo.GPSLatitude"] = std::string("4/1 15/1 33/1");
      exifData["Exif.GPSInfo.GPSLongitude"] = std::string("4/1 15/1 33/1");

      image->writeMetadata();
    }
  }
  return 0;
}
