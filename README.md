# gps2pic
A software to embed GPS meta data from Google Maps takeout into picture.
Suppors new format of Google GPS data `location_history.json`.

## supported format
- JPG

## build environment
- MSVC 17(tested)
## dependencies
In windows, recomend installing them via vcpkg, otherwise you would spend much time to build exiv2. Triplets are in parences.
- `exiv2(exiv2:x64-windows)` 
   - `expat(expat:x64-windows)`
   - `inih(inih:x64-windows)`
- `nlohmann_json(nlohmann-json:x64-windows)`
- `argparse(argparse:x64-windows)`
- `date(date:x64-windows)`  


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
