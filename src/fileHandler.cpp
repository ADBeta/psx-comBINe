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
#include <boost/filesystem.hpp>

#include "fileHandler.h"
#include "TeFiEd.h"
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

int setupOutputFiles(std::string baseDirectory, std::string filePrefix) {
	//Always make sure baseDirectory has a / at the end for easier catenation.
	if(baseDirectory.back() != '/') baseDirectory.push_back('/');
	
	//TODO if any stray . or / get into the filenames, error
	
	//If the baseDirectory doesn't exist, make it
	if(boost::filesystem::is_directory(baseDirectory) == false) {
		boost::filesystem::create_directory(baseDirectory);
		//Print message to let user know directory has been created
		std::cout << "Created Directory: " << baseDirectory << std::endl;
	}
	
	//Create string for baseDirectory + prefix.
	std::string fileBase = baseDirectory + filePrefix;
	
	//Create string for both cue and bin outputs. Will be converted to c style
	std::string cueFilename = fileBase + ".cue";
	std::string binFilename = fileBase + ".bin";
	
	//Link TeFiEd object cueFileOut to prefix.cue in the directory
	cueFileOut = new TeFiEd(cueFilename.c_str());
	
	//If cueFileOut can't be created, error
	if(cueFileOut->create() != 0) {
		std::cout << "Cannot open cue file!" << std::endl;
		return 1;
	}
	
	//Set binFileOut filename to prefix.bin in directory
	binFileOut.open(binFilename.c_str(), std::ios::out | std::ios::binary);
	
	//If bin file doesn't exist, error
	if(!binFileOut) {
		//TODO error handling
		std::cout << "Cannot open bin file!" << std::endl;
		return 1;
	}
	
	//Return success
	return 0;
}

int dumpBinFiles(const char* outputFilename) {
	//Create the new output file.


	//Return 0 for success
	return 0;
}
