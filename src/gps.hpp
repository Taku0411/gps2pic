#pragma once

#include <array>
#include <chrono>
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <vector>

namespace gps
{
  struct gpsdata
  {
    gpsdata(const std::chrono::sys_seconds time_, const std::string point_);
    gpsdata(const std::chrono::sys_seconds time_,
            const std::array<double, 2> point_);
    gpsdata(const std::chrono::sys_seconds time_);

    std::array<double, 2> point;  // ido, keido / latitude, longitude
    std::chrono::sys_seconds time;


    // operator overload
    bool operator<(const gpsdata &another) const
    {
      return time < another.time;
    };

    // latitude
    std::string getLatitude();
    std::string getLatitudeRef();

    std::string getLongitude();
    std::string getLongitudeRef();
  };

  std::ostream &operator<<(std::ostream &ostr, const gpsdata &data);
  std::vector<gpsdata> parse_json(const std::filesystem::path json_path);

  std::chrono::sys_seconds getTime(Exiv2::ExifData &exifData);
};  // namespace gps

/////////////////////////////////////////////////////////
template<typename Ty>
std::ostream &operator<<(std::ostream &ostr, const std::vector<Ty> &v)
{
  if (v.empty())
  {
    ostr << "{ }";
    return ostr;
  }
  ostr << "{" << v.front();
  for (auto itr = ++v.begin(); itr != v.end(); itr++) { ostr << ", " << *itr; }
  ostr << "}";
  return ostr;
}

template<typename Ty, std::size_t N>
std::ostream &operator<<(std::ostream &ostr, const std::array<Ty, N> &v)
{
  if (v.empty())
  {
    ostr << "{ }";
    return ostr;
  }
  ostr << "{" << v.front();
  for (auto itr = v.begin() + 1; itr != v.end(); itr++)
  {
    ostr << ", " << *itr;
  }
  ostr << "}";
  return ostr;
}
/////////////////////////////////////////////////////////
