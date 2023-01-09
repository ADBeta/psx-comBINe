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
*
* This file contains extern variables and declarations of types used between 
* program modules.
*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include<fstream>

#include "TeFiEd.h"

#ifndef COMMON_H
#define COMMON_H

//Error message handling function. Declared at very top of fileHandler.cpp
//but want all modules to have access to these functions
//errLevel = 0 = warn, 1 = error (non fatal), 2 = error (fatal)
extern void errorMsg(unsigned int errLevel, std::string msg);

//Prompt user if they wish to continue then return a bool value based on their
//choice
extern bool promptContinue();

//Input and output .cue file TeFiEd pointers. Both get set and used later. 
extern TeFiEd *cueFileIn, *cueFileOut;

//Input and output binary files. Used by fileHandler but keeping things global 
//helps clarity, and makes sure they are absolutely in RAM when program is execd
extern std::ofstream binFileOut, binFileIn;

//Vector of filenames pulled from the cueFile.
extern std::vector<std::string> binFile;

#endif
