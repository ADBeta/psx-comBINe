/*******************************************************************************
* clampp - Command Line Argument Manager, Plus Plus
* A Simple, Efficient and Fast CLI Argument Handler Library.
*
* See the GitHub For more information: https://github.com/ADBeta/clampp
* See example.cpp for programatic demonstation of usage/syntax.
*
* ADBeta(c)    24 Nov 2023    Version 1.0.0
*******************************************************************************/
#ifndef CLAMPP_H
#define CLAMPP_H

#include <vector>
#include <string>

/*** Configuration Variables **********************************************/
namespace ClamppConfig {
	//Are undefined arguments allowed. Defaults to false
	extern bool allow_undefined_args;
}


typedef enum {
	CLAMPP_ENONE = 0, CLAMPP_ENOARGS = -1, CLAMPP_ENOMATCH = -2, 
	CLAMPP_ENOSUBSTR = -3
} ClamppError;

class ClamppClass {
	public:
	/*** API Functions ********************************************************/
	//NOTE: Function overflow for both versions of a Defined Argument.
	//Returns an index of the argument in the list or -1 if an error occured
	//Add a Defined Argument with one one, primary flag string
	int AddDefinition(const char *pri, const bool has_substr);
	int AddDefinition(const char *pri, const char *sec, const bool has_substr);
	
	//Scan through the arguments using the standard argc/argv method
	//Returns a ClamppError value
	ClamppError ScanArgs(const int argc, const char *argv[]);
	
	//Get a DefinedArg's detected status. Either by index, or by flag_str
	//Returns 0 or 1 if detected or not, or -1 if an error occured
	int GetDetectedStatus(const int index);
	int GetDetectedStatus(const char *flag);
	
	//Get a DefinedArg's Substring, in std::string format, either by index or
	//flag_str. Returns an empty string on error
	std::string GetSubstring(const int index);
	std::string GetSubstring(const char *flag);
	
	//Get the Undefined Arg at index
	std::string GetUndefinedArg(const int index);
	
	private:
	/*** Argument Definition Struct *******************************************/
	typedef struct {
		const char *flag_pri = NULL;   //Primary flag string for argument (e.g -h)
		const char *flag_sec = NULL;   //(Optional) Secondary flag string (e.g --help)
		
		bool has_substr = false;       //Does argument have a substring (e.g -m hello)
		const char *substr = NULL;     //Argument string from the argv[] array
		
		bool was_detected = false;     //Was the argument detected during scan
	} ArgDef_t;

	//List of Defined Arguments 	
	std::vector<ArgDef_t> DefinedArgList;
	//List of Undefined Arguments. Not used if Config diables it
	std::vector<std::string> UndefinedArgList;

	/*** Private Functions ****************************************************/
	//Finds & returns the index of a DefinedArg by its flag_str.
	//Returns -1 if the flag_str could not match with any Defined Arg
	int FindDefinedByFlag(const char *flag);

};

#endif
