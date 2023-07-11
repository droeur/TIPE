# Tipe Cocheril-Crevecoeur Dimitri
[![CMake](https://github.com/Audic212/TIPE/actions/workflows/cmake.yml/badge.svg)](https://github.com/Audic212/TIPE/actions/workflows/cmake.yml)
## Build
Use Mingw64 or vs

## Launch
Options:
```
  -h [ --help ]               Help screen
  -m [ --map ] arg            Map location
  -p [ --parameter ] arg      Location of parameter files
  -n [ --no-graphic ]         Start without graphics
  -g [ --graphic-folder ] arg Graphic folder location
  -t [ --font ] arg           Font file
  -f [ --fast ]               Start without speed limitation
  -l [ --log ] arg            Log level
  -o [ --output ] arg         Output file
```
standard launch:
```
TIPE.exe -m ${projectDir}/map_empty.txt -g ${projectDir} -t ${projectDir}/cour.ttf -p ${projectDir}/expe.txt
```
Generate vs files:
```
cmake -B Builds -G "Visual Studio 17 2022" 
```
