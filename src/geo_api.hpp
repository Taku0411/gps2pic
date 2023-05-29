#pragma once

#include <format>

#include "date_api.hpp"
#include "exiv2_api.hpp"
#include "header.hpp"

namespace geo
{

  struct geo
  {
    geo(json_value &j) : time(j["timestamp"], "%FT%T")
    {
      latitudeE7_ = j["latitudeE7"].get<double>() / 1.0e7;
      longitudeE7_ = j["longitudeE7"].get<double>() / 1.0e7;
      if (j.contains("altitude")) altitude_ = j["altitude"].get<double>();
      if (j.contains("accuracy")) accuracy_ = j["accuracy"].get<double>();
    };
    geo(date_api::time &time) : time(time){};

    std::string latitude()
    {
      double d = std::fabs(latitudeE7_);
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
    };

    std::string latitudeRef() { return latitudeE7_ > 0 ? "N" : "S"; }

    std::string longitude()
    {
      double d = std::fabs(longitudeE7_);
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

    std::string longitudeRef() { return longitudeE7_ > 0 ? "E" : "W"; }

    std::string altitude()
    {
      char result[50];
      std::snprintf(result, sizeof(result), "%d/1",
                    static_cast<int>(altitude_));
      return std::string(result);
    }

    std::string altitudeRef() { return (altitude_ > 0) ? "0" : "1"; }

    std::string accuracy()
    {
      char result[50];
      std::snprintf(result, sizeof(result), "%d/1",
                    static_cast<int>(accuracy_));
      return std::string(result);
    }

    void overwrite_geo(Exiv2::Image::UniquePtr &image,
                       Exiv2::ExifData &ExifData);


    date_api::time time;
    double latitudeE7_{};       // 経度
    double longitudeE7_{};      // 緯度
    double altitude_ = -256.0;  // 高度 (m)
    int accuracy_ = 0;          // 精度
  };

  std::ostream &operator<<(std::ostream &stream, const geo &geo)
  {
    stream << std::format("{}\nlatitude: {:3.7f}, longitude: {:3.7f}, "
                          "altitude: {:3.7f}, "
                          "accuracy: {}",
                          date::format("%a, %b %d, %Y at %T %Z", geo.time.tp),
                          geo.latitudeE7_, geo.longitudeE7_, geo.altitude_,
                          geo.accuracy_)
           << std::endl;
    return stream;
  }


  geo interpolate(date_api::time &key, geo &data1, geo &data2)
  {
    int time[3] = {key.getTime(), data1.time.getTime(), data2.time.getTime()};
    double latitude[2] = {data1.latitudeE7_, data2.latitudeE7_};
    double longitude[2] = {data1.longitudeE7_, data2.longitudeE7_};
    double time_ratio = ((double) time[2] - time[1]) / time[0];

    geo result(key);
    result.latitudeE7_ =
            std::lerp(data1.latitudeE7_, data2.latitudeE7_, time_ratio);
    result.longitudeE7_ =
            std::lerp(data1.longitudeE7_, data2.longitudeE7_, time_ratio);
    if (data1.accuracy_ > 100 && data2.accuracy_ < 100) return data2;
    else if (data1.accuracy_ < 100 && data2.accuracy_ > 100)
      return data1;
    else
    {
      if (data1.altitude_ == -256.0 && data2.altitude_ == -256.0)
        result.altitude_ = -256.0;
      else if (data1.altitude_ == -256.0)
        result.altitude_ = data2.altitude_;
      else if (data2.altitude_ == -256.0)
        result.altitude_ = data1.altitude_;
      else
      {
        result.altitude_ =
                std::lerp(data1.altitude_, data2.altitude_, time_ratio);
      }
      result.accuracy_ = std::max(data1.accuracy_, data2.accuracy_);
      return result;
    };
  };

  void geo::overwrite_geo(Exiv2::Image::UniquePtr &image,
                          Exiv2::ExifData &exifData)
  {
    // apply geo tag
    if (accuracy_ < 100)
    {
      exifData["Exif.GPSInfo.GPSLatitude"] = latitude();
      exifData["Exif.GPSInfo.GPSLatitudeRef"] = latitudeRef();

      exifData["Exif.GPSInfo.GPSLongitude"] = longitude();
      exifData["Exif.GPSInfo.GPSLongitudeRef"] = longitudeRef();

      if (altitude_ != -256.0)
      {
        exifData["Exif.GPSInfo.GPSAltitude"] = altitude();
        exifData["Exif.GPSInfo.GPSAltitudeRef"] = altitudeRef();
        exifData["Exif.GPSInfo.GPSHPositioningError"] = accuracy();
        std::cout << "altitude is available" << std::endl;
      }
      std::cout << "lati: " << latitude() << std::endl;
      std::cout << "long: " << longitude() << std::endl;
      std::cout << "accu: " << accuracy() << std::endl;
      image->writeMetadata();
    }
    else { std::cout << "file is ignored due to bad accuracy." << std::endl; }
  }
  inline geo getGeo(date_api::time &key, json_value &locations)
  {
    // dummy time
    date_api::time tmp("1970-02-24T14:25:14.900Z", "%FT%T");
    size_t res_index;
    for (size_t index = 0; index < locations.size(); index++)
    {
      auto &l = locations.at(index);
      auto t = date_api::time(l["timestamp"], "%FT%T");
      if (std::chrono::abs(t - tmp) > std::chrono::abs(t - key))
      {
        tmp = t;
        res_index = index;
      }
    }
    geo mae(locations.at(res_index - 1));
    geo usiro(locations.at(res_index));
    std::cout << mae << std::endl;
    std::cout << usiro << std::endl;
    return interpolate(key, mae, usiro);
  };

}  // namespace geo
