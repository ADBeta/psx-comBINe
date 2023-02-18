/*******************************************************************************
* This file is part of CLIah, Command Line Interface argument handler.
* If this file is included as part of a seperate project, please see the github
* page for this project: https://github.com/ADBeta/CLIah
*
* This project is under the GPL3.0 licence. (c) 2023 ADBeta
*
* Modified 17 Feb 2023
* V 1.0.0
*******************************************************************************/

#include <string>
#include <vector>

#ifndef H_CLIAH
#define H_CLIAH

namespace CLIah {

/*** Configuration sub namespace **********************************************/
	namespace Config {
		//Error method under certain circumstances, e.g ErrMode::Ignore
		enum class ErrMode { ignore, warn, exit };
		
		//How certain errors will be handled, defaults to exit
		extern ErrMode errorMode;
		
		//Verbosity selection. Prints matched Args. Defaults to false
		extern bool verbose;
		
	} //namespace Config

/*** CLI Arg types, structs and variables *************************************/
/* Arg types:
	* Flag is a simple flag like -h or --verbose. No substring is returned
		example --help / -h or -v / --verbose
	* Subcommand is a matched arg with a second string to return as a substring
		example --input hello-world would set substring to hello-world
	* Variable is a type that has an = or - or some other non-text char to get
	  the substring to set
	  	example --value=12 sets substring to 12
*/
enum class ArgType { flag, subcommand, variable }; 

/*** Error Handling ***********************************************************/
//Pass a error level flag and an error string.
//0 - Warning    1 - Error    2 - Fatal Error (exits program)
void errorMsg(const unsigned int errLevel, const std::string errMsg);

/*** Argument Structure *******************************************************/
//CLI Arg structure
struct Arg {
	//Argument reference name (used to index through all arg objects)
	std::string argReference;

	//Main and secondary (alias) match strings
	std::string priMatchStr = "", aliasMatchStr = "";
	//Is the match string(s) case sensitive
	bool caseSensitive = true;
	
	//What type of argument is this (default to flag)
	ArgType type = ArgType::flag;
	
	//Function pointer. Executed automatically if match is found TODO
	
	/*** Detection variables. Not for user modification ***/
	//Return string from sub-command or variable type
	std::string substring; 
	//Flag set if the argument has been detected, and validated.
	bool detected = false;
	
}; //struct Arg

/*** Internal functions not intedned for API use ******************************/
//Prints an Argument structs data to standard out
void printArg(const Arg &);
//returns if the Argument pri/alias strings match input (handles case sensitive)
bool argStringsMatch(const Arg &, std::string input);

//Vector array of Args, stores user defined Args & variables therin
extern std::vector <Arg> argVector;

/*** CLIah API functions ******************************************************/
//Pushes a new argument to the argVecor. Some variables are mandatory:
// argReference		- String by which the argument is known. eg verbose
// priMatchStr		- String to match for a detection (alias is optional)
// argType			- The type of the argument, see ArgType for info
void addNewArg(const std::string argReference, const std::string priMatchStr,
               const ArgType type, const std::string aliasMatchStr, 
               const bool caseSensitive);

//Skims all arguments passed via cli and sets internal struct vars.
//Must be called before any other functions can be.
void analyseArgs(int argc, char *argv[]);

//Finds and returns Arg struct with matching argReference string.
Arg getArgByReference(const std::string reference);

//Finds and returns the detected flag of an Arg by argReference
bool isDetected(const std::string reference);

//Returns the referenced Args substring
std::string getSubstring(const std::string reference);

}; //namespace CLIah

#endif
