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
#include <string>
#include <vector>

#include "TeFiEd.h"

#ifndef CUE_HANDLER_H
#define CUE_HANDLER_H

class CueHandler {
	public:
	//Constructor takes a filename (char *) and passes it to the TeFiEd cueFile
	CueHandler(const char* filename);
	
	/** File Management *******************************************************/
	//Read in an existing cue file
	void read();
	
	//Create a new cue file
	void create();
	
	/**** Reading Functions ****/	
	//Checks a .cue file line to make sure it matches the model from research
	//FILE "xxx.bin" BINARY. If no .bin and no BINARY, file is not valid 
	bool lineIsValid(const size_t lineNo);
	
	//Pulls the filename string out of a valid .cue FILE command and returns it
	std::string getFILE(const std::string line);

	//Finds all lines with FILE in them, then pushed the filename on that line,
	//appended with the parent directory to a vector, (for dumping bin files)
	void pushFILEToVector(std::vector<std::string> &vect);

	/**** Writing Functions ****/
	//Output (overwrite) the TeFiEd object RAM into the file.
	void write();
	
	void newFILE(std::string fileName, std::string type);
	
	void newTRACK(std::string type);
	
	void newINDEX(std::string indexStr);

	
	/** AUX Functions *********************************************************/
	//Converts a number of bytes into an Audio CD timestamp.
	std::string bytesToTimestamp(const unsigned long bytes);
	
	//Converts an Audio CD timestamp into number of bytes
	unsigned long timestampToBytes(std::string timestamp);
	

	//private:
	/** Variables **/
	TeFiEd *cueFile; //TeFiEd text file object
	
	unsigned int cValTRACK = 1; //Current TRACK overall (start from 1)
	unsigned int cValINDEX = 0; //Current INDEX intager resets on new FILE
	std::string  cStrINDEX; //Current INDEX string. 
	
	/** Functions **/
	//Takes an input uint32_t, zero-pads to -pad- then return a string
	std::string padIntStr(const unsigned long val, unsigned int pad = 0);

}; //class CueHandler
#endif
