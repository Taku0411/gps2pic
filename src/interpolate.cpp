#pragma once


#include <interpolate.hpp>

namespace geo
{
  gps::gpsdata getInterpolatedGPSData(const std::chrono::sys_seconds time,
                                      const std::vector<gps::gpsdata> &data)
  {
    gps::gpsdata dummy(time, std::array<double, 2>{0.0, 0.0});
    auto it_left = std::lower_bound(data.begin(), data.end(), dummy);
    auto left = *(it_left - 1);
    std::cout << left << std::endl;
    auto right = *(it_left);
    std::cout << right << std::endl;

    // interpolate
    std::chrono::sys_seconds time_triplet[3] = {time, left.time, right.time};
    double latitude[2] = {left.point[0], right.point[0]};
    double longitude[2] = {left.point[1], right.point[1]};
    double time_ratio = (double) ((time_triplet[2] - time_triplet[1]).count()) /
                        (double) time_triplet[0].time_since_epoch().count();

    dummy.point[0] = std::lerp(latitude[0], latitude[1], time_ratio);
    dummy.point[1] = std::lerp(longitude[0], longitude[1], time_ratio);
    return dummy;
  }
};  // namespace geo
