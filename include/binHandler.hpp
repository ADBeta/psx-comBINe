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
#include <fstream>
#include <vector>

#ifndef BINHANDLER_H
#define BINHANDLER_H

//Define how large the RAM byte array while dumping should be. (100MiB)
#define _def_ARR_SIZE 104857600

/** Variables *****************************************************************/
//Each entity is at which byte a file transistion to the next file. This is for
//.cue output INDEX value.
extern std::vector<size_t> binOffsetBytes;

//Pass vector to bin filenames. Opens each file one at a time and dumps them
//into outFn as a binary file.
//Returns error status (0 success, 1 failure)
int dumpBinFiles(std::vector<std::string> &binVect, const std::string outFn);

#endif
