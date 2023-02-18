/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing, this is ideal for PSX/PS1 CD-ROMs
* e.g. Rayman to get them ready for cue2pops or some emulators. I also find it 
* improves reliabilty when buring to a disk to only have one .bin file.
*
* Cue File manager. Uses TeFiEd to handle output file cue file track, file and 
* index text
* 
* this might be turned into its own project if I see it useful in future.
*
* (c) ADBeta
*******************************************************************************/
#include <string>

#ifndef HELPERS_H
#define HELPERS_H


/*** Error handling ***********************************************************/
//Print error message with different error levels:
//0 warn	1 error		2 fatal
//funct is error function. defaults to "Undefined"
//Pass error string, or error message enum index.
void errorMsg(unsigned int errLevel, std::string funct, std::string errStr);
void errorMsg(unsigned int errLevel, std::string funct, int errEnum);

//Prompt user if they wish to continue and return status
bool promptContinue();

//Pads and returns a comma seperate string of bytes.
//NOTE: Pads string size not byte number
std::string padByteStr(size_t bytes, unsigned int pad = 0);

//Pads, sizes and returns a string of how many MiBs are in the value passed
std::string padMiBStr(size_t bytes, unsigned int pad = 0);

#endif
