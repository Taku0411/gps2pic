
#include <filesystem>
#include <fstream>
#include <gps.hpp>
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using json_value = nlohmann::basic_json<>::value_type;
namespace fs = std::filesystem;

namespace gps
{

  gpsdata::gpsdata(const std::chrono::sys_seconds time_,
                   const std::string point_)
  {
    // time
    time = time_;

    // coordinates
    auto coordinates_str = point_.substr(4);
    size_t commaPos = coordinates_str.find(',');

    // set
    point[0] = std::stod(coordinates_str.substr(0, commaPos));
    point[1] = std::stod(coordinates_str.substr(commaPos + 1));
  }

  gpsdata::gpsdata(const std::chrono::sys_seconds time_,
                   const std::array<double, 2> point_)
  {
    // time
    time = time_;

    // coordinates
    point = point_;
  }

  gpsdata::gpsdata(const std::chrono::sys_seconds time_)
  {
    // time
    time = time_;
  }

  std::string gpsdata::getLatitude()
  {
    double d = std::fabs(this->point[0]);
    int deg = static_cast<int>(d);
    d -= deg;
    d *= 60.0;
    int min = static_cast<int>(d);
    d -= min;
    d *= 60.0 * 100.0;
    double sec = d;

    char result[100];
    snprintf(result, sizeof(result), "%d/1 %d/1 %d/100", deg, min,
             static_cast<int>(sec));
    return std::string(result);
  }
  std::string gpsdata::getLatitudeRef()
  {
    return this->point[0] > 0 ? "N" : "S";
  }

  std::string gpsdata::getLongitude()
  {
    double d = std::fabs(this->point[1]);
    int deg = static_cast<int>(d);
    d -= deg;
    d *= 60.0;
    int min = static_cast<int>(d);
    d -= min;
    d *= 60.0 * 100;
    double sec = d;

    char result[100];
    snprintf(result, sizeof(result), "%d/1 %d/1 %d/100", deg, min,
             static_cast<int>(sec));
    return std::string(result);
  }

  std::string gpsdata::getLongitudeRef()
  {
    return this->point[1] > 0 ? "E" : "N";
  }

  std::ostream &operator<<(std::ostream &ostr, const gpsdata &data)
  {
    ostr << std::format("time: {}, point: {}, {}", data.time, data.point[0],
                        data.point[1]);
    return ostr;
  }

  std::vector<gpsdata> parse_json(const fs::path json_path)
  {

    // load json
    std::cout << "Reading location-history.json ... ";
    std::fflush(stdout);
    std::ifstream json_str(json_path);
    auto json = json::parse(json_str);
    std::cout << "OK" << std::endl;

    // start parse
    std::cout << "Parsing location-history.json ... ";
    std::fflush(stdout);
    std::vector<gpsdata> result;
    for (auto &block: json)
    {
      // skip if activity or visit type
      if (block.count("timelinePath") == 0) continue;

      // parse
      auto starttime_str = block["startTime"].get<std::string>();
      std::stringstream ss(starttime_str);
      std::chrono::sys_seconds starttime;
      // "2024-08-20T10:00:00.000Z"
      ss >> std::chrono::parse("%Y-%m-%dT%H:%M:%S", starttime);
      //std::cout << starttime << std::endl;

      auto timelinePath = block["timelinePath"];
      for (auto &entity: timelinePath)
      {
        auto point = entity["point"].get<std::string>();
        int durationMinutesOffsetFromStartTime =
                std::stoi(entity["durationMinutesOffsetFromStartTime"]
                                  .get<std::string>());
        std::chrono::minutes durationmin{durationMinutesOffsetFromStartTime};
        //std::cout << point << " " << durationmin << " " << starttime + durationmin
        //<< std::endl;
        result.emplace_back(starttime + durationmin, point);
      }
    }
    std::cout << "OK" << std::endl;
    return result;
  };

  std::chrono::sys_seconds getTime(Exiv2::ExifData &exifData)
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
      throw std::runtime_error("time stamp not found");
      std::exit(1);
    }
  }
};  // namespace gps
