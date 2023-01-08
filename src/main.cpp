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
* v0.4.2
* 08 Jan 2023
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>

#include "common.h"
#include "TeFiEd.h"
#include "fileHandler.h"


//Cue file TeFiEd Object. Gets Set later.
TeFiEd *cueFile;

//Vector of filenames pulled from the cueFile.
std::vector<std::string> binFile;

int main(int argc, char *argv[]){

	//Test the user input is correct ** TODO
	if(argc == 1) {
		std::cout << "not enough args" << std::endl;
		return 1;
	}
	
	//If argv[1] is a valid file, create a new TeFiEd using that filename, and
	//assign it to cueFile, then read cueFile into the vector.
	cueFile = new TeFiEd(argv[1]);
	if(cueFile->read() != 0) return 1;
	
	//Get the parent directory of the .cue file and save to a string.
	std::string baseDir = cueFile->filename();
	baseDir = baseDir.substr(0, baseDir.find_last_of('/')+1);
	
	//Check each line that has FILE in it
	size_t matchLineNo;
	while(( matchLineNo = cueFile->findNext("FILE") )) {
		//Keep the current string rather than keep calling getLine()
		std::string currentLineStr = cueFile->getLine(matchLineNo);
		
		if(isLineValid(currentLineStr)) {
			//Push the filename string to the vector.
			binFile.push_back(baseDir + getFilenameFromLine(currentLineStr));
		}
	}
	
	for (auto i: binFile)
		std::cout << i << std::endl;
	
	const char* binOutFile = (cueFile->filename())->replace(".cue", ".bin");
	
	std::cout << binOutFile << std::endl;
		
	return 0;
}
