#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "header.hpp"


namespace date_api
{

  class time
  {
  public:
    time(){};
    time(const std::string &time, const std::string &fmt)
    {
      std::istringstream input(time);
      input >> date::parse(fmt, tp);
      //std::cout << tp.time_since_epoch().count() << std::endl;
      //std::cout << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
      //std::cout << tp.time_since_epoch().count() << std::endl;
    };

    void applyTimeZoneM(int minute, bool out=true)
    {
      tp = tp - std::chrono::seconds(60 * minute);
      if (out)
      {
        std::cout << "EXIF local timestamp "
                  << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
        std::cout << "EXIF UTC   timestamp "
                  << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
      }
    }

    auto getUnixTime() { return tp.time_since_epoch().count(); }

    int getTime() { return tp.time_since_epoch().count(); }


    bool operator>(const time &right) { return tp > right.tp; };
    bool operator<(const time &right) { return tp < right.tp; };
    auto operator-(const time &right) { return tp - right.tp; }

    tf tp;
  };

  inline time getTime(Exiv2::ExifData &exifData)
  {
    if (exifData["Exif.Photo.DateTimeOriginal"].count() != 0)
    {
      std::string time = exifData["Exif.Photo.DateTimeOriginal"].toString();
      date_api::time t(time, "%Y:%m:%d %T");
      return t;
    }
    else
    {
      std::cout << "time stamp not found" << std::endl;
      std::exit(1);
    }
  };


}  // namespace date_api
