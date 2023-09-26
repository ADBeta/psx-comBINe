/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* (c) ADBeta
*******************************************************************************/
#include <string>
#include <exception>
#include <chrono>

#ifndef HELPERS_H
#define HELPERS_H

/*** Exception handling *******************************************************/
class PrgExcep : public std::exception {
	public:
	PrgExcep(const char* msg) { errMsg = msg; }
	
	const char *what () const throw() { return errMsg; }
	
	private:
	const char *errMsg;
	
};

//binary file dumping Exceptions
extern PrgExcep cannot_create_bin, cannot_open_bin, allocate_buffer_fail;

//Cue recreation Exceptions
extern PrgExcep bin_file_not_given, no_bins_in_directory;

//cueHandler Exceptions
extern PrgExcep file_corrupt, file_unknown_input, line_invalid, 
push_track_order, push_index_order, file_object_fail, timestamp_fail,
timestamp_str_invalid, timestamp_overflow;

//main Exceptions
extern PrgExcep input_path_invalid, cannot_clean_dir, cannot_create_dir,
combine_dir_not_supported, cue_not_given;

/*** Warning Function *********************************************************/
void warnMsg(std::string, std::string);

/*** Helper Functions *********************************************************/
//Prompt user if they wish to continue, and return status
bool promptContinue();

//Convert a string to uppercase
std::string strToUppercase(std::string input);

//Pads and returns a comma seperate string of bytes.
//NOTE: Pads string size not byte number
std::string padByteStr(size_t bytes, unsigned int pad = 0);

//Pads, sizes and returns a string of how many MiBs are in the value passed
std::string padMiBStr(size_t bytes, unsigned int pad = 0);

//Get Miliseconds since epoch 
std::chrono::milliseconds getMillisecs();


#endif
