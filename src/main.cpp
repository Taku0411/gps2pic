#include "IO.hpp"
#include "date_api.hpp"
#include "exiv2_api.hpp"
#include "geo_api.hpp"
#include "header.hpp"
#include <argparse/argparse.hpp>


int main(int argc, char **argv)
{
  // parse args: https://github.com/p-ranav/argparse
  argparse::ArgumentParser program("gps2pic");
  program.add_argument("-i", "--input")
          .required()
          .nargs(argparse::nargs_pattern::at_least_one)
          .help("specity input directory");
  program.add_argument("-d", "--dry")
          .help("dry run")
          .default_value(false)
          .implicit_value(true);
  program.add_argument("-j", "--json")
          .default_value("./Records.json")
          .help("path to the Records.json");
  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }
  bool dry_run = program.get<bool>("-d");
  auto arg_paths = program.get<std::vector<std::string>>("-i");
  auto paths = arg2path(arg_paths);

  // check existense of Records.json
  std::cout << "Opening Records.json ... ";
  std::fflush(stdout);
  auto json_path = fs::path(program.get<std::string>("-j"));
  if (!fs::exists(json_path))
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
  std::cout << "OK" << std::endl;


  // file loop for items
  for (auto &item: paths)
  {
    std::cout << item << std::endl;
    // check extension
    if (item.extension() == ".CR2")
    {
      // print
      std::cout << std::string(69, '-') << std::endl;
      std::cout << item << std::endl;

      // construct
      date_api::time time;

      // read image data
      Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(item.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();

      // get time from exif data
      time = date_api::getTime(exifData);

      // get timezone and convert to unix time
      auto time_zone = exiv2_api::getTimeZoneCR2(exifData);
      time.applyTimeZoneM(time_zone);

      // search closes time
      auto loc = geo::getGeo(time, locations);
      std::cout << "result" << std::endl;
      std::cout << loc << std::endl;

      if (!dry_run) loc.overwrite_geo(image, exifData);

      // also overwrite JPG in upper directory
      fs::path jpg = ".." / item.replace_extension("JPG");
      if (fs::exists(jpg))
      {
        //Exiv2::Image::UniquePtr image_jpg = Exiv2::ImageFactory::open(jpg);
        image->readMetadata();
        Exiv2::ExifData &exifData_jpg = image->exifData();
        if (!dry_run) loc.overwrite_geo(image, exifData_jpg);
      }
    }
  }
  return 0;
}
