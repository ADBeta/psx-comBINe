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

## TO-DO
* help flag
* user input protection
* make sure program can handle .BIN .bin .CUE and .cue files.
* Parent directory handler may not play nice with windows. make a test for this  
before full deploy.
* Create user input handler for program args
* Make cueRead function work
* Ensure printFILE detects a request for non-existent vector ID. causes segfault
* Convert code to use TeFiEd getWord() functionality

## Changelog (Reminder for self)
* 0.3.1 - .cue file lines get detected and stripped of file info, with error  
checking
* 0.4.5 - Included boost in make and src files. 
* 0.5.5 - Implimented filesystem managment and file creation
* 0.11.9 - Sanity check for output buffer array creation
* 0.12.10 - Changed fileHandler to binHander, and added a library solely for  
cue handling
* 0.12.12 - Moved functions from common.h to helpers.h, and removed extern  
dependance
* 0.17.14 - Started rework of CueHandler
* 0.18.14 - Testing struct vector managment
* 0.18.15 - Cue data struct printing finished. Debug assist
* 0.20.15 - Cue File Track and Index push functions finished.
* 0.21.15 - Have getCueData and printFILE setup to a basic extent.

## Licence
This software is under the GPL (GPL3.0), please see LICENCE for information
