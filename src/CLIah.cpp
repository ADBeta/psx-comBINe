/*******************************************************************************
* This file is part of CLIah, Command Line Interface argument handler.
* If this file is included as part of a seperate project, please see the github
* page for this project: https://github.com/ADBeta/CLIah
*
* This project is under the GPL3.0 licence. (c) 2023 ADBeta
*******************************************************************************/

#include <iostream>
#include <string>
#include <vector>

//NOTE: algorithm is used for strToUpper(), purely for practice with transform.
//If it is noted to be slow, or break some systems I will replace it with manual
//Index based looping on a string. Let me know.
#include <algorithm> 

#include "CLIah.hpp"

/*** Helper functions *********************************************************/
std::string strToUpper(std::string input) {
	//Transform seems to be the recommended method - causes another include 
	//and may cause other unknown issues. For practice, this will stay for now,
	//But may be replaced with a direct index (or iterator) in future.
	std::transform(input.begin(), input.end(), input.begin(), 
		//operation is getting the current char and performing toupper
		[](unsigned char chr){ return std::toupper(chr); } // correct
	);
	
	//Return the uppercase string
	return input;
}

/*** Error Handling ***********************************************************/
void CLIah::errorMsg(const unsigned int errLevel, const std::string errMsg) {
	std::string prefix;
	
	//Set prefix to Warning or Error depenging on errLevel
	if(errLevel == 0) { prefix = "Warning: ";
	} else { prefix = "Error: "; }
	
	std::cerr << prefix << errMsg << std::endl;
	
	//If the errLevel is > 1 then exit the program as a fatal error
	if(errLevel > 1) exit(EXIT_FAILURE);
}

/*** CLIah Functions **********************************************************/
namespace CLIah {
	namespace Config {
		//Error mode when certain circumatances are encountered
		ErrMode errorMode = ErrMode::exit;
		
		//Verbosity selection. Prints matched Args. Defaults to false
		bool verbose = false;
		
	} //namespace Config

/*** Non API Functions ********************************************************/
std::vector <Arg> argVector;

void printArg(const Arg &ref) {
	//Print the argReference, primary and alias args.
	std::cout << "Name            | " << ref.argReference << std::endl;
	std::cout << "Argument(s)     | " << ref.priMatchStr << "  " 
	          << ref.aliasMatchStr << std::endl;
	
	//Print case sensitivity
	std::string caseStr;
	if(ref.caseSensitive == false) { caseStr = "False";
	} else { caseStr = "True"; }
	
	std::cout << "Case Sensitive  | " << caseStr << std::endl;
	
	//Print the type
	std::string typeStr;
	if(ref.type == ArgType::flag) typeStr = "Flag";
	if(ref.type == ArgType::subcommand) typeStr = "Subcommand";
	if(ref.type == ArgType::variable) typeStr = "Variable";
	
	std::cout << "Type            | " << typeStr << std::endl;
	
	//If the type is Subcommand or Variable, print the substring (if detected)
	if(ref.type == ArgType::subcommand || ref.type == ArgType::variable) {
		std::cout << "Substring       | " << ref.substring << std::endl;
	}
	
	std::cout << std::endl;
}

bool argStringsMatch(const Arg &ref, std::string input) {
	//Copy the Arg match strings into strings we can modify
	std::string priStr = ref.priMatchStr;
	std::string aliasStr = ref.aliasMatchStr;
	
	//Case sensitivity check. If the Arg isn't case sensitive then convert
	//Pri Alias and input to uppercase to make any case usage irrelevant
	if(ref.caseSensitive == false) {
		//Convert input, priStr and aliasStr to uppercase
		input = strToUpper(input);
		priStr = strToUpper(priStr);
		aliasStr = strToUpper(aliasStr);
	}
	
	//If an exact match for either pri or alias occurs, return a match flag
	if(priStr.compare(input) == 0 || aliasStr.compare(input) == 0) {
		return true;
	}
		
	//If no match is found return false
	return false;
}

/*** API Functions ************************************************************/
void analyseArgs(int argc, char *argv[]) {
	//Regular argc and argv should have been passed. Adjust to remove argv[0]
	--argc;
	++argv;
	
	//Bool flag when a match happens. Used to catch errors
	bool matchFound;

	//Go through every element in the argv array
	for(int argStrIdx = 0; argStrIdx < argc; argStrIdx++) {
		//Convert curent argv char[] to string
		std::string inputArg = argv[argStrIdx];
		
		//Loop through all Args stored in argVector. Use iterator
		std::vector<Arg>::iterator itrtr; 
		for(itrtr = argVector.begin(); itrtr != argVector.end(); itrtr++) {

			 
			//Set matched flag to the return bool of argStringsMatch and check it 
			if( (matchFound = argStringsMatch(*itrtr, inputArg)) == true) {
			
				/*** Arg Type detection ***************************************/
				//Get the matched argument type to do specific execution.
				CLIah::ArgType tempType = itrtr->type;
				
				//If the Arg is a Flag Type, no special executuion takes place
				//Flag type --
				
				//If the Arg is Subcommand type, get the next inputArg string,
				//set the substring of the main object
				if(tempType == CLIah::ArgType::subcommand) {
					//Make sure there *IS* a next argument, if not fatal error.
					if(argStrIdx + 1 >= argc) {
						errorMsg(2, "analyseArgs: " + inputArg +
						          " is Subcommand type but has no substring");
					}
					
					//Incriment argStrIdx to get the substring, and the next
					//loop will go past it
					++argStrIdx;
					itrtr->substring = (std::string)argv[argStrIdx];
				}
				
				//TODO Variable type	
				
				/*** Constant execution when a match is found *****************/
				//Set the current argVector object detection flag to true
				itrtr->detected = true;
				
				//If verbosity is enabled, print the matched arg
				if(Config::verbose == true) {
					std::cout << "A match was found for \"" 
					          << inputArg << "\"" << std::endl;
					printArg(*itrtr);
				}
				
				//Break to prevent looping more Args looking for a match
				break;
			}//End of match found routine
			
		} //End of ArgVector loop
		
		
		/*** Error Handling ***************************************************/
		//If no match was found for inputArg after checking all argVector objs
		if(matchFound == false) {
			//If mode is set to ignore, skip this bit and just continue
			if(Config::errorMode != Config::ErrMode::ignore) {
				//Set up the error string
				std::string errStr = "No match for Arg \"" + inputArg + "\"";
				
				//Depending on ErrMode, warn or exit with errStr.
				if(Config::errorMode == Config::ErrMode::warn) {
					errorMsg(0, errStr);
				}
				if(Config::errorMode == Config::ErrMode::exit) {
					errorMsg(2, errStr);
				}
			}
		} //End of error handler 
		
	} //End of inputArg loop
}

void addNewArg(const std::string ref, const std::string pri, const ArgType type,
               const std::string alias = "", const bool caseSensitive = true) {
	//Create an Arg object to set the parameters to
	Arg newArg;
	
	//Set the new args variables
	newArg.argReference = ref;
	newArg.priMatchStr = pri;
	newArg.type = type;
	newArg.aliasMatchStr = alias;
	newArg.caseSensitive = caseSensitive;
	
	//Push the new argument to the argVector
	argVector.push_back(newArg);
}

Arg getArgByReference(const std::string refStr) {
	static Arg retArg;
	
	//Go through every argVector element and check for reference string
	std::vector<Arg>::iterator itrtr; 
	for(itrtr = argVector.begin(); itrtr != argVector.end(); itrtr++) {
		//Compare reference string and argReference. 
		if(refStr.compare( itrtr->argReference ) == 0) {
			//Set the return Arg object pointer, and return it
			retArg = *itrtr;
			return retArg;
		}
	}
	
	//If no match is found, fatal error as this could cause segfaults
	errorMsg(2, "getArgByReference: No Arg exists using reference " + refStr);
	
	//Return to avoid compile warning
	return retArg;
}

bool isDetected(const std::string refStr) {
	//Get the Arg by reference and assign it to an Arg object
	Arg tempArg = getArgByReference(refStr);
	
	return tempArg.detected;
}

std::string getSubstring(const std::string refStr) {
	//Get the Arg by reference and assign it to an Arg object
	Arg tempArg = getArgByReference(refStr);
	
	return tempArg.substring;
}

} //namespace CLIah
