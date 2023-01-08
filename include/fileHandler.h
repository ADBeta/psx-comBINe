/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing, this is ideal for PSX/PS1 CD-ROMs
* e.g. Rayman to get them ready for cue2pops or some emulators. I also find it 
* improves reliabilty when buring to a disk to only have one .bin file.
*
* (c) ADBeta
*******************************************************************************/
#include <iostream>

#ifndef fileHandler_H
#define fileHandler_H

//Checks a .cue file line to make sure it follows the rules my program knows
//FILE "xxx.bin" BINARY 
//If no .bin and no BINARY, file is not valid
bool isLineValid(std::string line);

//Pulls the filename out of a valid .cue file line, converts it to char* and 
//pushes it to the vector array.
std::string getFilenameFromLine(std::string line);

#endif
