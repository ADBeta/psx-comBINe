# psx-comBINe
<b> (c) ADBeta 2023 </b>  

psx-comBINe is a program to combine/merge multiple .bin files from a psx .cue  
.bin CD-ROM dump, into a single .cue and .bin file.  

This function is commonly done via CDMage, or other tools in the PSX community,  
but having a purpose built tools seems like a useful addition to the community  
I love so much.  

This project is just a hobby, and is designed for Linux to begin with. I will  
eventually port it to a Windows exe, feel free to do this with the files if you  
want to test it out.  

## How To Use

## TO-DO
* cue decode to list of files
* binary file append handler
* help flag
* user input protection
* if a .cue FILE is not recognised, pause and ask for confirm rather than exit.
* ^^ Add a 'continue y/N' prompt function to call any time
* make a verbose error handler

## Changelog
* 0.0.1 - Initial base design.  
* 0.1.1 - Finished TeFiEd integration
* 0.2.1 - Added .cue file line detection
* 0.3.1 - .cue file lines get detected and stripped of file info, with error  
checking  
* 0.3.2 - Swap char* vect to string, and convert in file open routine

## Licence
This software is under the GPL (GPL3.0), please see LICENCE for information  
