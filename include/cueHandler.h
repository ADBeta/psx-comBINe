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
	
	//Adds a FILE command entry to the cueFile. type can be BINARY or AUDIO etc
	void newFILE(const std::string fileName, const std::string type);
	
	//Adds a TRACK, Numbered as num, to the cueFile. Type should be AUDIO etc
	void newTRACK(const unsigned int num, const std::string type);
	
	//Adds a new INDEX, numbered as num, and the timestamp to the cueFile
	void newINDEX(const unsigned int num, const std::string indexStr);

	
	/** AUX Functions *********************************************************/
	//Converts a number of bytes into an Audio CD timestamp.
	std::string bytesToTimestamp(const unsigned long bytes);
	
	//Converts an Audio CD timestamp into number of bytes
	unsigned long timestampToBytes(std::string timestamp);
	
	//Adds two INDEX strings together and returns a timestamp string.
	std::string addTimestamps(const std::string ts1, const std::string ts2);
	

	//private:
	/** Variables **/
	TeFiEd *cueFile; //TeFiEd text file object
	
	/** Functions **/
	//Takes an input uint32_t, zero-pads to -pad- then return a string
	std::string padIntStr(const unsigned long val, unsigned int pad = 0);

}; //class CueHandler
#endif
