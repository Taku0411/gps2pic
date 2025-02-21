#pragma once

#include <exiv2/exiv2.hpp>
#include <gps.hpp>

namespace geo
{
  gps::gpsdata getInterpolatedGPSData(const std::chrono::sys_seconds time,
                                      const std::vector<gps::gpsdata> &data);
  bool writeGPSData(Exiv2::ExifData &exifData, gps::gpsdata &data);
}  // namespace geo
