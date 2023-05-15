#include "date/date.h"
#include "header.hpp"

date::sys_time<std::chrono::seconds> get_unix_time(const std::string &time_str)
{
  std::istringstream ss(time_str);;
  date::sys_time<std::chrono::seconds> timestamp;
  ss >> date::parse("%FT%TXZ", timestamp);
	std::cout << timestamp << std::endl;
}
