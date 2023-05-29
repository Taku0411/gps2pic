#pragma once
#include "header.hpp"

template <typename T>
std::vector<fs::path> arg2path(T &args)
{
  std::vector<fs::path> result;
  for (auto &i: args)
  {
    auto tmp = fs::path(i);
    //std::cout << tmp << std::endl;
    if (!fs::exists(tmp))
    {
      std::cerr << "no such file or directory: " << tmp << std::endl;
      std::abort();
    }
    result.push_back(tmp);
  }
  return result;
}
