/******************************************************************************
* psx-comBINe utility header
* ADBeta (c)
******************************************************************************/
#include "utils.hpp"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>

// Take the input filesystem path and returns its type
FilesystemType GetPathType(const std::filesystem::path &path) {
	FilesystemType type = FilesystemType::Invalid;
	
	if(std::filesystem::is_regular_file(path)) {
		type = FilesystemType::File;	
	} else if(std::filesystem::is_directory(path)) {
		type = FilesystemType::Directory;
	}
	
	return type;
}


// Converts an input string to all lowercase
std::string StringToLower(const std::string &in) {
	std::string out;
	
	for(std::string::const_iterator it = in.cbegin(); it != in.cend(); ++it) {
		// NOTE: this doesn't support multi-byte chars. fix this later
		out.push_back(static_cast<char>(tolower(*it)));
	}
	
	return out;
}


// Finds the first file with the passed extension and returns its path
std::filesystem::path FindFileWithExtension(const std::filesystem::path &dir, 
													   const std::string &ext) {	
	std::filesystem::path found_file;
	// Scan through all files in the passed directory
	for(const auto &entry : std::filesystem::directory_iterator(dir)) {
		if(entry.is_regular_file() && entry.path().extension() == ext) {
			found_file = entry.path();
			break;
		}
	}
	
	return found_file;
}


// Get and return the current Milliseconds
std::chrono::milliseconds GetMillisecs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch() );
}


// Converts bytes to a string of its value in MiB, padded with leading spaces
std::string BytesToPaddedMiBString(const size_t bytes, const size_t pad_len) {
	// Convert the input bytes to MiB, then create a string from it
	std::string mib_str = std::to_string((bytes / (1 << 20)));
	
	size_t cur_len = mib_str.length();
	if(pad_len > cur_len) mib_str.insert(0, pad_len - cur_len, ' ');
	
	// Append the Unit String then return it
	mib_str.append(" MiB");
	return mib_str;
}
