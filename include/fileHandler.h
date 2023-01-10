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
#include<fstream>

#include "common.h"

#ifndef fileHandler_H
#define fileHandler_H

//Define how large the RAM byte array while dumping should be. (100MiB)
#define _def_ARR_SIZE 104857600

/** Variables *****************************************************************/
//Output filenames saved globally to enable printing of filenames at certain
//points in the program execution.
std::string cueFileOutFilename, binFileOutFilename;



/** Functions *****************************************************************/
//Checks a .cue file line to make sure it matches the model from research
//FILE "xxx.bin" BINARY 
//If no .bin and no BINARY, file is not valid
bool isLineValid(std::string line);

//Pulls the filename out of a valid .cue file line, converts it to char* and 
//pushes it to the vector array.
std::string getFilenameFromLine(std::string line);

//Pass a file suffix and a base directory, and use them to initilize and 
//set the output .bin and .cue files
//Return error status. (0 Success, 1 Failure)
int setupOutputFiles(std::string baseDirectory, std::string filePrefix);

//Opens all the files in the global binFilename vector and dumps them to 
//binFileOut, and logs the transistion byte of each file in the TODO
//Returns error status (0 success, 1 failure)
int dumpBinFiles();

/** Helper functions **********************************************************/
//Pads and returns a comma seperate string of bytes.
//NOTE: Pads string size not byte number
std::string padByteStr(size_t bytes, unsigned int pad = 0);

//Pads, sizes and returns a string of how many MiBs are in the value passed
std::string padMiBStr(size_t bytes, unsigned int pad = 0);

#endif
