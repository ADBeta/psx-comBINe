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
	
	//If the fileprefix has / in it, tell user
	if(filePrefix.find('/') != std::string::npos) {
		errorMsg(1, "Output filePrefix contains / This should not be a directory");
	}
	
	//If the filePrefix contains .
	if(filePrefix.find('.') != std::string::npos) {
		errorMsg(1, "Output filePrefix contains . : Suffix should not be provided");
	}
	
	//If the baseDirectory doesn't exist, make it
	if(boost::filesystem::is_directory(baseDirectory) == false) {
		boost::filesystem::create_directory(baseDirectory);
		//Print message to let user know directory has been created
		std::cout << "Created Directory: " << baseDirectory << std::endl;
	}

	//Create string for both cue and bin outputs. Will be converted to c style
	std::string cueFn = baseDirectory + filePrefix + ".cue";
	std::string binFn = baseDirectory + filePrefix + ".bin";
	
	//Link TeFiEd object cueFileOut to prefix.cue in the directory
	cueFileOut = new TeFiEd(cueFn.c_str());
	
	//If cueFileOut can't be created, error
	if(cueFileOut->create() != 0) {
		//Serious error
		errorMsg(1, "Can not open output .cue file");
		return 1;
	}
	
	//Set binFileOut filename to prefix.bin in directory
	binFileOut.open(binFn.c_str(), std::ios::out | std::ios::binary);
	
	//If bin file doesn't exist, error
	if(!binFileOut) {
		//Serious error
		errorMsg(1, "Can not open output .bin file");
		return 1;
	}
	
	//Return success
	return 0;
}

int dumpBinFiles() {
	//Current byte in array and overall bytes read followers
	size_t arrByte = 0, readByte = 0;
	
	//Create a heap byte array (100MiB)
	#define ARR_SIZE 104857600
	char *byteArray = new char[ARR_SIZE];

	//Go through all filenames in the .cue file
	for(size_t indx = 0; indx < binFilename.size(); indx++) {
		//Print message about file
		std::cout << "Dumping " << binFilename.at(indx);
		
		//Open the current binFilename to binFileIn and reset positions
		binFileIn.open(binFilename[indx].c_str(), std::ios::in | std::ios::binary);
		binFileIn.seekg(0, std::ios::beg);
		
		//Byte pulled fron binary file
		char cByte;
		while(binFileIn.get(cByte)) {
			//Put the read byte into the array
			byteArray[arrByte] = cByte;
			++arrByte;
			
			//If the array is full, dump it to the output file, and reset.
			if(arrByte == ARR_SIZE) {
				binFileOut.write(byteArray, arrByte);
				arrByte = 0;
			}
			
			//Keep track of how many bytes read so far
			++readByte;
		}
		
		//TODO Log the end byte of the current byte to convert to INDEX in .cue
		
		//Close the current file for next loop
		binFileIn.close();
		
		//Report how many bytes each file is, and that it is done.
		std::cout << "\t" << arrByte << "\tBytes\t"<< "\tDone" << std::endl;
	}

	std::cout << "read " << readByte << " bytes" << std::endl;

	//Flush what is left of the byte array to the output file
	//Write the entire array out to the binFileOut file
	if(arrByte > 0) binFileOut.write(byteArray, arrByte);

	//Delete heap byte array
	delete[] byteArray;
	
	//Return 0 for success
	return 0;
}
