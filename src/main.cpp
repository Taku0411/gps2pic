#include <argparse/argparse.hpp>
#include <filesystem>
#include <gps.hpp>
#include <interpolate.hpp>
namespace fs = std::filesystem;


int main(int argc, char **argv)
{
  // parse args: https://github.com/p-ranav/argparse
  argparse::ArgumentParser program("gps2pic");
  program.add_argument("-d", "--dry")
          .help("dry run: not writing meta data, only showing estimated "
                "results")
          .default_value(false)
          .implicit_value(true);
  program.add_argument("-j", "--json")
          .required()
          .default_value("./location-history.json")
          .help("path to the location-history.json");
  program.add_argument("-t", "--timezone")
          .default_value(9)
          .help("time difference by UTC. default is +9 hours (JST).");
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
  auto timezone = program.get<int>("-t");

  // get the vector of input path(s)
  std::vector<fs::path> paths;
  for (const fs::directory_entry &x: fs::directory_iterator("./"))
  {
    auto item = x.path();
    if (item.extension() == ".jpg" || item.extension() == ".JPG")
      paths.push_back(item);
  }

  // check existence of location-history.json
  std::fflush(stdout);
  auto json_path = fs::canonical(fs::path(program.get<std::string>("-j")));
  if (!fs::exists(json_path))
  {
    std::cout << "location-history.json not found" << std::endl;
    std::abort();
  }

  // parse Records.json
  auto data = gps::parse_json(json_path);

  // create output directory
  if (!dry_run)
  {
    bool res = fs::create_directory("./output");
    if (!res)
    {
      throw std::runtime_error("output directory already exists\naborted");
      std::exit(1);
    }
  }

  // file loop for items
  for (auto &item: paths)
  {
    std::cout << item << std::endl;
    // check extension
    if (item.extension() == ".JPG" || item.extension() == ".jpg")
    {
      // set output name
      auto item_output = "output" / item.filename();
      if (dry_run) item_output = item;

      // print
      std::cout << std::string(69, '-') << std::endl;
      std::cout << item << std::endl;

      // copy file if not dry run
      if (!dry_run) fs::copy(item, item_output);

      // read image data
      Exiv2::Image::UniquePtr image =
              Exiv2::ImageFactory::open(item_output.string());
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();

      // time
      std::chrono::sys_seconds time = gps::getTime(exifData);
      std::cout << "original local time:" << time << std::endl;
      time -= std::chrono::hours(timezone);
      std::cout << "original UTC" << time << std::endl;

      // search interpolated time
      auto interpolated = geo::getInterpolatedGPSData(time, data);
      std::cout << "interpolated UTC" << interpolated << std::endl;

      // write
      exifData["Exif.GPSInfo.GPSLatitude"] = interpolated.getLatitude();
      exifData["Exif.GPSInfo.GPSLatitudeRef"] = interpolated.getLatitudeRef();
      exifData["Exif.GPSInfo.GPSLongitude"] = interpolated.getLongitude();
      exifData["Exif.GPSInfo.GPSLongitudeRef"] = interpolated.getLongitudeRef();
      if (!dry_run) image->writeMetadata();
    }
  }
  return 0;
}
