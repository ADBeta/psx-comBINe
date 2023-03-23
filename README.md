# psx-comBINe

psx-comBINe combines a multi-bin CD-ROM PSX Game into a single .CUE and .BIN pair
and supports all CUE MODES, Remarks, 99 TRACKS, with 99 INDEXS each, support for
multi-session dumps.

32bit and 64bit exes are in the release files.

![logo](https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

I have made a [YouTube video](https://www.youtube.com/watch?v=gBMUwBJaiMM) about psx-comBINe

psx-comBINe is unlike other applications because it has excellent compatibility 
with the CUE sheet standards. It can support any of the GNU approved CUE TYPEs, 
as well as REM lines, a max of 99 TRACKs and INDEXs, and repairs slightly 
malformed inputs.

It is written in C++ to be as fast as possible on all systems, and is designed to
be able to run on a 32bit systems for those who need to use older machines.  
Speed tests can be seen below.

Some uses for this include:
* cue2pops or other methods to create VCD POPStarter files do not support multiple
bin files.
* Most emulators do not support multi-bin dumps
* In my experience, CDs burnt using single .bin .cue pairs suffer far fewer burn
errors, read errors or hangs. I believe this to be due to IO File Swaps during
burning causing issues in some cheaper CDs and/or software bugs.

This program uses [TeFiEd](https://github.com/ADBeta/TeFiEd) and [CLIah](https://github.com/ADBeta/CLIah)  

<b> NOTE: This program is in no way intended to support or condone piracy. This program
should only be used with legitimately acquired backups of disks you own LEGALLY </b>  

----
## Dependencies
To compile This program requires the `boost-filesystem` library. (boost)

#### Arch
`sudo pacman -Syu boost`  
#### Ubuntu / Debian
`sudo apt-get install libboost-all-dev`  

## Requirments
The minimum requirments for this program are:  
* C++11 Compiler
* 32 or 64 bit Architecture
* 100MB RAM for binay file buffer + 50KB RAM for the CUE Sheet data structure at 
max capacity (99 TRACKS, 99 INDEXS per TRACK).

## Compilation
#### Linux
The easiest way to install psx-comBINe is to use git clone:
```
git clone https://github.com/ADBeta/psx-comBINe
cd ./psx-comBINe
sudo make install
```

## Usage
The default way to use psx-comBINe is like this  
`$ ./psx-comBINe /Games/Game.cue`

On Windows it is very much the same use, but use Windows Backslashes "\\"  
`.\psx-comBINe.exe .\Games\Game.cue`

For more uses and information use the inbuilt help  
`psx-comBINe --help`

----
## Speed tests
```
Rage Racer   750,713 KB - 17 Tracks
Ridge Racer  467,180 KB - 14 Tracks
Rayman       701,881 KB - 51 Tracks

PSX Game      Average time     Data Speed

Windows 10:
-----------------------------------------
Rage Racer   22.37 Seconds    33,558 KiB/s
Ridge Racer  13.91 Seconds    33,585 KiB/s
Rayman       21.29 Seconds    32,967 KiB/s
                   Average     32.59 MiB/s

Arch Linux:
-----------------------------------------
Rage Racer   09.77 Seconds    76,838 KiB/s
Ridge Racer  05.34 Seconds    87,486 KiB/s
Rayman       08.67 Seconds    80,955 KiB/s
                   Average     79.84 MiB/s

Fedora Linux 18 (2012)
32bit, 1.6GHz, Single core Atom CPU:
-----------------------------------------
Rage Racer   67.35 Seconds    11,195 KiB/s
Ridge Racer  41.73 Seconds    11,154 KiB/s
Rayman       62.50 Seconds    11,230 KiB/s
                   Average     10.93 MiB/s

Windows 7
32bit, 1.6GHz, Single core Atom CPU:
-----------------------------------------
Rage Racer   178.2 Seconds     4,212 KiB/s
Ridge Racer  110.1 Seconds     4,243 KiB/s
Rayman       172.3 Seconds     4,073 KiB/s
                   Average      4.17 MiB/s
```
From my tests, psx-comBINe ran around 2.5x faster on Linux, however Windows still
performed very well.  
The performance on older hardware, while not impressive, is very usable, and provides
support for those people stuck with old hardware.

----
## Changelog (Reminder for self)
* 1.0.0 - Initial functional release
* 1.1.0 - CLIah is integrated
* 1.2.0 - Refined execution, added help message
* 1.4.0 - Support REM lines, changed error handlng
* 1.5.0 - Support for .CUE and .cue, neater main exec
* 1.7.0 - Upgraded TeFiEd and CLIah. Improved support and speed. Failsafes enabled
* 2.0.0 - All CLI modes supported. Pre-compiled 32bit and 64bit exes provided. 
Super low-spec machine compile and speed test (1 core 
Atom CPU, 1GB RAM). Improved performance generally.
* 2.1.1 - Fully static exes and minor bugfixes.

## Licence
<b> 2023 ADBeta </b>  
This software is under the GPL 3.0 Licence, please see LICENCE for information
