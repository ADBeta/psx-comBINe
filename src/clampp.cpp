/*******************************************************************************
* clampp - Command Line Argument Manager, Plus Plus
* A Simple, Efficient and Fast CLI Argument Handler Library.
*
* See the GitHub For more information: https://github.com/ADBeta/clampp
* See example.cpp for programatic demonstation of usage/syntax.
*
* ADBeta(c)
*******************************************************************************/
#include "clampp.hpp"

#include <iterator>
#include <cstring>
#include <string>

/*** Configuration ************************************************************/
bool ClamppConfig::allow_undefined_args = false;


/*** Private Functions ********************************************************/
int ClamppClass::FindDefinedByFlag(const char *flag) {
	//Go through each Defined Argument looking for a match
	for(auto def = this->DefinedArgList.begin(); def != this->DefinedArgList.end(); def++) {
		//See if the primary or secondary flag strings match
		int pri_cmp = -1, sec_cmp = -1;
		pri_cmp = strcmp(def->flag_pri, flag);
		if(def->flag_sec != NULL) sec_cmp = strcmp(def->flag_sec, flag);
		
		//If either flag string matched, return the distance from begin() to now
		if(pri_cmp == 0 || sec_cmp == 0) {
			return (int)std::distance(DefinedArgList.begin(), def);
		}
	}

	//No match
	return -1;
}

/*** API Functions ************************************************************/
//Add a Defined Argument with a single, primary flag string
int ClamppClass::AddDefinition(const char *pri, const bool has_substr) {
	
	ArgDef_t arg_temp;
	arg_temp.flag_pri = pri;
	arg_temp.flag_sec = NULL;
	arg_temp.has_substr = has_substr;

	try {
		DefinedArgList.push_back(arg_temp);
	} catch(const std::bad_alloc &) {
		return -1;
	}
	
	return (int)std::distance(DefinedArgList.begin(), DefinedArgList.end()) - 1;
}	
	
//Add a Defined Argument with two flag strings, one primary + one alias
int ClamppClass::AddDefinition(const char *pri, const char *sec, const bool has_substr) {

	ArgDef_t arg_temp;
	arg_temp.flag_pri = pri;
	arg_temp.flag_sec = sec;
	arg_temp.has_substr = has_substr;

	try {
		DefinedArgList.push_back(arg_temp);
	} catch(const std::bad_alloc &) {
		return -1;
	}
	
	return (int)std::distance(DefinedArgList.begin(), DefinedArgList.end()) - 1;
}

ClamppError ClamppClass::ScanArgs(const int argc, const char *argv[]) {
	if(argc <= 0) return CLAMPP_ENOARGS;
	
	//Go through all of the input argument strings
	for(int crnt_arg = 0; crnt_arg < argc; crnt_arg++) {
		const char *arg_str = argv[crnt_arg];
		
		int index = FindDefinedByFlag(arg_str);
		
		//If the target was found, set its detected flag, and get a substring if
		//It was set. Return if no substring was given but was required
		if(index >= 0) {
			ArgDef_t *found_arg = &this->DefinedArgList[(size_t)index];
			
			found_arg->was_detected = true;
			
			if(found_arg->has_substr == true) {
				if(++crnt_arg == argc) return CLAMPP_ENOSUBSTR;
				found_arg->substr = argv[crnt_arg];				
			}
		} 
		else {
			//If undefined strings are not allowed, return an error.
			if(ClamppConfig::allow_undefined_args == false) return CLAMPP_ENOMATCH;
			//If they are allowed, push the string to the end of the list.
			this->UndefinedArgList.push_back(std::string(arg_str));
		}		
	}
	
	//If everything went well, exit with no error
	return CLAMPP_ENONE;
}

int ClamppClass::GetDetectedStatus(const int index) {
	if(index < 0) return -1;
	if((size_t)index >= this->DefinedArgList.size()) return -2;
	
	return DefinedArgList[(size_t)index].was_detected;
}

int ClamppClass::GetDetectedStatus(const char* flag) {
	int ret;
	if((ret = FindDefinedByFlag(flag)) < 0) return -1;
	
	//Get index's detected var
	return this->GetDetectedStatus(ret);
}

std::string ClamppClass::GetSubstring(const int index) {
	//Return empty string is there's an error
	if(index < 0 || (size_t)index >= this->DefinedArgList.size()) return std::string();
	
	const char *substr = this->DefinedArgList[ (size_t)index ].substr;
	//Guard against NULL char strings.
	if(substr == NULL) return std::string();
	return std::string(substr);
}

std::string ClamppClass::GetSubstring(const char *flag) {
	int ret;
	if((ret = FindDefinedByFlag(flag)) < 0) return std::string();
	
	//Get index's detected var
	return this->GetSubstring(ret);
}

std::string ClamppClass::GetUndefinedArg(const int index) {
	//Return empty string is there's an error
	if(index < 0 || (size_t)index >= this->UndefinedArgList.size()) return std::string();
	
	return UndefinedArgList[ (size_t)index ];
}
