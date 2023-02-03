# psx-comBINe

!(https://github.com/ADBeta/psx-comBINe/blob/main/psx-comBINe.jpg)

psx-comBINe combines multiple .bin files into a single .bin file, according to 
a .cue sheet from a PSX CD-ROM Dump.

Some uses for this include:
* cue2pops or other methods to create VCD POPStarter files do not support multiple
bin files.
* Most emulators do not support multiple .bin files, and those that doe would still
perform better with a single .bin file.
* (from my own experience) Single .bin and .cue file pairs are burnt to a CD much
easier and with less errors, and perform better when being read by the hardware.

Combining /bin files is commonly done via CDMage, ISOBuster or other tools in the 
PSX community - But for those wanting more specific, lightweight tools this is the
target for psx-comBINe. 

psx-comBINe is currently designed for Linux systems, but I do intend to port it 
to Windows 32bit and 64bit executables in the near future. 

This project utilized TeFiEd heavily. Check it out here: [TeFiEd](https://github.com/ADBeta/TeFiEd)  

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
* 100MB RAM for file buffer ± 50KB RAM for the CUE Sheet data structure at max 
capacity (99 TRACKS, 99 INDEXS per TRACK).

## Compilation
#### Linux
- change directory into the downloaded parent directory
- run `make`
- an executable called 'psx-comBINe' should be in the bin directory

```
$ cd ./psx-comBINe-main
./psx-comBINe-main $ make

./psx-comBINe-main $ ./bin/psx-comBINe 
```

## Usage
psx-comBINe currently only supports 'direct' mode, which takes a multi-bin .cue file,
where it will create a sub-directory called psx-comBINe, where it will combine the
.bin and .cue data, and output them.  
`$ psx-comBINe /Games/Game.cue`  

## TO-DO
* help flag
* user input protection
* make sure program can handle .BIN .bin .CUE and .cue files.
* Parent directory handler may not play nice with windows. make a test for this
before full deploy.
* Create user input handler for program args


## Changelog (Reminder for self)
* 1.0.0 - Initial functional release
* 1.0.1 - Added (some) use instructions
* 1.0.2 - Help is a bit more helpful

## Licence
<b> 2023 ADBeta </b>  
This software is under the GPL (GPL3.0), please see LICENCE for information
