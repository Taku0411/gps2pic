# gps2pic
A software to embed GPS meta data from Google takeout into picture.
Suppors new format of Google GPS data `location_history.json`.

## supported format
- JPG

## build environment
- MSVC 17.
## dependencies
- `exiv2` (and expat as its dependency)
- `nlohmann_json`
- `argparse`
note that all libaries can be installed via vcpkg
Much work would be required for builing exiv2 on windows. 


