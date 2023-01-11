# psx-comBINe
<b> (c) ADBeta 2023 </b>  

psx-comBINe is a program to combine/merge multiple .bin files from a psx .cue  
.bin CD-ROM dump, into a single .cue and .bin file.  

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
./psx-comBINe $ make
./psx-comBINe $ ./bin/psx-comBINe --help
```

## How To Use

## TO-DO
* help flag
* user input protection
* make sure program can handle .BIN .bin .CUE and .cue files.
* Parent directory handler may not play nice with windows. make a test for this  
before full deploy.  
* Create user input handler for program args
* optimize dump more?
* add cleanup function to close files
* add try catch return error handling for dump function

## Changelog (Reminder for self)
* 0.3.1 - .cue file lines get detected and stripped of file info, with error  
checking  
stored in the binFile vector ready for append and dump.
* 0.4.3 - Moved some variables and types to common.h
* 0.4.5 - Included boost in make and src files. 
* 0.5.5 - Implimented filesystem managment and file creation
* 0.6.5 - Added error message system
* 0.7.5 - Added continue prompting function.
* 0.8.7 - Completed binary dump function (medium optimized)
* 0.8.8 - Included byte and MiB printing functions
* 0.9.8 - Complete refactor of most functions
* 0.10.8 - Testing phase for cue INDEX beings
* 0.11.8 - Have .cue timestamp string creation working.

## Licence
This software is under the GPL (GPL3.0), please see LICENCE for information  
