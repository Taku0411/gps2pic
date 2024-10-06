#include "location_history.hpp"
#include <argparse/argparse.hpp>
#include <exiv2_api.hpp>
#include <geo_api.hpp>


int main(int argc, char **argv)
{
  // parse args: https://github.com/p-ranav/argparse
  argparse::ArgumentParser program("gps2pic");
  program.add_argument("-i", "--input")
          .required()
          .nargs(argparse::nargs_pattern::at_least_one)
          .help("specity input directory");
  program.add_argument("-d", "--dry")
          .help("dry run: not writing meta data, only showing estimated "
                "results")
          .default_value(false)
          .implicit_value(true);
  program.add_argument("-j", "--json")
          .default_value("./Records.json")
          .help("path to the Records.json");
  program.add_argument("-t", "--timezone")
          .default_value(9)
          .implicit_value(9)
          .help("time difference between UTC. default is 9hours (JST). If exif "
                "data contains timezone information, this value will be "
                "ignored.");
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

  // get arguments
  bool dry_run = program.get<bool>("-d");
  auto arg_paths = program.get<std::vector<std::string>>("-i");
  auto timezone = program.get<int>("-t");

  // get the vector of input path(s)
  std::vector<fs::path> paths;
  for (auto &i: arg_paths)
  {
    auto tmp = fs::path(i);
    if (!fs::exists(tmp))
    {
      std::cerr << "no such file or directory: " << tmp << std::endl;
      std::abort();
    }
    paths.push_back(tmp);
  }

  // check existence of location-history.json
  std::fflush(stdout);
  auto json_path = fs::canonical(fs::path(program.get<std::string>("-j")));
  if (!fs::exists(json_path))
  {
    std::cout << "Records.json not found" << std::endl;
    std::abort();
  }

  // parse Records.json
  auto data = parse_json(json_path);

  // file loop for items
  for (auto &item: paths)
  {
    std::cout << item << std::endl;
    // check extension
    if (item.extension() == ".CR2" || item.extension() == ".JPG")
    {
      // print
      std::cout << std::string(69, '-') << std::endl;
      std::cout << item << std::endl;

      // read image data
      Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(item.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();

      // time
      std::chrono::sys_seconds time = exiv2_api::getTime(exifData);
      std::cout << "original local time:" << time << std::endl;

      // get timezone if exists and convert to unix time
      auto exiftimezone = exiv2_api::getTimeZoneCR2(exifData);
      if (exiftimezone != -1000) timezone = exiftimezone;
      time -= std::chrono::hours(timezone);
      std::cout << "original UTC" << time << std::endl;

      // search interpolated time
      auto interpolated = geo::getInterpolatedGPSData(time, data);

      std::cout << "interpolated UTC" << interpolated << std::endl;
    }
  }
  return 0;
}
