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
* v0.11.8
* 11 Jan 2023
*******************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "common.h"
#include "TeFiEd.h"
#include "fileHandler.h"

//Input and output .cue file TeFiEd pointers. Global to all modules
TeFiEd *cueFileIn, *cueFileOut;

//Input and output .bin files. Global to all modules.
std::fstream binFileOut, binFileIn;

//Vector of filenames pulled from the cueFile. Global to all modules.
std::vector<std::string> binFilenameVect;

int main(int argc, char *argv[]){
	/** Get user args *********************************************************/
	//Test the user input is correct ** TODO
	if(argc == 1) {
		std::cout << "not enough args" << std::endl;
		return 1;
	}
	
	/** Setup *****************************************************************/
	
	//TODO
	//If argv[1] is a valid file, create a new TeFiEd using that filename, and
	//assign it to cueFile
	cueFileIn = new TeFiEd(argv[1]);
	//Set safety flag on the .cue input file. 100KB
	cueFileIn->setByteLimit(102400);
	
	cueFileIn->setVerbose(true);
	//Read the .cue file in
	if(cueFileIn->read() != 0) return 1;
	
	
	//Declare Output Directory and Output filenames for the .cue and .bin files.                                            
	std::string outDir, cueFileOutFilename, binFileOutFilename;
	//Output directory is input .cue parent directory, with psx-comBine appended
	outDir = cueFileIn->parentDir() + "psx-comBINe/";
	//Output cue filename is outDir + file name + .cue
	cueFileOutFilename = outDir + getFileName(cueFileIn->filename()) + ".cue";
	//Output bin filename is outDir + file name + .bin
	binFileOutFilename = outDir + getFileName(cueFileIn->filename()) + ".bin";
		
	/** Program execution *****************************************************/
	//Check each line that has FILE in it
	size_t matchLineNo;
	while(( matchLineNo = cueFileIn->findNext("FILE") )) {
		//Keep the current string rather than keep calling getLine()
		std::string cLineStr = cueFileIn->getLine(matchLineNo);
		
		//If the current line isn't valid, prompt with continue message.
		//Exit if false, continue if true.
		if(lineIsValid(cLineStr) == false) {
			if(promptContinue() == false) return 1;
		}
		
		//Push the filename string to the vector.
		binFilenameVect.push_back(cueFileIn->parentDir() + 
		                          getFileFromCueLine(cLineStr));
	}
	
	
	//Create output directory, and setup .cue and .bin output file objects.
	if(openOutputFiles(outDir, cueFileOutFilename, binFileOutFilename) != 0) {
		errorMsg(2, "Exiting - Could not finish output setup. Check privilege");
	}
	//Print blank line for readability
	std::cout << std::endl;
	
	
	//Dump the binary filename vect to the binOutFile. Very verbose
	if(dumpBinFiles(binFilenameVect, binFileOutFilename) != 0) {
		errorMsg(2, "Exiting - Could not dump binary files");
	}
	//Print blank line for readability
	std::cout << std::endl;
	
	
	for(size_t indx = 0; indx < fileIndexByte.size(); indx++) {
		size_t crnt = fileIndexByte.at(indx); 
		
		std::cout << getTimestamp(crnt) << std::endl;	
	}
	
	//Done :)
	
	
	cueFileIn->flush();
	cueFileOut->flush();
	return 0;
}
