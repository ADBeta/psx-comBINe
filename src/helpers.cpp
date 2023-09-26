/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* (c) ADBeta
*******************************************************************************/
#include "helpers.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

/*** Exception Handling *******************************************************/
//binary dumping Exceptions
PrgExcep cannot_create_bin("Cannot create binary output binary file.");
PrgExcep cannot_open_bin("Cannot open binary file.");
PrgExcep allocate_buffer_fail("Could not allocate the binary copy buffer");

//Cue recreation exceptions
PrgExcep bin_file_not_given("No Binary file given during a (re)create operation");
PrgExcep no_bins_in_directory("No Binary files available in the passed directory");

//cueHandler Exceptions
PrgExcep file_corrupt("File is Corrupted or Unsupported");
PrgExcep file_unknown_input("File Contains Unknown Input");
PrgExcep line_invalid("Current Line in .cue File is Invalid");
PrgExcep push_track_order("Attempt to push TRACK before FILE. (Missing FILE line)");
PrgExcep push_index_order("Attempt to push INDEX before TRACK. (Missing TRACK line)");
PrgExcep file_object_fail("Selected FILE Object does is invalid");
PrgExcep timestamp_fail("cue timestamp and SECTORS do not match (bytes mismatch)");
PrgExcep timestamp_str_invalid("cue timestamp string is not valid (Wrong length)");
PrgExcep timestamp_overflow("cue timestamp exceeds 99 Minutes");

//Main Exceptions
PrgExcep input_path_invalid("The Input Path is invalid");
PrgExcep cannot_clean_dir("Cannot clean the output directory. Check privelages");
PrgExcep cannot_create_dir("Cannot create output directory. Check privelages");
PrgExcep combine_dir_not_supported("Combining .cue files does not support directories as inpt");
PrgExcep cue_not_given("input .cue file was not given as the first argument");

/*** Warning Function *********************************************************/
void warnMsg(std::string funct, std::string msg) {
	std::cerr << "Warning in function: " << funct << ": " << msg << std::endl;
}

/*** Helper Functions *********************************************************/
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

std::string strToUppercase(std::string input) {
	std::transform(input.begin(), input.end(), input.begin(), 
		[](unsigned char chr){ return std::toupper(chr); }
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

std::chrono::milliseconds getMillisecs() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

