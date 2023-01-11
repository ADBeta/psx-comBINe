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
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>

#include "fileHandler.h"
#include "TeFiEd.h"
#include "common.h"

//Each entity is at which byte a file transistion to the next file. This is for
//.cue output INDEX value (in time format which gets converted later)
std::vector<size_t> fileIndexByte;

std::string getFileName(const std::string inFn) {	
	//TODO Windows may not work with this methodology	
	std::string fileName;
	//Create fileName from substring of inputFIle - Remove directory informaton
	fileName = inFn.substr(inFn.find_last_of('/') + 1, inFn.length());
	
	//Remove the first . and anything past it in fileName
	fileName = fileName.substr(0, fileName.find('.'));
	
	return fileName;	
}

bool lineIsValid(const std::string line) {
	//Make sure the file extension is .bin	
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

std::string getFileFromCueLine(const std::string line) {
	//Find the first and last quote mark, and handle errors if none exist.
	size_t firstQuote, lastQuote;
	//First Quote
	firstQuote = line.find('"') + 1;
	
	//Last Quote
	if(firstQuote != std::string::npos) lastQuote = line.find('"', firstQuote);
	
	if(firstQuote == std::string::npos || lastQuote == std::string::npos) {
		//Fatal error
		errorMsg(2, "A filename in .cue file is corrupted (Missing quotes)");	
	}
	
	//Create a new string from that substring - subst(firstQuite, length)
	std::string filename = line.substr(firstQuote, (lastQuote - firstQuote));
	return filename;
}

int openOutputFiles(const std::string baseDir, const std::string cueFn, 
                    const std::string binFn) {
                    
	//If the base output directory doesn't exist already, create it
	if(boost::filesystem::is_directory(baseDir) == false) {
		//Watch for errors creating output directory
		if( boost::filesystem::create_directory(baseDir) == false) {
			errorMsg(1, "Can not create output director. Check privilege");
			return 1;
		}
		
		//If success print message to let user know directory has been created
		std::cout << "Created Directory: " << baseDir << std::endl;
	}

	//Create a TeFiEd object to cueFn
	cueFileOut = new TeFiEd(cueFn.c_str());
	
	//If cueFileOut can't be created, error
	if(cueFileOut->create() != 0) {
		//Serious error
		errorMsg(1, "Can not create output .cue file");
		return 1;
	}
	
	//Create the output binary file to binFn
	binFileOut.open(binFn.c_str(), std::ios::out | std::ios::binary);
	
	//If bin file doesn't exist, error
	if(!binFileOut) {
		//Serious error
		errorMsg(1, "Can not create output .bin file");
		return 1;
	}
	
	//Return success
	return 0;
}

int dumpBinFiles(std::vector<std::string> &binVect, const std::string outFn) {
	//Current byte in array and overall bytes read followers
	size_t arrBytes = 0, readBytes = 0;
	
	//Create a heap byte array (From defined size in header)
	char *byteArray = new char[_def_ARR_SIZE];
	
	//TODO array creation check

	//Go through all filenames that were in the .cue file
	for(size_t indx = 0; indx < binVect.size(); indx++) {
		//Print message about file
		std::cout << "Dumping: " << binVect.at(indx) << "    ";
		
		//Open the current binFilename to binFileIn and reset positions
		binFileIn.open(binVect[indx].c_str(), std::ios::in | std::ios::binary);
		binFileIn.seekg(0, std::ios::beg);
		
		//Byte pulled fron binary file
		char cByte;
		while(binFileIn.get(cByte)) {
			//Put the read byte into the array
			byteArray[arrBytes] = cByte;
			++arrBytes;
			
			//If the array is full, dump it to the output file, and reset.
			if(arrBytes == _def_ARR_SIZE) {
				binFileOut.write(byteArray, arrBytes);
				arrBytes = 0;
			}
			
			//Keep track of how many bytes read so far
			++readBytes;
		}
		
		//Log the transistion byte of the current file for INDEX in .cue
		fileIndexByte.push_back(readBytes);
		
		//Close the current file for next loop
		binFileIn.close();
		
		//Report how many bytes each file is, and that it is done.
		std::cout << padMiBStr(arrBytes, 3) << "    Done" << std::endl;
	}
	
	//Flush what is left of the byte array to the output file
	if(arrBytes > 0) binFileOut.write(byteArray, arrBytes);

	//Delete heap byte array
	delete[] byteArray;
	
	//Report completion and bytes written to the output bin file
	std::cout << std::endl << "Successfully dumped " << padByteStr(readBytes) 
	          << " to " << outFn << std::endl;

	//Return 0 for success
	return 0;
}

std::string getTimestamp(size_t bytes) {
	/* The timestamp is in Minute:Second:Frame format.
	There are 75 sectors per second, 2352 bytes per sector. If the input number 
	is not divisible by 1 sector, then exit the program.
	
	I am trying to use divide then modulo so the compiler stands some chance of
	optimizing my code to use the remainder of the ASM div operator. */
	
	std::string timestamp;	
	
	//There are 2352 byte per sector. Calculate how many sectors are in the file
	size_t sectors = bytes / 2352;
	
	//Error check if the input is divisible by a sector. Exit if not
	if(bytes % 2352 != 0) {
		errorMsg(2, "A bin file is not divisible by SECTOR size (Corrupt bin file)");
	}
	
	//75 sectors per second. Frames are the left over sectors from a second
	int seconds = sectors / 75;
	int rFrames = sectors % 75;
	
	//Convert seconds to minutes. Seconds is the remainder of itself after / 60
	int minutes = seconds / 60;
	seconds = seconds % 60;
	
	//If minutes exceeds 99, there is probably an error due to Audio CD Standard
	if(minutes > 99) errorMsg(1, "Total bin file size exceeds 99 minutes");
	
	//Now the string can be formed from the values. Need to 0 pad each value
	timestamp.append(padIntStr(minutes, 2) + ":" + padIntStr(seconds, 2) + ":"
	                           + padIntStr(rFrames, 2));
	
	return timestamp;
}

/** Helper functions **********************************************************/
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

std::string padByteStr(size_t bytes, unsigned int pad) {
	//Create the initial string from bytes
	std::string byteStr = std::to_string((bytes));
	
	//Add commas every 3 chars starting from the end
	size_t commaPos = 3;
	while(commaPos < byteStr.length()) {
		byteStr.insert(byteStr.length() - commaPos, ",");
		commaPos += 4;
	}
	
	//Pad if selected
	if(pad != 0 && pad > byteStr.length()) {
		unsigned int padDelta = pad - byteStr.length();
		byteStr.insert(0, padDelta, ' ');
	}
	
	byteStr.append(" Bytes");
	
	return byteStr;
}

std::string padMiBStr(size_t bytes, unsigned int pad) {
	//Create the initial string from bytes / MiB
	std::string MiBStr = std::to_string((bytes / 1048576));
		
	//Pad if selected
	if(pad != 0 && pad > MiBStr.length()) {
		unsigned int padDelta = pad - MiBStr.length();
		MiBStr.insert(0, padDelta, ' ');
	}
	
	MiBStr.append(" MiB");
	
	return MiBStr;
}

std::string padIntStr(size_t val, unsigned int pad) {
	std::string intStr = std::to_string(val);
	
	//Pad if selected
	if(pad != 0 && pad > intStr.length()) {
		unsigned int padDelta = pad - intStr.length();
		intStr.insert(0, padDelta, '0');
	}
	
	return intStr;
}
