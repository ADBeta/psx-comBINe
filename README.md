# psx-comBINe V4!
![logo](/psx-comBINe.jpg?raw=true)

## What's new?
* Improved Command Line handler library
* Improved Cue Handler Library
* Increased speed around 10x - Thanks to [@mark-dawn](https://github.com/mark-dawn)
* Multiple bug fixes - mainly bug that deleted all files in parent directory
* Program can now handle taking a directory as input, finding a .cue file on its own
* Removed recreate mode - was not very useful
* Program is now compiled in -O2 mode, and is stress tested and profiled for efficiency
* Modernised the codebase to be easier to read and maintain

## What's next?
I want to create a GUI version of this program using wxWidgets so it can be 
easily ported to Windows and Linux. This will be included in Version 5

## Decription
**psx-comBINe** is a cross-platform program to combine multi-bin CD-ROM
PSX Games into a single `.CUE` and `.BIN` pair. It supports all CUE `MODES`,
`Remarks,` 99 `TRACKS`, with 99 `INDEX`s each, support for multi-session dumps, and more.  

**32-bit and 64-bit exes for Windows Systems are in the release files.**

psx-comBINe is designed to be lightweight and fast, with considerations to 
support the use of this program inside other projects - like emulators.  
Simplicity is also a main goal of psx-comBINe.  

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
* C++17 or higher Compiler
* 32-bit or higher architecture

## Compilation
### Linux
The easiest way to install psx-comBINe is to use git clone:
``` sh
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
sudo make install
```
This will copy the the psx-combine executable to your `/usr/local/bin/` folder.  
If you do not want to install to your system, use  
```sh
make
```
This will output the psx-combine executable to the the `/psx-comBINe/bin`
directory  

### Windows
Compiling on Windows has not been tested extensively; as such there is no standard
method, compile using your own preffered methods.  
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

## Usage
See the [Usage file](/Usage.md) 
for detailed usage notes and instructions  
Alternatively, use the inbuilt help  
`psx-combine --help`

## Speed Tests
```
Test Game    Size         Tracks
-----------------------------------
Rayman       701,881 KB   51 Tracks
WipEout      505,151 KB   9 Tracks
-----------------------------------

PSX Game      Average time     Data Speed
-------------------------------------------
Windows 10
64bit, AMD Ryzen 7 5700X:
-------------------------------------------
Rayman        01.51s          464,821 KiB/s
WipEout       01.44s          350,799 KiB/s

Arch Linux
64bit, AMD Ryzen 7 5700X:
-------------------------------------------
Rayman        00.85s          825,742 KiB/s
WipEout       00.85s          594,295 KiB/s

Debian 8 (jessie)
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        10.14s           69,219 KiB/s
WipEout       16.55s           30,522 KiB/s

Windows 7
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        18.94s           37,058 KiB/s
WipEout       18.20s           27,755 KiB/s
```
----
**ADBeta (c) 2023-2024**  
This software is under the GPL 2.0 Licence, please see LICENCE for information
