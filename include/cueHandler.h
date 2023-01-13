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
#include <iostream>

#include "TeFiEd.h"

#ifndef CUE_HANDLER_H
#define CUE_HANDLER_H

class CueHandler {
	public:
	//Constructor takes a filename (char *) and passes it to the TeFiEd cueFile
	CueHandler(const char* filename);

	private:
	TeFiEd *cueFile;

}; //class CueHandler


//

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



//Takes a byte value and returns a MM:SS:FF timestamp string. This is used for 
//.cue file INDEX points. Uses CD Audio cue sheet methodology.
std::string getTimestamp(size_t bytes);


#endif
