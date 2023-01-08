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

#include "fileHandler.h"
#include "common.h"

bool isLineValid(std::string line) {
	//Make sure the file extension is .bin
	if(line.find(".bin") == std::string::npos) {
		std::cout << "Error, file does not have .bin extension" << std::endl;
		return false;
	}
	
	//Make sure the file type is BINARY
	if(line.find("BINARY") == std::string::npos) {
		std::cout << "Error, file is not of type BINARY" << std::endl;
		return false;
	}
	
	//File is valid
	return true;
}

std::string getFilenameFromLine(std::string line) {
	//Find the first and last quote mark, and handle errors if none exist.
	size_t firstQuote, lastQuote;
	//First Quote
	firstQuote = line.find('"') + 1;
	
	if( firstQuote == std::string::npos) {
		//TODO error message
		std::cout << "Error, No initial \" found " << std::endl;
		exit(EXIT_FAILURE); //End execution.
	}
	
	//Last Quote
	lastQuote = line.find('"', firstQuote);
	
	if(lastQuote == std::string::npos) {
		std::cout << "Error, No Seconds \" found " << std::endl;
		exit(EXIT_FAILURE); //End execution.
	}
	
	//Create a new string from that substring - subst(firstQuite, length)
	std::string filename = line.substr(firstQuote, (lastQuote - firstQuote));
	
	return filename;
}
