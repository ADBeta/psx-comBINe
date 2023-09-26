/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* (c) ADBeta
*******************************************************************************/
#ifndef SYSVARS_H
#define SYSVARS_H

#include <filesystem>

//Define how large the RAM byte array while dumping should be. (4KB)
#define _def_ARR_SIZE 4096

struct ConfigData {
	bool verbose = false;
	bool timed = false;
};

extern ConfigData Conf;


enum class FilesystemType { file, directory, invalid };

//Variables used by the main system, passed while changing execution path
struct SystemVariables {
	FilesystemType inputPathType;
	//Paths
	std::filesystem::path inputDir, outputDir;
	//Cue Files	
	std::filesystem::path inputCue, outputCue;
	//Bin Files
	std::filesystem::path inputBin, outputBin;	
};

#endif
