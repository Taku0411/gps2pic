#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <date/date.h>
#include <date/tz.h>
#include <sstream>

namespace date_api
{

  class time
  {
  public:
    time();
    time(const std::string &iso8601)
    {
      std::istringstream input(iso8601);
      input >> date::parse("%FT%T", tp);
      //std::cout << tp.time_since_epoch().count() << std::endl;
    };

    void getUnix() { std::cout << date::make_zoned(date::current_zone(), tp); }
    auto getUnixTime() { return tp.time_since_epoch().count(); }


    bool operator>(const time &right) { return tp > right.tp; };
    bool operator<(const time &right) { return tp < right.tp; };


    date::sys_seconds tp;
  };

  inline time closest(std::vector<time> &vec, time &key)
  {
    time result("3000-01-01T00:00:00.000Z");
    auto key_time = key.getUnixTime();
    for (auto i: vec)
    {
      if (std::fabs(key.getUnixTime() - i.getUnixTime()) < result.getUnixTime())
        result = i;
    }
    return result;
  };

//Exif.Photo.OffsetTimeOriginal: iPhone+9

}  // namespace date_api
