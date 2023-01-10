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

#include "common.h"

#ifndef fileHandler_H
#define fileHandler_H

//Define how large the RAM byte array while dumping should be. (100MiB)
#define _def_ARR_SIZE 104857600

/** Variables *****************************************************************/

/** Functions *****************************************************************/
//Pass input filename string, returns the file name without the extension 
//NOTE: The *FIRST* . is considered the start of the file extension.
//e.g. "/directory/file.tar.xz" returns "file"
std::string getFileName(const std::string inputFile);

//Checks a .cue file line to make sure it matches the model from research
//FILE "xxx.bin" BINARY 
//If no .bin and no BINARY, file is not valid
bool lineIsValid(const std::string line);

//Pulls the filename string out of a valid .cue FILE line and returns it
std::string getFileFromCueLine(const std::string line);

//Pass the base output directory string, cueFileOut and binFileOut filename. 
//Creates directory and opens the files and checks they are valid files / exist
//Return error status. (0 Success, 1 Failure)
int openOutputFiles(const std::string baseDir, const std::string cueFn, 
                    const std::string binFn);

//Pass vector to bin filenames. Opens each file one at a time and dumps them
//into the binOutFile. Pass output filename for printing to cli
//Returns error status (0 success, 1 failure)
int dumpBinFiles(std::vector<std::string> &binVect, const std::string outFn);

/** Helper functions **********************************************************/
//Print error message with different error levels:
//0 warn	1 error		2 fatal
void errorMsg(unsigned int errLevel, std::string msg);

//Prompt user if they wish to continue and return status
bool promptContinue();

//Pads and returns a comma seperate string of bytes.
//NOTE: Pads string size not byte number
std::string padByteStr(size_t bytes, unsigned int pad = 0);

//Pads, sizes and returns a string of how many MiBs are in the value passed
std::string padMiBStr(size_t bytes, unsigned int pad = 0);

#endif
