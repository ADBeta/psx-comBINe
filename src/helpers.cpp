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
#include "helpers.hpp"

#include <iostream>
#include <string>
#include <algorithm>

/*** Error message handler ****************************************************/
//errLevel = 0 = warn, 1 = error (non fatal), 2 = error (fatal)

void errorMsg(unsigned int errLevel, std::string funct, std::string errStr) {
              	
	if(errLevel == 0) {
		std::cerr << "Warn: ";
	} else {
		std::cerr << "Error: ";
	}
	
	//Print the function, then the user message
	std::cerr << funct << ": " << errStr << std::endl;
	
	//If the errLevel is > 1 then exit the program as a fatal error
	if(errLevel > 1) exit(EXIT_FAILURE);
}

void errorMsg(unsigned int errLevel, std::string funct, int errEnum) {
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


/***  *************************************************************************/
//Stolen from CLIah with new name
std::string strToUppercase(std::string input) {
	//Transform seems to be the recommended method - causes another include 
	//and may cause other unknown issues. For practice, this will stay for now,
	//But may be replaced with a direct index (or iterator) in future.
	std::transform(input.begin(), input.end(), input.begin(), 
		//operation is getting the current char and performing toupper
		[](unsigned char chr){ return std::toupper(chr); } // correct
	);
	
	//Return the uppercase string
	return input;
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
