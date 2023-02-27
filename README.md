# psx-comBINe

psx-comBINe combines a multi-bin CD-ROM PSX Game into a single .CUE and .BIN pair.  
(supports all CUE MODES, 99 TRACKS, with 99 INDEXS each, and can handle multi-session
dumps)

An up-to-date release exe is in the release files.

![logo](https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

Some uses for this include:
* cue2pops or other methods to create VCD POPStarter files do not support multiple
bin files.
* Most emulators do not support multi-bin dumps, and those that do would still
perform better with a single .bin file.
* (from my own experience) Single .bin and .cue file pairs are burnt to a CD much
easier and with less errors, and perform better when being read by the hardware.

Combining .bin files is commonly done via CDMage, ISOBuster or other tools in the 
PSX community - psx-comBINe aims to be more lightweight, faster, and easier to
combine into other projects as it uses only std and boost, and aims for a low overhead  

This program uses TeFiEd heavily. Check it out here: [TeFiEd](https://github.com/ADBeta/TeFiEd)  
and CLIah for the Command line handling, which is here: [CLIah](https://github.com/ADBeta/CLIah)  

<b> NOTE: This program is in no way intended to support or condone piracy. This program
should only be used with legitimately acquired backups of disks you own LEGALLY </b>  

----
## Dependencies
To compile This program requires the `boost-filesystem` libraries, but I recommend 
installing all available `boost` libraries.  

#### Arch
`sudo pacman -Syu boost`  
#### Ubuntu / Debian
`sudo apt-get install libboost-all-dev`  

## Requirments
The minimum requirments for this program are:  
* C++11 Compiler
* 32 bit Architecture
* 100MB RAM for binay file buffer + 50KB RAM for the CUE Sheet data structure at 
max capacity (99 TRACKS, 99 INDEXS per TRACK).

## Compilation
#### Linux
- change directory into the downloaded parent directory
- run `make`
- an executable called 'psx-comBINe' should be in the bin directory

```
$ cd ./psx-comBINe-main
./psx-comBINe-main $ make

./psx-comBINe-main $ ./bin/psx-comBINe -h
```

## Usage
psx-comBINe currently only supports 'direct' mode, which takes a multi-bin .cue file,
where it will create a sub-directory called psx-comBINe, where it will combine the
.bin and .cue data, and output them.  
`$ ./psx-comBINe /Games/Game.cue`  

On Windows it is very much the same use, but use Windows Backslashes "\\"  
`.\psx-comBINe.exe .\Games\Game.cue`  

----
## Speed tests
Tests performed on Arch Linux and Windows 10 Respectively, with a sample size of
10 multibin dumps from Enter Key press, to program exit using psx-comBINe 1.7.0  
```
Rage Racer   750,713 KB - 17 Tracks
Ridge Racer  467,180 KB - 14 Tracks
Rayman       701,881 KB - 51 Tracks

Platform     Average time     Data Speed

Windows:
-----------------------------------------
Rage Racer   22.37 Seconds    33,558 KB/s
Ridge Racer  13.91 Seconds    33,585 KB/s
Rayman       21.29 Seconds    32,967 KB/s

Linux:
-----------------------------------------
Rage Racer   11.21 Seconds    66,968 KB/s
Ridge Racer  7.05 Seconds     66,266 KB/s
Rayman       11.24 Seconds    62,444 KB/s
```

From my tests, psx-comBINe ran around 2x faster on Linux, however Windows still
performed very well compared to alternative programs.  

----
## TO-DO
* Make internal WINDOWS detect and auto-port
* Speed testing
* youtube video "how to combine multi-track bin files for ps1/psx games using psx-comBINe"

## Changelog (Reminder for self)
* 1.0.0 - Initial functional release
* 1.1.0 - CLIah is integrated
* 1.2.0 - Refined execution, added help message
* 1.4.0 - Support REM lines, changed error handlng
* 1.5.0 - Support for .CUE and .cue, neater main exec
* 1.7.0 - Upgraded TeFiEd and CLIah. Improved support and speed. Failsafes enabled

## Licence
<b> 2023 ADBeta </b>  
This software is under the GPL (GPL3.0), please see LICENCE for information
