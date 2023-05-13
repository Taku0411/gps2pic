#include "header.hpp"

std::vector<fs::path> arg2path(int argc, char *argv[])
{
  std::vector<fs::path> result;
  for (size_t i = 1; i < argc; i++)
  {
    auto tmp = fs::path(argv[i]);
		std::cout << tmp << std::endl;
    if (!fs::exists(tmp))
    {
      std::cerr << "no such file or directory: " << tmp << std::endl;
      std::abort();
    }
    result.push_back(tmp);
  }
  return result;
}
