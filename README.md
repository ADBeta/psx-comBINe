# psx-comBINe
## Version 3 is here!
**What's new?**  
* Migrated from Boost C++ Libraries to C++17 for directory handling
* Changed the executable name from `psx-comBINe` to `psx-combine` to improve 
ease of use, and formality within the UNIX families (all lowercase)
* Changed the size of the RAM buffer, to increase cache usage. Increases speed
* Added the ability to time the programs execution, via command arguments
(See --help)
* Improved help messages
* Added utility for creating a `.cue` file from a `.bin` files, or a directory.
this is useful to repair corrupted dumps - **PLEASE NOTE** This feature only 
works for specific situations. See [Usage.md](https://github.com/ADBeta/psx-comBINe/blob/main/Usage.md) for more info
* Improved speed and stability of the program - Around 30% less processing 
overhead
* Changed the sourcecode to use more modern and type-safe declaration types

**What's next?**
* I plan to branch off this program to have a GUI, so it's more user friendly to
those without knowledge of the command line
* I will be increasing stability, speed and including more features as time goes
on, however:
* Feature creep is a concern with projects like these, as such, some requests 
and features will be ported into their own programs as time goes on.
* I will try to make the next version multi-threaded
* Adding directory support for combining input files, to make using psx-combine
even easier

**32bit and 64bit exes for Windows Systems are in the release files.**

![logo](https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

I have made a [YouTube video](https://www.youtube.com/watch?v=gBMUwBJaiMM) about
psx-comBINe  
(**NOTE** This video is outdated for build instructions, and doesn't cover all
of the available functions)

## Decription
**psx-comBINe** combines a multi-bin CD-ROM PSX Game into a single .CUE and .BIN
pair and supports all CUE MODES, Remarks, 99 TRACKS, with 99 INDEXS each, 
support for multi-session dumps, and more.  

psx-comBINe is designed to be lightweight and fast; one of the main design goals
with this CLI version is to allow it to be ported into other projects - such as 
emulators so that multi-bin files can be played witout having to first combine 
the files, but instead letting psx-comBINe handle that, output files to a temp
folder, and allow the emulator to play them from there.

The goal of this is to make the PSX scene even easier to get into, without
having to know how or why you need to combine .bin .cue files.  

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
This will copy the the psx-combine executable to your `/usr/local/bin./` folder.  
If you do not want this to happen you can use  
```sh
make
```
instead, this will output the psx-combine executable to the the `/psx-comBINe/bin`
directory  

### Windows
I will not go into detail about this, as it is highly preferential, however:  
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
`psx-comBINe --help`

----
## Speed tests
```
Test Game    Size         Tracks
-----------------------------------
Rayman       701,881 KB   51 Tracks
Rage Racer   750,713 KB   17 Tracks
WipEout      505,151 KB   9 Tracks

PSX Game      Average time     Data Speed

Windows 10
64bit, Intel i7-4790:
-------------------------------------------
Rayman        11.46            61,246 KiB/s
Rage Racer    09.43            79,609 KiB/s
WipEout       05.91            85,473 KiB/s
                      Average: 75.44 MiB/s (2.31x faster than Version 2.x.x)

Arch Linux
64bit, Intel i7-4790:
-------------------------------------------
Rayman        05.57           126,010 KiB/s
Rage Racer    05.95           126,170 KiB/s
WipEout       04.00           126,287 KiB/s
                     Average: 126.15 MiB/s (1.58x faster than version 2.x.x)

Debian 8 (jessie)
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        45.33            15,483 KiB/s
Rage Racer    47.20            15,904 KiB/s
WipEout       31.55            16,011 KiB/s
                      Average: 15.79 MiB/s (1.44x faster than Version 2.x.x)

Windows 7
32bit, 1.6GHz, Single core Atom CPU:
-------------------------------------------
Rayman        58.54            11,989 KiB/s
Rage Racer    59.91            12,530 KiB/s
WipEout       40.97            12,329 KiB/s
                      Average: 12.28 MiB/s (2.94x faster than Version 2.x.x)
```

----
<b> 2023 ADBeta </b>  
This software is under the GPL 2.0 Licence, please see LICENCE for information
