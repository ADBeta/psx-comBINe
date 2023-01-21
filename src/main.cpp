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
* v0.18.14
* 21 Jan 2023
*******************************************************************************/
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "helpers.h"
#include "TeFiEd.h"
#include "binHandler.h"
#include "cueHandler.h"

//Vector of filenames pulled from the cueFile.
std::vector<std::string> binFilenameVect;

//Strings used for input/output file system and directory management
std::string baseDirStr, baseFileStr, outDirStr;

//Generate the file system strings from input file (argv[1])
void genFSStrings(const std::string inFile) {
	//Find the last / in the in file, to split it into baseDir and baseFile
	size_t lastSlash = inFile.find_last_of('/');
	
	//Split from 0 to / to get directory
	baseDirStr = inFile.substr(0, lastSlash + 1);
	
	//Append psx-comBINe to the input string for output path
	outDirStr = baseDirStr + "psx-comBINe/";
	
	//Two stage process - First split from the end to the / of the inFile string
	//then substring from 0 to the first . of that string. This is done to 
	//prevent "./dirctory" or "/directory/file.a.b.c" from breaking the string.
	baseFileStr = inFile.substr(lastSlash + 1, inFile.length());
	baseFileStr = baseFileStr.substr(0, baseFileStr.find('.'));	
}

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/** Get user args *********************************************************/
	//Test the user input is correct ** TODO
	if(argc == 1) {
		std::cout << "not enough args" << std::endl;
		return 1;
	}
	
	/** Setup *****************************************************************/
	
	//TODO validate if argv[1] is a filename
	//If argv[1] is a valid file, create a new TeFiEd using that filename, and
	
	
	//Open a new cueHandler object for the input file
	CueHandler cueIn(argv[1]);
	//Read the .cue file in
	cueIn.read();
	
	//Generate the file system strings for use later TODO Make this default behaviour with overwrite
	genFSStrings(std::string(argv[1]));
		
	/** Program execution *****************************************************/
	//If the output directory doesn't exist already, create it.
	if(boost::filesystem::is_directory(outDirStr) == false) {
		//Watch for errors creating output directory
		if( boost::filesystem::create_directory(outDirStr) == false) {
			errorMsg(2, "Cannot create output director. Check privileges");
		}
		
		//If success print message to let user know directory has been created
		std::cout << "Created Directory: " << outDirStr << std::endl;
	}
	
	
	//Get the FILE strings from the input cue file and store them in a vector
	//cueIn.pushFILEToVector(binFilenameVect);
	
	cueIn.getCueData();
	
	
	//Open the output cue file and create the file. Exits on failure
	//CueHandler cueOut("./test.cue");
	//cueOut.create();
	
	//cueOut.TrackData[0].indexByte[0] = 69;
	
	//std::cout << cueOut.TrackData[0].indexByte[0] << std::endl;
	/*
	
	
	
	cueOut.newTRACK(1,"AUDIO");
	cueOut.newTRACK(2,"AUDIO");
	cueOut.newTRACK(3,"AUDIO");
	cueOut.newTRACK(4,"AUDIO");
	
	
	cueOut.write();

	*/
	/*
	
	//Dump the binary filename vect to the output binary file.
	if(dumpBinFiles(binFilenameVect, (outDirStr + baseFileStr + ".bin")) != 0) {
		errorMsg(2, "Exiting - Could not dump binary files");
	}
	//Print blank line for readability
	std::cout <<std::endl;
	
	for(size_t indx = 0; indx < fileIndexByte.size(); indx++) {
		size_t crnt = fileIndexByte.at(indx); 
		
		std::cout << getTimestamp(crnt) << std::endl;	
	}
	
	
	//std::cout << "FILE \"" << baseFilename << ".bin" << "\" BINARY" << std::endl;
	*/
	//Done :)
	
	return 0;
}
