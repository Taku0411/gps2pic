#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <date/date.h>
#include <date/tz.h>
#include <sstream>


using tf = date::sys_time<std::chrono::seconds>;
namespace date_api
{

  class time
  {
  public:
    time(const std::string &iso8601, const std::string &fmt)
    {
      std::istringstream input(iso8601);
      input >> date::parse(fmt, tp);
      std::cout << tp.time_since_epoch().count() << std::endl;
      using namespace date;
      std::cout << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
      //std::cout << tp.time_since_epoch().count() << std::endl;
    };

    void applyTimeZoneM(int minute)
    {
      std::cout << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
      tp = tp - std::chrono::seconds(60 * minute);
      std::cout << date::format("%a, %b %d, %Y at %T %Z", tp) << std::endl;
      std::cout << tp.time_since_epoch().count() << std::endl;
    }

    auto getUnixTime() { return tp.time_since_epoch().count(); }


    bool operator>(const time &right) { return tp > right.tp; };
    bool operator<(const time &right) { return tp < right.tp; };


    tf tp;
  };


  //Exif.Photo.OffsetTimeOriginal: iPhone+9

}  // namespace date_api
