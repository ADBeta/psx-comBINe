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

//Error message handler
//errLevel = 0 = warn, 1 = error (non fatal), 2 = error (fatal)
void errorMsg(unsigned int errLevel, std::string msg) {
	if(errLevel == 0) {
		std::cerr << "Warn: ";
	} else {
		std::cerr << "Error: ";
	}
	
	//Print the user message
	std::cerr << msg << std::endl;
	
	//If the errLevel is > 1 then exit the program as a fatal error
	if(errLevel > 1) exit(EXIT_FAILURE);
}

//Prompt user if they wish to continue then return their choice
bool promptContinue() {
	//Prompt
	std::cout << "Would you like to continue? y/N  ";
	
	//Clear buffer
	std::cin.clear();
	
	//Get input
	std::string userInput;
	std::getline(std::cin, userInput);
	
	//Return value
	if(userInput == "y" || userInput == "Y") {
		return true;
	} else {
		return false;
	}
}

bool isLineValid(std::string line) {
	//Make sure the file extension is .bin
	//TODO add continue option
	
	if(line.find(".bin") == std::string::npos) {
		errorMsg(0, "file does not have .bin extension");
		return false;
	}
	
	//Make sure the file type is BINARY
	if(line.find("BINARY") == std::string::npos) {
		errorMsg(0, "file is not of type BINARY");
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
	
	//Last Quote
	if(firstQuote != std::string::npos) lastQuote = line.find('"', firstQuote);
	
	if(firstQuote == std::string::npos || lastQuote == std::string::npos) {
		//Fatal error
		errorMsg(2, "A file path in the .cue file is corrupted - Missing quotes");	
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
		//Serious error
		errorMsg(1, "Can not open output .cue file");
		return 1;
	}
	
	//Set binFileOut filename to prefix.bin in directory
	binFileOut.open(binFilename.c_str(), std::ios::out | std::ios::binary);
	
	//If bin file doesn't exist, error
	if(!binFileOut) {
		//Serious error
		errorMsg(1, "Can not open output .bin file");
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
