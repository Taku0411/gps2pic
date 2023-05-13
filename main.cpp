#include "header.hpp"


std::vector<fs::path> arg2path(int argc, char *argv[]);

int main(int argc, char *argv[])
{
  auto paths = arg2path(argc, argv);
	for(auto &item:paths)
	{
		if(item.extension() == ".jpeg")
		{
		}
		else if(item.extension() == ".CR2")
		{

		}

	}
  return 0;
}
