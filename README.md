# psx-comBINe V5!!
![logo](/images/psx-comBINe.jpg?raw=true)

## What's new?
* Fixed issue with dark theme under Linux
* Changed Makefile under Linux to be dynamic linked
* Ensured Cue file generation is correct cross-platform

## Thanks!
A huge thank you to the following contributors, for their efforts to make 
psx-comBINe even better  
(Not in Chronological order)
* [FormerlyZeroCool](https://github.com/FormerlyZeroCool) for porting to C++17 and std::filesystem
* [tiaghun](https://github.com/tiaghun) for fixing libboost linking
* [mark-dawn](https://github.com/mark-dawn) for improving the file writing algorithm
* [Kargjen](https://github.com/Karojen) for finding an issue with themes under Linux
* [sahlberg](https://github.com/sahlberg) for ideas on general improvements to the application
* [SneakyyPeakyy](https://github.com/SneakyyPeakyy) for finding bugs with libboost

![ui](/images/UI.jpg?raw=true)

## Decription
**psx-comBINe** is a lightweight and fast, cross-platform program to combine
multi-bin CD-ROM PSX Games into a single `.CUE` and `.BIN` pair. It supports
all CUE `MODES`, `Remarks,` 99 `TRACKS`, with 99 `INDEX`s each, support for 
multi-session dumps, and more.  

**32-bit and 64-bit exe's for Windows Systems are in the release files.**

psx-comBINe supports the CUE sheet specifications;
* Supports all of the GNU approved CUE `TYPE`s
* `REM` lines
* a max of 99 `TRACK`s and 99 `INDEX`s (as per CUE spec)
* Repairs slightly malformed inputs
* Ensures proper byte-alignment for the CUE Specifications

**NOTE** This program is in no way intended to support or condone piracy. 
This program should only be used with legitimately acquired backups of disks 
you own **LEGALLY**  

## Minimum Requirments  
* 4KB Free RAM
* 32-bit or higher architecture

## Compilation
### Requirments
To Compile this project you will need:
* wxWidgets
* mingw (If using Linux to compile for Windows)
* wxWidgets for mingw (If using Linux to compile for Windows)
### Linux
The easiest way to install psx-comBINe for other architectures is to use 
git clone:
``` sh
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
sudo make install
```
This will copy the psx-combine executable to your `/usr/local/bin/` folder.  
If you do not want to install to your system, use  
```sh
make
```
This will output the psx-combine executable to the `/psx-comBINe/bin`
directory  

### Windows
Compiling on Windows has not been tested extensively; as such there is no standard
method, compile using your own preferred methods.  
**32-bit and 64-bit .exe files are in the release files**, you can use these 
instead of compiling the program manually.  

**WARNING** This is not thoroughly tested, use at your own risk:  
The Makefile includes a Windows target, which compiles on Linux using mingw.  
```sh
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
make win32
# or
make win64
```

----
**ADBeta (c) 2023-2024**  
This software is under the GPL 2.0 Licence, please see LICENCE for information
