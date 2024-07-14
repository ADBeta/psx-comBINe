/******************************************************************************
* psx-comBINe utility header
* ADBeta (c)
******************************************************************************/
#ifndef PSXCOMBINE_UTILS
#define PSXCOMBINE_UTILS

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>

// What type a File System path is
enum class FilesystemType {File, Directory, Invalid};

/// @breif Take an input filesystem path and returns its type
/// @param path, filesystem path to parse
/// @return FilesystemType enum 
FilesystemType GetPathType(const std::filesystem::path &path);

/// @breif Finds the first file with the passed extension and returns its path
/// @param filesystem path to start looking from
/// @param extension string, .cue for example
/// @return filesystem path to the first file
std::filesystem::path FindFileWithExtension(const std::filesystem::path &path, 
											const std::string &ext);

/// @breif Returns the current Milliseconds
/// @param none
/// @return current millis
std::chrono::milliseconds GetMillisecs();

/// @breif Takes an input string, converts to all lowercase
/// @param input string
/// @return string in all lower case
std::string StringToLower(const std::string &input);

/// @breif Takes number of bytes, converts it to a string in MiB, 
/// padded with leading spaces
/// @param bytes, number of bytes, to be converted to MiB string
/// @param pad_length, number of chars to pad the string to in total
/// @return String of bytes in MiB, padded with spaces
std::string BytesToPaddedMiBString(const size_t bytes, const size_t pad_len);


#endif
