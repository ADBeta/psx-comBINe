/*******************************************************************************
* This file is part of psx-comBINe. 
* Please see the github: https://github.com/ADBeta/psx-comBINe
* This and all other files relating to psx-conBINe are under the GPLv2.0 License
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing
*
* ADBeta (c)	01 Jan 2024	v4.9.3
*******************************************************************************/
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "cuehandler.hpp"
#include "clampp.hpp"

/*** wx-widgets guff *********************************************************/
// Disable all the warnings JUST for wx-widgets
// Editor note: fix your shit wx. That many warnings is a joke
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
////
#include <wx/wx.h>
////
#pragma GCC diagnostic pop

// Forward declare the wxWidgets app initialization
extern wxAppConsole *wxCreateApp();
extern wxAppInitializer wxTheAppInitializer;


/*** Globals *****************************************************************/
//Define how large the RAM byte array while dumping should be. (4KB)
#define _BINARY_ARRAY_SIZE 4096

namespace message {
const char *copyright = "\npsx-comBINe v4.9.3 01 Jan 2024 ADBeta(c)";

const char *short_help = 
"Usage: psx-combine [input.cue or directory] [options]\n\
use --help for full help information\n";

const char *long_help = 
"Usage: psx-combine [input.cue or directory] [options]\n\n\
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
\t\t\tpsx-combine ./input.cue -f combined_game.cue (or combined_game)\n\n";

//Messages for throw()
const char *missing_filepath = "Filename or directory was not specified";
const char *invalid_filepath = "Input path is invalid, or does not exist";
const char *filepath_bad_extension = "Input file must be a .cue file";
const char *dir_missing_cue = "Input directory does not contain any .cue files";
const char *cue_has_no_files = "Input .cue file has no FILEs";

const char *filename_has_dir = "filename argument must only contain a filename";
const char *filename_bad_extension = "filename extension must be .cue";

const char *input_bin_not_open = "The input file could not be opened";
const char *output_bin_create_failed = "Output binary file could not be created";
} //namespace message


/*** Enums & Classes **********************************************************/
enum class FilesystemType {File, Directory, Invalid};
//clampp Argument handler object
ClamppClass Args;


/*** Forward Declarations *****************************************************/
//Take the input filesystem path and returns its type
FilesystemType GetPathType(const std::filesystem::path&);

//Finds the first file with the passed extension and returns its path
std::filesystem::path FindFileWithExtension(const std::filesystem::path&, 
															const std::string&);

//Get and return the current Milliseconds
std::chrono::milliseconds GetMillisecs();


//Converts an input string to all lowercase
std::string StringToLower(const std::string&);
//Converts bytes to a string of its value in MiB, padded with leading spaces
std::string BytesToPaddedMiBString(const size_t bytes, const size_t pad_len);


//Combines all file inside the original cue sheet to a single .bin file
void DumpBinFile(const CueSheet &, const std::filesystem::path &input_dir_path,
				const std::filesystem::path &output_bin_path, const bool timed);

/*** Main *********************************************************************/
int main(const int argc, const char *argv[]) {

	// Initialize wxWidgets
	wxDISABLE_DEBUG_SUPPORT();
	wxApp::SetInstance(wxCreateApp());
	wxEntryStart(const_cast<int&>(argc), const_cast<char**>(argv));
	wxTheApp->CallOnInit();
	
	// Start the wxWidgets main loop
	wxTheApp->OnRun();
	
	// Clean up wxWidgets
	wxTheApp->OnExit();
	wxEntryCleanup();


	// TODO: Only run wx when gui specified.
	// convert main to function for duming bin file. call from wx or cli
	while(true);;

	/*** Local variables for later use ****************************************/
	//Filesystem variables
	FilesystemType input_path_type;
	std::filesystem::path input_cue_path,  input_dir_path,
						  output_cue_path, output_dir_path, output_bin_path;
		
	/*** Define clampp Arguments **********************************************/
	ClamppConfig::allow_undefined_args = true;
	int arg_index_help = Args.AddDefinition("--help", "-h", false);
	int arg_index_dir = Args.AddDefinition("--directory", "-d", true);
	int arg_index_file = Args.AddDefinition("--filename", "-f", true);
	int arg_index_timed = Args.AddDefinition("--time", "-t", false);
	int arg_index_verbose = Args.AddDefinition("--verbose", "-v", false);
	
	/** User Argument handling ************************************************/
	//Scan the arguments
	int scan_ret = Args.ScanArgs(argc - 1, argv + 1);
	
	//Make sure at least one argument was passed
	if(scan_ret == CLAMPP_ENOARGS) {
		std::cerr << message::short_help << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//If no substring was given when expected, error
	if(scan_ret == CLAMPP_ENOSUBSTR) {
		std::cerr << "Argument is missing a parameter. " << message::short_help 
				  << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//If help was requested, print the long help message then exit.
	if(Args.GetDetectedStatus(arg_index_help) == true) {
		std::cout << message::long_help << message::copyright << std::endl;
		exit(EXIT_SUCCESS);
	}
	
	/*** Filesystem handling **************************************************/
	try {
		//Make sure a filepath string has been passed
		if(Args.GetUndefinedArg(0).empty()) {
			throw std::invalid_argument(message::missing_filepath);
		}
	
	/*** Input ***/
		//Create a path from the first undefined argument input
		std::filesystem::path arg_filepath(Args.GetUndefinedArg(0));
		//Get its type and guard against an invalid input
		input_path_type = GetPathType(arg_filepath);
		if(input_path_type == FilesystemType::Invalid) {
			throw std::invalid_argument(message::invalid_filepath);
		}
				
		//If the input is a file, check the extension
		if(input_path_type == FilesystemType::File) {
			//Make sure the dir_path has a trailing / delim
			input_dir_path = arg_filepath.parent_path() / "";
			
			//Make sure any file input is a .cue file
			if(StringToLower(arg_filepath.extension().string()) != ".cue") {
				throw std::invalid_argument(message::filepath_bad_extension);
			}
			
			input_cue_path = arg_filepath;
		}
		
		//If the input is a path set the input_dir
		if(input_path_type == FilesystemType::Directory) {
			input_dir_path = arg_filepath / "";
			
			//Look for a .cue file in the directory. Error if one is not found
			input_cue_path = FindFileWithExtension(input_dir_path, ".cue");
			if(input_cue_path.empty()) {
				throw std::invalid_argument(message::dir_missing_cue);
			}
		}
		
	/*** Output ***/
		//Set the output directory to the -d argument if given; if not, set it
		//to the input directory + /psx-comBINe/
		if(Args.GetDetectedStatus(arg_index_dir) == true) {
			output_dir_path = 
				   std::filesystem::path(Args.GetSubstring(arg_index_dir)) / "";
		} else {
			output_dir_path = input_dir_path / "psx-comBINe" / "";
		}
		
		//If the directory does not already exists, create it
		if(!std::filesystem::is_directory(output_dir_path)) {
			std::filesystem::create_directory(output_dir_path);
			std::cout << "Created Directory: " << output_dir_path << "\n\n";
		}
		
		//Create output binary and cue filename. If override was given use that,
		//if not, use the input .cue name
		if(Args.GetDetectedStatus(arg_index_file) == true) {
			std::filesystem::path out_file_tmp(Args.GetSubstring(arg_index_file));
			
			//Make sure the filename is NOT a directory
			if(out_file_tmp.has_parent_path()) {
				throw std::invalid_argument(message::filename_has_dir);
			}
			//Make sure the extension is .cue
			if(StringToLower(out_file_tmp.extension().string()) != ".cue") {
				throw std::invalid_argument(message::filename_bad_extension);
			}
			
			//Append the custom .cue filename to the output .cue and .bin paths
			output_cue_path = output_dir_path / out_file_tmp;
			(output_bin_path = output_cue_path).replace_extension("bin");
			
		//If no override was passed, use the input .cue filename for output
		} else {
			output_cue_path = output_dir_path / input_cue_path.filename();
			(output_bin_path = output_cue_path).replace_extension("bin");
		}
		
	} catch(const std::exception &e) {
		std::cerr << "Fatal Error: Filesystem: " << e.what() << "\n\n"
				  << message::short_help << std::endl;
		exit(EXIT_FAILURE);
	}
	
	/*** Cue Handling *********************************************************/
	//Create a CueSheet object to hold original Sheet data, and one combined
	//sheet for writing to a file
	CueSheet cue_original, cue_combined;
	
	//Create an input and output .cue file handlers from the filesystem paths
	CueFile cue_in(input_cue_path.string().c_str());
	CueFile cue_out(output_cue_path.string().c_str());

	try {
		//Read the cue sheet data in, make sure there is at least one FILE
		cue_in.ReadCueData(cue_original);
		if(cue_original.FileList.empty()) {
			throw CueException(message::cue_has_no_files);
		}
		
		//Read the FILE sizes. Throws an exception on failure
		cue_in.GetCueFileSizes(cue_original, input_dir_path.string());
		
		//Copy the original sheet info to the combined sheet, then combine.
		cue_original.CopyTo(cue_combined);
		cue_combined.Combine(output_bin_path.filename().string(), "BINARY");
		
		//If the verbose flag was passed, print the combined sheet
		if(Args.GetDetectedStatus(arg_index_verbose) == true) {
			cue_combined.Print();
		}
		
		//Write the combined .cue file out to the cue_out file
		cue_out.WriteCueData(cue_combined);
		
	} catch(const CueException &e) {
		std::cerr << "Fatal Error: Cue Handler: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	/*** Binary Handling ******************************************************/
	//Combine the binary files from input path, to output bin path, and pass
	//the timed parameter
	DumpBinFile(cue_original, input_dir_path, output_bin_path, 
									   Args.GetDetectedStatus(arg_index_timed));
	
	//Done
	return 0;
}




/*** Aux Functions ************************************************************/
//Take the input filesystem path and returns its type
FilesystemType GetPathType(const std::filesystem::path &path) {
	FilesystemType type = FilesystemType::Invalid;
	
	if(std::filesystem::is_regular_file(path)) {
		type = FilesystemType::File;	
	} else if(std::filesystem::is_directory(path)) {
		type = FilesystemType::Directory;
	}
	
	//Return the invalid signal to signal a failure
	return type;
}

//Converts an input string to all lowercase
std::string StringToLower(const std::string &in) {
	std::string out;
	
	for(std::string::const_iterator it = in.cbegin(); it != in.cend(); ++it) {
		//NOTE this doesn't support multi-byte chars. fix this later
		out.push_back(static_cast<char>(tolower(*it)));
	}
	
	return out;
}

//Finds the first file with the passed extension and returns its path
std::filesystem::path FindFileWithExtension(const std::filesystem::path &dir, 
													   const std::string &ext) {	
	std::filesystem::path found_file;
	//Scan through all files in the passed directory
	for(const auto &entry : std::filesystem::directory_iterator(dir)) {
	
		if(entry.is_regular_file() && entry.path().extension() == ext) {
			found_file = entry.path();
			break;
		}
	}
	
	return found_file;
}

//Get and return the current Milliseconds
std::chrono::milliseconds GetMillisecs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
						  (std::chrono::system_clock::now().time_since_epoch());
}

//Converts bytes to a string of its value in MiB, padded with leading spaces
std::string BytesToPaddedMiBString(const size_t bytes, const size_t pad_len) {
	//Convert the input bytes to MiB, then create a string from it
	std::string mib_str = std::to_string((bytes / (1 << 20)));
	
	size_t cur_len = mib_str.length();
	if(pad_len > cur_len) mib_str.insert(0, pad_len - cur_len, ' ');
	
	//Append the Unit String then return it
	mib_str.append(" MiB");
	return mib_str;
}

//Combines all file inside the original cue sheet to a single .bin file
void DumpBinFile(const CueSheet &cs, const std::filesystem::path &in_dir_path,
			   const std::filesystem::path &output_bin_path, const bool timed) {
	//Define the Begin and End Milliseconds for timed mode. Only use them if
	//timed mode was selcted
	std::chrono::milliseconds beg_millis, end_millis;
	if(timed) beg_millis = GetMillisecs();
	
	//Create an output file from the out_bin_path, and a file that will switch
	//between each of the FILEs inside the cue sheet
	std::fstream bin_out, bin_in;
	
	//Open/Create the output binary file. Make sure it is open, catch errors
	try {
		bin_out.open(output_bin_path, 
							std::ios::out | std::ios::binary | std::ios::trunc);
		if(!bin_out) throw std::runtime_error(message::output_bin_create_failed);

	} catch(const std::exception &e) {
		std::cerr << "Fatal Error: Creating " << output_bin_path << ": " 
				  << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	
	/*** Runtime ***/
	//Variables to keep track of total bytes copied to the output file, bytes 
	//copied just from the current file, and bytes read to the buffer per loop
	size_t total_output_bytes = 0, current_file_bytes = 0;
	std::streamsize bytes_read_to_buffer = 0;
	
	//Create an array on the heap for the binary copy operations
	char *bin_array = new char[_BINARY_ARRAY_SIZE];
	
	//Go through all files in the CueSheet
	for(const auto &f_itr : cs.FileList) {
		//Create a filepath from the input directory + the current cue filename
		std::filesystem::path cur_bin(in_dir_path / f_itr.filename);
		std::cout << "Dumping File " << cur_bin << std::flush;
		
		//Try to open the file
		try {
			bin_in.open(cur_bin, std::ios::in | std::ios::binary);
			if(!bin_in) std::runtime_error(message::input_bin_not_open);
			
		} catch(const std::exception &e) {
			std::cerr << "Fatal Error: Opening input binary file " << cur_bin 
					  << ": " << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
		
		//Reset the file flags and go to the beginning
		bin_in.clear();
		bin_in.seekg(0, std::ios::beg);
		
		//Reset bytes read and current file variables each loop
		current_file_bytes = 0;
		bytes_read_to_buffer = 0;
		
		//Copy binary_array sized chunks to the output file until there is
		//nothing left to copy
		do {
			//Read up to buffer_size bytes, and keep track of number of bytes
			bin_in.read(bin_array, _BINARY_ARRAY_SIZE);
			bytes_read_to_buffer = bin_in.gcount();
			
			//Write the data array to the output binary file
			bin_out.write(bin_array, bytes_read_to_buffer);
			
			//Add the bytes read this loop to the current file bytes
			current_file_bytes += static_cast<size_t>(bytes_read_to_buffer);
		} while (bytes_read_to_buffer);
		
		//Add this files bytes to the total
		total_output_bytes += current_file_bytes;
		//Report how many MiB was successfully copied on this file
		std::cout << BytesToPaddedMiBString(current_file_bytes, 6) << std::endl;
		//Close this file for the next loop
		bin_in.close();
	}
	
	
	//Clear the RAM Buffer and close the output file
	delete[] bin_array;
	bin_out.close();
	
	//Print sats messages - if the timed flag was set, print that info first
	if(timed == true) {
		end_millis = GetMillisecs();
		//Work out difference in millis from start to finish
		float delta_time = static_cast<float>((end_millis - beg_millis).count())
																	  / 1000.0f;
		
		//Use printf instead of cout for native float length settings
		printf("\nFinished in %.2f Seconds", delta_time);
	}
	
	std::cout << "\nSuccessfully dumped " 
			  << BytesToPaddedMiBString(total_output_bytes, 0)
			  << " to " << output_bin_path << std::endl;
}
