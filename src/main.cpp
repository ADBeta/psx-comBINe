/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing
*
* (c) ADBeta
* v3.6
* 26 Sep 2023
*******************************************************************************/
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "helpers.hpp"
#include "sysVars.hpp"
#include "TeFiEd.hpp"
#include "cueHandler.hpp"
#include "CLIah.hpp"

/*** Pre-defined output messages **********************************************/
namespace message {
std::string copyright = "\npsx-comBINe 3.6 26 Sep 2023 ADBeta(c)";

std::string malformed = "Filename (or path in recreate mode) must be the first argument\n";

std::string shortHelp = "Usage: psx-combine [input.cue] [options]\n\
use --help for full help information\n";

std::string longHelp = "Usage: psx-comBINe [input.cue] [options]\n\n\
By default psx-comBINe takes a single input.cue file, creates a directory in\n\
the .cue's parent directory called \"psx-comBINe\".\n\
it will then output the combined .bin and .cue file, leaving the original \n\
files untouched.\n\n\
Options:\n\
-h, --help\t\tShow this help message\n\
-v, --verbose\t\tPrint a verbose CUE sheet diagnostics before dumping\n\
-t, --time\t\tTimes how long the program takes to complete, and prints it\n\n\
-d, --directory\t\tChange the output directory\n\
\t\t\tpsx-combine ./input.cue -d /home/user/games\n\n\
-f, --filename\t\tSpecify the output .cue filename\n\
\t\t\tpsx-combine ./input.cue -f combined_game.cue (or combined_game)\n\n\
-c, --create\t\t(re)Creates a .cue file from a .bin file or a directory\n\
\t\t\tpsx-combine /game/ (or /game/track1.bin for a single bin game) -c\n";
} //namespace message

/*** Enums & Classes **********************************************************/
//Create a config structure to set system configuration
ConfigData Conf;

/*** Forward Declarations *****************************************************/
std::string stringToLower(const std::string &);
int comparePaths(const std::filesystem::path, const std::filesystem::path);

FilesystemType getPathType(std::filesystem::path);
void createCueFromBinFile(SystemVariables &);
void createCueFromBinDir(SystemVariables &);

void combineFiles(SystemVariables &);
void dumpCombinedBins(const SystemVariables &, const CueHandler &, CueHandler &); 

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
	//Time the execution process
	 CLIah::addNewArg(
		"Timed",
		"--time",
		CLIah::ArgType::flag,
		"-t"
	);
	//Create a .CUE file from multiple inputs
	 CLIah::addNewArg(
		"Create",
		"--create",
		CLIah::ArgType::flag,
		"-c"
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

	//Set timed and verbose flags if arguments were passed
	if(CLIah::isDetected("Timed")) Conf.timed = true;
	if(CLIah::isDetected("Verbose")) Conf.verbose = true;
	
	/** Setup *****************************************************************/
	SystemVariables SysVars;
	
	//Create a path from the first argument, and make sure it is valid
	std::filesystem::path argPath(CLIah::stringVector.at(0).string);
	
	try {
		SysVars.inputPathType = getPathType(argPath);
		
		//If the input is invalid, throw an error
		if(SysVars.inputPathType == FilesystemType::invalid) {
			throw input_path_invalid;
		}
		
		//If the input is a file, check the extension
		if(SysVars.inputPathType == FilesystemType::file) {		
			//Set the input path to the parent
			SysVars.inputDir = argPath.parent_path();
			
			//Check which file type the input is
			std::string extension = stringToLower(argPath.extension().string());
			if(extension == ".bin") {
				SysVars.inputBin = argPath;
			} else if(extension == ".cue") {
				SysVars.inputCue = argPath;
			}
		}
		
		//If the input is a path set up the inputDir
		if(SysVars.inputPathType == FilesystemType::directory) {
			SysVars.inputDir = argPath;
		}
				
	} catch(const PrgExcep &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Set up the output path
	try {
		//Make sure the input dir is valid
		if(SysVars.inputDir.empty()) throw input_path_invalid;
		
		//If the argument to output to a custom directory 
		if(CLIah::isDetected("Dir")) {
			std::filesystem::path temp = CLIah::getSubstring("Dir");
			SysVars.outputDir = temp / "";
		} else {		
		//Otherwise use the input Directory
			SysVars.outputDir = SysVars.inputDir / "psx-comBINe";
		}
		
		//Set up an empty directory from the output path
		//If the directory already exists, delete all files in it
		if(std::filesystem::is_directory(SysVars.outputDir)) {
			if(!std::filesystem::remove_all(SysVars.outputDir)) {
				throw cannot_clean_dir;
			}
		}	
	
		//Create the output directory
		if(!std::filesystem::create_directory(SysVars.outputDir)) {
			throw cannot_create_dir;
		}
	} catch(const PrgExcep &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}	

	//If success print message to let user know directory has been created
	std::cout << "Created Directory: " << SysVars.outputDir << std::endl
	          << std::endl;	
	
	/*** Execution Branches ***************************************************/
	//If the recreation mode was selected
	if(CLIah::isDetected("Create")) {
		if(SysVars.inputPathType == FilesystemType::file) {
			createCueFromBinFile(SysVars);
		}
		
		if(SysVars.inputPathType == FilesystemType::directory) {
			createCueFromBinDir(SysVars);
		}
		
		return 0;
	}
	
	//If no special execution args were passed, just combine the files
	combineFiles(SysVars);
	return 0;
}

/*** Aux Functions ************************************************************/
//Convert a string to lowercase
std::string stringToLower(const std::string &in) {
	std::string out;
	
	for(std::string::const_iterator it = in.cbegin(); it != in.cend(); ++it) {
		//NOTE this doesn't support multi-byte chars. fix this later
		out.push_back(tolower( (char)*it) );
	}
	
    return out;
}

int comparePaths(const std::filesystem::path a, const std::filesystem::path b) {
	return a < b;
}

//Take the input filesystem path (Argument [0]) and detect if it is a file,
//directory or invalid
FilesystemType getPathType(std::filesystem::path path) {
	if(std::filesystem::is_regular_file(path)) return FilesystemType::file;
	
	if(std::filesystem::is_directory(path)) return FilesystemType::directory;
	
	//Return the invalid signal to signal a failure
	return FilesystemType::invalid;
}

//Takes an input .bin file and creates a .cue file
void createCueFromBinFile(SystemVariables &vars) {
	try {
		if(vars.inputBin.empty() == true) throw bin_file_not_given;
	} catch(const PrgExcep &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Create an output .cue file handler
	if(CLIah::isDetected("Filename")) {
		vars.outputCue = vars.outputDir / CLIah::getSubstring("Filename");
	} else {
		vars.outputCue = vars.outputDir / vars.inputBin.filename();
	}
	vars.outputCue.replace_extension("cue");
	
	//Open the output cue file and create the file. Exits on failure
	CueHandler cueOut(vars.outputCue.string());
	cueOut.create();
	
	//Push information to the output .cue
	cueOut.pushFILE(vars.inputBin.filename().string(), ftBINARY);
	
	//NOTE This defaults to "MODE2/2352" since that seems common
	cueOut.pushTRACK(1, ttMODE2_2352);
	
	//Pass index, ID 1 and always starts from 00:00:00
	cueOut.pushINDEX(1, 0);
	
	//Print out all of the output CUE data if verbose is enabled
	if(Conf.verbose == true) cueOut.printFILE(cueOut.FILE[0]);
	
	//Write the result of the combination to the cueOut file.
	cueOut.outputCueData();
	cueOut.write();
	
	std::cout << "Wrote output .cue file to " << vars.outputCue << std::endl;
}

void createCueFromBinDir(SystemVariables &vars) {
	//Get all files in the directory passed, ignore any files that are not
	//.bin files, then sort them alphabetically
	std::vector<std::filesystem::path> binFiles;
		
	for(auto const& ent : std::filesystem::directory_iterator(vars.inputDir)) {
		std::filesystem::path file = ent.path();
	
		//Check the file is a .bin file
		std::string extension = stringToLower(file.extension().string());
		if(extension == ".bin") {
			//Add the file path to a vector
			binFiles.push_back(file);
		}
	}
	
	try {
		//Make sure vector is not empty
		if(binFiles.empty()) throw no_bins_in_directory;
	} catch(const PrgExcep &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Sort the file vector alphabetically
	std::sort(binFiles.begin(), binFiles.end(), comparePaths);
	
	//Pre-define a vector iterator and track ID to create the output cue file
	std::vector<std::filesystem::path>::iterator iter = binFiles.begin();
	unsigned int currentTrack = 1;
	
	//Create an output .cue file handler
	if(CLIah::isDetected("Filename")) {
		vars.outputCue = vars.outputDir / CLIah::getSubstring("Filename");
	} else {
		vars.outputCue = vars.outputDir / iter->filename();
	}
	vars.outputCue.replace_extension("cue");
	
	//Open the output cue file and create the file. Exits on failure
	CueHandler cueOut(vars.outputCue.string());
	cueOut.create();
	
	//Push information of the first binary file to the output .cue
	cueOut.pushFILE(iter->filename().string(), ftBINARY);
	cueOut.pushTRACK(currentTrack, ttMODE2_2352);
	cueOut.pushINDEX(1, 0);

	//Incriment the counters, will loop if more files exist, skips if not
	++currentTrack;	
	++iter;
	
	//Loop to push information of all the other files to the output cue
	while(iter < binFiles.end()) {
		cueOut.pushFILE(iter->filename().string(), ftBINARY);
		cueOut.pushTRACK(currentTrack, ttAUDIO);
		cueOut.pushINDEX(0, 0);
		cueOut.pushINDEX(1, 352800); //00:02:00 timestamp
		
		++currentTrack;
		++iter;
	}
	
	//Print out all of the output CUE data if verbose is enabled
	if(Conf.verbose == true) {		
		for(size_t cFile = 0; cFile < cueOut.FILE.size(); cFile++) {
			cueOut.printFILE(cueOut.FILE[cFile]);
		}
	}
	
	//Write the result of the combination to the cueOut file.
	cueOut.outputCueData();
	cueOut.write();
	
	std::cout << "Wrote output .cue file to " << vars.outputCue << std::endl;
}

//"Normal" mode combine routine
void combineFiles(SystemVariables &vars) {
	try {
		if(vars.inputCue.empty() == true) throw cue_not_given;

		//NOTE Will add support for this later
		//If the input path is anything but a file, throw an error
		if(vars.inputPathType != FilesystemType::file) {
			throw combine_dir_not_supported;
		}
	} catch(const PrgExcep &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}	
	
	//Open a new cueHandler object for the input file, Performs validation.
	CueHandler cueIn( vars.inputCue.string() );
	//Read the .cue file into a TeFiEd RAM Vector
	cueIn.read();
	
	
	//Create an output .cue file handler
	if(CLIah::isDetected("Filename")) {
		vars.outputCue = vars.outputDir / CLIah::getSubstring("Filename");
		vars.outputCue.replace_extension("cue");
	} else {
		vars.outputCue = vars.outputDir / vars.inputCue.filename();
	}
	
	//Set output binary file name
	vars.outputBin = vars.outputDir / vars.inputCue.filename();
	vars.outputBin.replace_extension("bin");
	
	//Read the cue file into the FILE.TRACK.INDEX structure.
	std::cout << "Getting Input CUE Data... " << std::flush;
	cueIn.getCueData();
	std::cout << "Done" << std::endl << std::endl;
	
	//Print out all of the input CUE data if verbose is enabled
	if(Conf.verbose == true) {		
		for(size_t cFile = 0; cFile < cueIn.FILE.size(); cFile++) {
			cueIn.printFILE(cueIn.FILE[cFile]);
		}
	}
	
	//Open the output cue file and create the file. Exits on failure
	CueHandler cueOut(vars.outputCue.string());
	cueOut.create();
	
	//Dump the binary files in cueIn to the system var output bin
	dumpCombinedBins(vars, cueIn, cueOut);
	
	//Write the result of the combination to the cueOut file.
	cueOut.outputCueData();
	cueOut.write();
	
	std::cout << "Wrote output .cue file to " << vars.outputCue << std::endl;
}

//Combines input .cue and .bin files into a single .cue and .bin output
void dumpCombinedBins(const SystemVariables &vars, const CueHandler &cueIn, 
                      CueHandler &cueOut) {
	/** Setup *****************************************************************/
	//Clear the output cue data, then set the filename to the sysvar
	cueOut.cleanFILE();
	cueOut.pushFILE(vars.outputBin.filename().string(), cueIn.FILE[0].TYPE);
		
	//Input and output .bin files.
	std::fstream binFileOut, binFileIn;
	
	//Start and End milliseconds
	std::chrono::milliseconds startMillis, endMillis;
	if(Conf.timed == true) startMillis = getMillisecs();

	//Create the output binary file and check it is open
	try {
		binFileOut.open(vars.outputBin, std::ios::out | std::ios::binary);
		std::cout << "Creating " << vars.outputBin << std::endl;
		
		if(!binFileOut) throw cannot_create_bin;
	
	} catch(const PrgExcep &e) {
		std::cerr << vars.outputBin << ": " << e.what() << std::endl;
	}
	
	//Current byte in array, bytes in each file, and total bytes from all files
	size_t arrBytes = 0, fileBytes = 0, totalBytes = 0;
	
	//Create a heap byte array (From defined size in header)
	char *byteArray;
	
	try {
		byteArray = new char[_def_ARR_SIZE];
	} catch(const std::bad_alloc &e) {
		std::cerr << allocate_buffer_fail.what() << std::endl;
	}
	
	/** Runtime ***************************************************************/
	//Go through all bin files in the input cueObject
	for(size_t cueFile = 0; cueFile < cueIn.FILE.size(); cueFile++) {
		std::string binFilename = cueIn.FILE[cueFile].FILENAME;
		std::filesystem::path inputBinPath = vars.inputDir / binFilename;
		
		//Print message about file (flush to ensure the text is printed first)
		std::cout << "Dumping: " << binFilename << "    " << std::flush;
		
		//Open the input bin file
		try {
			//Open input binary file to the current string in the vecor.
			binFileIn.open(inputBinPath, std::ios::in | std::ios::binary);
		
			//error check
			if(!binFileIn) throw cannot_open_bin;
		} catch(const PrgExcep &e) {
			std::cerr << inputBinPath << ": " << e.what() << std::endl;
		}
		
		//Seek to the beginning of the in file to clear flags. Belt and braces
		binFileIn.seekg(0, std::ios::beg);
		
		
		//Create a temporary FILE object from cueIn, to copy data over
		CueHandler::FileData pFILE = cueIn.FILE[ cueFile ];
		
		//Go through all the TRACKs
		for(size_t cTrack = 0; cTrack < pFILE.TRACK.size(); cTrack++) {
			//Temporary TRACK Object
			CueHandler::TrackData pTRACK = pFILE.TRACK[ cTrack ];
			
			//Push pTRACKs info to the output file vect
			cueOut.pushTRACK(pTRACK.ID, pTRACK.TYPE);
			
			//Go through all INDEXs
			for(size_t cIndex = 0; cIndex < pTRACK.INDEX.size(); cIndex++) {
				//Temporary INDEX object
				CueHandler::IndexData pINDEX = pTRACK.INDEX[ cIndex ];
				
				//Push pINDEX info to the combined ref object, offset to the 
				//current byte position
				cueOut.pushINDEX(pINDEX.ID, pINDEX.BYTES + totalBytes);
			}
		}
		
		
		//Get all the bytes from the current input and push them to output.
		int bytesRead;
		do {
			binFileIn.read(byteArray, _def_ARR_SIZE);
			bytesRead = binFileIn.gcount();
			totalBytes += bytesRead;
			fileBytes += bytesRead;
			binFileOut.write(byteArray, bytesRead);
		} while (bytesRead == _def_ARR_SIZE);
		
		//Close the current file for next loop
		binFileIn.close();
		
		//Report how many megabytes the file is, that it is done, then reset.
		std::cout << padMiBStr(fileBytes, 3) << std::endl;
		fileBytes = 0;
	}
	
	//Flush what is left of the byte array to the output file
	if(arrBytes != 0) binFileOut.write(byteArray, arrBytes);

	//Delete heap byte array
	delete[] byteArray;
	
	//Print message that the outfile is waiting to finish wiring
	std::cout << "\nFinishing write to file ... " << std::flush;
	
	//Close the output file
	binFileOut.close();
	
	//If program is being timed, finish timing and print the mesage
	if(Conf.timed == true) {
		endMillis = getMillisecs();
		
		//Work out difference in millis from start to finish
		float deltaSecs = (endMillis - startMillis).count();
		//Then divide by 1000 to set seconds in float format
		deltaSecs /= 1000;
		
		//Use printf instead of cout for native float length settings
		printf("Finished in %.2f Seconds", deltaSecs);
	}
	
	//Report completion and bytes written to the output bin file
	std::cout << "\nSuccessfully dumped " << padByteStr(totalBytes) << " to " 
	          << vars.outputBin << std::endl;
}

