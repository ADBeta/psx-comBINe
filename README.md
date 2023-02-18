# psx-comBINe

![logo](https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

psx-comBINe combines a multi-bin CD-ROM PSX Game into a single .CUE and .BIN pair.  
(supports all CUE MODES, 99 TRACKS, with 99 INDEXS each, and can handle multi-session
dumps)

An experimental .exe is in the release files.  

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

psx-comBINe is currently designed for Linux systems, but I do intend to port it 
to Windows 32bit and 64bit executables in the near future. 

This program uses TeFiEd heavily. Check it out here: [TeFiEd](https://github.com/ADBeta/TeFiEd)  
and CLIah for the Command line handling, which is here: [CLIah](https://github.com/ADBeta/CLIah)  

<b> NOTE: This program is in no way intended to support or condone piracy. This program
should only be used with legitimately acquired backups of disks you own LEGALLY </b>

## Dependencies
This program requires the `boost-filesystem` libraries, but I recommend installing
all available `boost` libraries.  

#### Arch 
`sudo pacman -Syu boost`  
#### Ubuntu / Debian
`sudo apt-get install libboost-all-dev`  

## Requirments
The only external dependency is boost, which, along with my own software supports
32bit, Any platform -should- be able to be compiled to, if not I would be interested
in porting or allowing support for those platforms.

The minimum requirments for this program are:  
* C++11 Compiler
* 32 bit Architecture
* 100MB RAM for file buffer + 50KB RAM for the CUE Sheet data structure at max 
capacity (99 TRACKS, 99 INDEXS per TRACK).

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

On Windows it is very much the same use, but use Windows Backslashes "\"  
`.\psx-comBINe.exe .\Games\Game.cue`  


## TO-DO
* make sure program can handle .BIN .bin .CUE and .cue files.
* Make internal WINDOWS detect and auto-port
* REM support and output
* Hard testing and error handling
* Speed testing
* add arg options and write about them in help message


## Changelog (Reminder for self)
* 1.0.0 - Initial functional release
* 1.1.0 - CLIah is integrated
* 1.2.0 - Refined execution, added help message

## Licence
<b> 2023 ADBeta </b>  
This software is under the GPL (GPL3.0), please see LICENCE for information
