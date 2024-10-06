#pragma once

#include <exiv2.hpp>
#include <location_history.hpp>

namespace geo
{
  gpsdata getInterpolatedGPSData(const std::chrono::sys_seconds time,
                                 const std::vector<gpsdata> &data);
  bool writeGPSData(Exiv2::ExifData &exifData, gpsdata &data);
}  // namespace geo
