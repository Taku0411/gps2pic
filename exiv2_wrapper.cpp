#include "header.hpp"

void addKey(const fs::path &path, const std::string &key, const std::string &value)
{
	Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(path);
	image->readMetadata();
	Exiv2::ExifData& exifData = image->exifData();
	
	image->writeMetadata();
}
//https://exiv2.org/doc/geotag_8cpp-example.html
