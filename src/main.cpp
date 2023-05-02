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
* v2.1.0
* 08 Mar 2023
*******************************************************************************/
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

#include "helpers.hpp"
#include "TeFiEd.hpp"
#include "binHandler.hpp"
#include "cueHandler.hpp"
#include "CLIah.hpp"

/*** OS Detection *************************************************************/
#ifdef WIN32
	#define DIR_DELIM '\\'
	#define DIR_SUFFIX "psx-comBINe\\"
#else
	#define DIR_DELIM '/'
	#define DIR_SUFFIX "psx-comBINe/"
#endif

/*** Pre-defined output messages **********************************************/
namespace message {
std::string copyright = "\npsx-comBINe 2.1.1 08 March 2023 ADBeta(c)";

std::string malformed = "Filename must be the first argument\n";

std::string shortHelp = "Usage: psx-comBINe [input.cue] [options]\n\
use --help for full help information\n";

std::string longHelp = "Usage: psx-comBINe [input.cue] [options]\n\n\
By default psx-comBINe takes a single input.cue file, creates a directory in\n\
the .cue's parent directory called \"psx-comBINe\".\n\
it will then output the combined .bin and .cue file, leaving the original \n\
files untouched.\n\n\
Options:\n\
-h, --help\t\tShow this help message\n\
-d, --directory\t\tChange the output directory\n\
-f, --filename\t\tSpecify the output filenames instead of copying the input\n\
-v, --verbose\t\tPrint a verbose CUE sheet diagnostics before dumping\n";
} //namespace message

/*** Main functions ***********************************************************/
//Vector of filenames pulled from the cueFile.
std::vector<std::string> binFilenameVect;

//Filesystem strings. Input and output directory, and the base filename of the 
//input, eg input "./game/game.cue" to use "game" for input and output files
std::string inputDirString, outputDirString, baseFileString;

//Output bin and cue path + filename + extension. Set later
std::string outCueFileString, outBinFileString;

//Generate the file system strings from input file (argv[1])
void generateFilesystemStrings(const std::string inFilename) {
	//Find the last delim (/ or \) position, to split the string
	size_t lastSlashPos = inFilename.find_last_of( DIR_DELIM ) + 1;
	
	//Split from 0 to the delim to get the input directory
	inputDirString = inFilename.substr(0, lastSlashPos);
	
	//If the directory flag is set, set the output Directory to that, otherwise,
	//use the default */psx-comBINe/ direcory
	if(CLIah::isDetected("Dir")) {
		outputDirString = CLIah::getSubstring("Dir");
		
		//Make sure the output directory passed has a termination DELIM.
		if(outputDirString.back() != DIR_DELIM) {
			outputDirString.push_back( DIR_DELIM );
		}
		
	} else {
		//Append psx-comBINe to the input string for output path
		outputDirString = inputDirString + DIR_SUFFIX;
	}
	
	//Set the output filename base (./directory/file to file)
	//If filename flag is set, trust the users string
	if(CLIah::isDetected("Filename")) {
		baseFileString = CLIah::getSubstring("Filename");
	}  else {
		//First split from the last delim (/ or \) to the end of the string
		baseFileString = inFilename.substr(lastSlashPos, inFilename.length());
		//Then split the first substring from 0 to the first "." 
		//this prevents "./dirctory" or "/directory/file.a.b.c" from breaking 	
		baseFileString = baseFileString.substr(0, baseFileString.find('.'));
	}
}

/*** Main *********************************************************************/
int main(int argc, char *argv[]){
	/*** Define CLIah Arguments ***********************************************/
	//CLIah::Config::verbose = true; //Set verbosity when match is found
	CLIah::Config::stringsEnabled = true; //Set arbitrary strings allowed
	
	CLIah::addNewArg(
		"Help",                 //Reference
		"--help",               //Primary match string
		CLIah::ArgType::flag,   //Argument type
		"-h"                    //Alias match string
	);
	//Change output directory
	CLIah::addNewArg(
		"Dir",
		"--directory",
		CLIah::ArgType::subcommand,
		"-d"
	);
	//Change output filenames
	CLIah::addNewArg(
		"Filename",
		"--filename",
		CLIah::ArgType::subcommand,
		"-f"
	);
    //Make psx-comBINe verbose
    CLIah::addNewArg(
		"Verbose",
		"--verbose",
		CLIah::ArgType::flag,
		"-v"
	);

	/** User Argument handling ************************************************/
	//Get CLIah to scan the CLI Args
	CLIah::analyseArgs(argc, argv);
	
	//If help was requested, print the long help message then exit.
	if( CLIah::isDetected("Help") ) {
		std::cout << message::longHelp << message::copyright << std::endl;
		return 0;
	}
	
	//Make sure that at least one string exists and is the first argument passed
	if( CLIah::stringVector.size() == 0 ) {
		std::cout << message::shortHelp << message::copyright << std::endl;
		return 1;
	}
	
	//Compare first string with argv[1] to make sure it is the first arg
	 if( CLIah::stringVector.at(0).string.compare(argv[1]) != 0 ) {
	 	std::cout << message::malformed << message::shortHelp 
	 	          << message::copyright << std::endl;
		return 1;
	 }
	
	/** Setup *****************************************************************/
	//Open a new cueHandler object for the input file, Performs validation.
	CueHandler cueIn( CLIah::stringVector.at(0).string );
	//Read the .cue file into a TeFiEd RAM Vector
	cueIn.read();
	
	//Generate the file system strings, handles -d option
	generateFilesystemStrings( CLIah::stringVector.at(0).string );
	
	//Create output .cue and .bin output filenames
	outCueFileString = outputDirString + baseFileString + ".cue";
	outBinFileString = outputDirString + baseFileString + ".bin";

	/**************************************************************************/
	//Read the cue file into the FILE.TRACK.INDEX structure.
	std::cout << "Getting input CUE Data... " << std::flush;
	cueIn.getCueData();
	std::cout << "Done" << std::endl << std::endl;
	
	//Print out all of the input CUE data if verbose is enabled
	if(CLIah::isDetected("Verbose")) {
		for(size_t cFile = 0; cFile < cueIn.FILE.size(); cFile++) {
			cueIn.printFILE(cueIn.FILE[cFile]);
		}
	}
	
	//Populate the binFilenameVect from the cue vect object
	for(size_t cFile = 0; cFile < cueIn.FILE.size(); cFile++) {
		//Create a string of the file path to each bin file
		std::string binFilePath = inputDirString + cueIn.FILE[cFile].FILENAME;
		binFilenameVect.push_back(binFilePath);
	}
	
	/**************************************************************************/
	//If the output directory doesn't exist already, create it.
	if(std::filesystem::is_directory(outputDirString) == false) {
		//Watch for errors creating output directory
		if(std::filesystem::create_directory( outputDirString ) == false) {
			errorMsg(2, "", "Cannot create output directory. Check privileges");
		}
		
		//If success print message to let user know directory has been created
		std::cout << "Created Directory: " << outputDirString << std::endl;
	}
	
	/**************************************************************************/
	//Dump the binary files (via binFilenameVect) to the output binary file
	if(dumpBinFiles(binFilenameVect, outBinFileString) != 0) {
		errorMsg(2, "main", "Could not dump binary files: ");
	}
	
	/**************************************************************************/
	//Open the output cue file and create the file. Exits on failure
	CueHandler cueOut(outCueFileString);
	cueOut.create();
	
	//Combine the data from all FILES in cueIn, to a single file on cueOut.
	//Pass the cueOut object, the relative .bin name (NOT OUTPUT BIN STRING)
	cueIn.combineCueFiles(cueOut, (baseFileString + ".bin"), binOffsetBytes);
	
	//Write the result of the combination to the cueOut file.
	cueOut.outputCueData();
	cueOut.write();
	
	//Exectuion is done.
	return 0;
}
