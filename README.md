# psx-comBINe V4!
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

![logo](https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

I have made a [YouTube video](https://www.youtube.com/watch?v=gBMUwBJaiMM) about
psx-comBINe  
(**NOTE** This video is outdated and doesn't cover all
of the available functions. Use the 
[Usage file](https://github.com/ADBeta/psx-comBINe/blob/main/Usage.md) and the 
information in this README until I make a new video)

## Decription
**psx-comBINe** combines a multi-bin CD-ROM PSX Game into a single .CUE and .BIN
pair and supports all CUE MODES, Remarks, 99 TRACKS, with 99 INDEXS each, 
support for multi-session dumps, and more.  

**32bit and 64bit exes for Windows Systems are in the release files.**

psx-comBINe is designed to be lightweight and fast; one of the main design goals
with this CLI version is to allow it to be ported into other projects - such as 
emulators so that multi-bin files can be played witout having to first combine 
the files, but instead letting psx-comBINe handle that, output files to a temp
folder, and allow the emulator to play them from there.

The goal of this is to make the PSX scene even easier to get into, without
having to know how or why you need to combine .bin & .cue files.  

psx-comBINe has excellent compatibility with the CUE sheet standards;
* It can support any of the GNU approved CUE TYPEs
* REM lines
* a max of 99 TRACKs and 99 INDEXs (as per CUE spec)
* Repairs slightly malformed inputs.
* Performs checksums and safety check on the CUE index and byte offsets

**NOTE** This program is in no way intended to support or condone piracy. 
This program should only be used with legitimately acquired backups of disks 
you own **LEGALLY**  

## Requirments
The minimum requirments for this program are:  
* C++17 or higher Compiler
* 32 or 64 bit Architecture

## Compilation
#### Linux
The easiest way to install psx-comBINe is to use git clone:
``` sh
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
sudo make install
```
This will copy the the psx-combine executable to your `/usr/local/bin/` folder.  
If you do not want this to happen you can use  
```sh
make
```
This will output the psx-combine executable to the the `/psx-comBINe/bin`
directory  

### Windows
I will not go into detail about this, as it is personal preference, however:  
**32 bit and 64 bit .exe files are in the release files** you can use these 
instead of compiling the program manually.  

If you wish to compile the code for Windows anyway, you can use a make flag in
linux (mingw):  
```sh
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
make win32
# or
make win64
```

You can also manually compile the code in Windows however you prefer.

## Usage
See the [Usage file](https://github.com/ADBeta/psx-comBINe/blob/main/Usage.md) 
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
Rayman                         61,246 KiB/s TODO
WipEout       05.91s           85,473 KiB/s

Arch Linux
64bit, AMD Ryzen 7 5700X:
-------------------------------------------
Rayman        00.85s          825,742 KiB/s
WipEout       00.85s          594,295 KiB/s

Debian 8 (jessie)
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        45.33s           15,483 KiB/s
WipEout       31.55s           16,011 KiB/s

Windows 7
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        18.94s           37,058 KiB/s
WipEout       18.20s           27,755 KiB/s
```
----
<b> 2023-2024 ADBeta </b>  
This software is under the GPL 2.0 Licence, please see LICENCE for information
