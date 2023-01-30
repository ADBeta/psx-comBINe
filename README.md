# psx-comBINe
<b> (c) ADBeta 2023 </b>  

psx-comBINe is a program to combine/merge multiple .bin files from a psx .cue  
.bin CD-ROM dump, and produces a single .bin file and .cue sheet.  
This is specifically useful for certain emulators, conversion softwares such as  
cue2pops, and I find it faster and less error prone when writing to cheap, low  
quality CD's.

This operation is commonly done via CDMage, or other tools in the PSX community,  
but having a purpose built tool seems like a useful addition to the community.  

This project is just a hobby, and is designed for Linux to begin with. I will  
eventually port it to a Windows exe, feel free to do this with the files if you  
want to test it out.  

This program depends boost::filesystem, make sure this is installed before use.  
If this causes problems with older systems, feel free to create your own fork,  
as this program shouldbe as universal as possible - fileHandler.cpp uses boost  
for directory creation and detection.  

This project utilized TeFiEd heavily. check it out here: [TeFiEd](https://github.com/ADBeta/TeFiEd)  

## Compilation
#### Linux
- change directory into the dowloaded parent directory
- run `make`
- an executable called 'psx-comBINe' should be in the bin directory

```
$ cd ./psx-comBINe-main
./psx-comBINe-main $ make
./psx-comBINe-main $ ./bin/psx-comBINe --help
```

## How To Use
Run psx-comBINe with the original cue file. for example  
`psx-comBINe ./game/game.cue`  

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

## Licence
This software is under the GPL (GPL3.0), please see LICENCE for information
