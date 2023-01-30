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
* v1.0.1
* 30 Jan 2023
*******************************************************************************/
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "helpers.hpp"
#include "TeFiEd.hpp"
#include "binHandler.hpp"
#include "cueHandler.hpp"

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
		std::cout << "not enough args. Please provide a .cue file to combine"
		          << std::endl;
		return 1;
	} 
	
	/** Setup *****************************************************************/
	
	//TODO validate if argv[1] is a filename
	//If argv[1] is a valid file, create a new TeFiEd using that filename, and
	
	
	//Open a new cueHandler object for the input file
	CueHandler cueIn(argv[1]);
	//Read the .cue file in TODO
	cueIn.read();
	
	
	//Generate the file system strings for use later TODO Make this default behaviour with overwrite
	genFSStrings(std::string(argv[1]));
		
	
	//Read the cue file into the FILE.TRACK.INDEX vector structure.
	std::cout << "Getting input CUE Data... " << std::flush;
	cueIn.getCueData();
	std::cout << "Done" << std::endl << std::endl;
	
	
	//Populate the binFilenameVect from the cue vect object
	for(size_t cFile = 0; cFile < cueIn.FILE.size(); cFile++) {
		binFilenameVect.push_back(baseDirStr + cueIn.FILE[cFile].FILENAME);
	}
	
	
	/** Program execution *****************************************************/
	//If the output directory doesn't exist already, create it.
	if(boost::filesystem::is_directory(outDirStr) == false) {
		//Watch for errors creating output directory
		if( boost::filesystem::create_directory(outDirStr) == false) {
			errorMsg(2, "Main", "Cannot create output director. Check privileges");
		}
		
		//If success print message to let user know directory has been created
		std::cout << "Created Directory: " << outDirStr << std::endl;
	}
	
	
	//Dump the binary files (found via binFilenameVect) to the output binary file
	if(dumpBinFiles(binFilenameVect, (outDirStr + baseFileStr + ".bin")) != 0) {
		errorMsg(2, "main", "Could not dump binary files");
	}
	
	
	//Open the output cue file and create the file. Exits on failure
	std::string outCueFilename = outDirStr + baseFileStr + ".cue";
	CueHandler cueOut( outCueFilename.c_str() );
	cueOut.create();
	
	
	//Combine the data from all FILES in cueIn, to a single file on cueOut.
	cueIn.combineCueFiles( cueOut, baseFileStr + ".bin", binOffsetBytes );
	
	
	//Write the result of the combination to the cueOut file.
	cueOut.outputCueData();
	cueOut.write();
	
	//Exectuion is done.
	return 0;
}
