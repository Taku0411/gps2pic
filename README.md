# gps2pic
A software to embed GPS meta data from Google Maps takeout into picture.
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

## how to use
1. Get `location_history.json` from your Smart phone and transfer it to PC.
2. in the directory where *.JPG exists, run the follwoing command.
  ~~~
  gps2pic -j <path to location_history.json> [--dry] [--timezone +9]
  ~~~
  * `-j`: path to location_history.json
  * `--dry`: without output result files, just get console stdout
  * `--timezone`: time zone when the photos are taken. Ex in JP, --timezone +9

  `output` directory will be created in cwd, and resuls JPG files are exported there.
