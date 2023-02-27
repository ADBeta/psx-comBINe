/******************************************************************************* 
* Te(xt) Fi(le) Ed(itor) TeFiEd 
* Simple Text File Editor, written be as fast and efficient as possible.
* -Beta version- 
* Some simple text editing features aren't built in quite yet, but I plan to
* impliment some simple search, replace, etc features.
*
* Please see the github page for this project: https://github.com/ADBeta/TeFiEd
* 
* (c) ADBeta 
*******************************************************************************/

#include "TeFiEd.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

TeFiEd::TeFiEd(const char* filename) {
	//Create a char array at m_filename the size of the input string.
	m_filename = new char[ strlen(filename) + 1 ];
	
	//Copy the input string to the new char array at m_filename
	strcpy(m_filename, filename);
}

TeFiEd::TeFiEd(const std::string filename) {
	//Create a char array at m_filename the size of the input string.
	m_filename = new char[ filename.size() + 1 ];

	//Copy the input string to the new char array at m_filename
	strcpy(m_filename, filename.c_str());
} 

//Destructor cleans up the vector and oher RAM garbage disposal.
TeFiEd::~TeFiEd() {	
	delete[] m_filename;
	
	if(m_ramfile.size() != 0) {
		this->flush();
	}
}

/** File Metadata getters *****************************************************/
std::string TeFiEd::filename() {
	//Convert the const char to a string
	return (std::string)m_filename;
}

const char *TeFiEd::filename_c_str() {
	return m_filename;
}

std::string TeFiEd::parentDir() {
	std::string fn = this->filename();
	//TODO Windows may not work with this methodology
	
	//If there is no / in the filename, assume the path to be ./
	if(fn.find('/') == std::string::npos) return "./";
	
	//Otherwise return a substring of the filename string, from 0 to the last /
	return fn.substr(0, fn.find_last_of('/') + 1);
}

size_t TeFiEd::bytes() {
	//Return number of bytes in the file
	size_t byteCount = 0;
	
	//Go through every element in the vector
	size_t vectElement = 0;
	while(vectElement < m_ramfile.size()) {
		//For each element, add the string size to the count.
		//NOTE <string>.size() returns number of bytes. it does not natively
		//understand unicode or multi-byte character sets, so this should
		//be a reliable method of getting bytes (as of 2022)
		byteCount += m_ramfile[vectElement].size();
	
		//NOTE Strings do not know about their terminating \n, so add 
		//1 byte to the count after every loop
		//This has been tested to agree with both Thunar and Nautilus 
		//file manager. may need rework later in time
		++byteCount;
	
		++vectElement;
	}
	
	return byteCount;
}

size_t TeFiEd::lines() {
	return m_ramfile.size();
}

/** Basic Functions ***********************************************************/
//Create empty file from filename
int TeFiEd::create() {
	//Open file as output, truncate
	this->m_file.open(m_filename, std::ios::out | std::ios::trunc);
	
	//Make sure file is open and exists
	if(m_file.is_open() == 0) { 
		errorMsg("create", "Could not create file");
		return 1;
	}
		
	//Close file and clear flags
	resetAndClose();
	
	//If verbosity is enabled, print a nice message
	if(this->verbose == true) {
		std::cout << "Create " << m_filename << " Successful" << std::endl;
	}
	
	return 0;
}

//Read file into RAM File. Includes byte and line length failsafe
int TeFiEd::read() {
	//Flush the vector
	flush();

	//Open file as read
	m_file.open(m_filename, std::ios::in);
	
	//Make sure file is open and exists
	if(m_file.is_open() == 0) {
		errorMsg("read", "File does not exist");
		return 1;
	}

	//String containing current line of text
	std::string lineStr;
	
	size_t byteCount = 0;
	//Get the next line in the stream, unless next line is EOF
	while(this->m_file.peek() != EOF) {
		//Copy current line string to var
		std::getline(this->m_file, lineStr);
		
		//Byte count, same methodology as getRAMBytes.
		//Add bytes to RAM byte counter
		byteCount += lineStr.size();
		//Add newline char per loop
		++byteCount;
		
		//Check that the next push won't overflow the size of the file
		if(byteCount > MAX_RAM_BYTES) {
			//Error message
			errorMsg("read", "File exceeds MAX_RAM_BYTES :", MAX_RAM_BYTES);
			
			//Return error status
			return 1;
		}
		
		//if no failure, push string into vector
		this->m_ramfile.push_back(lineStr);
	}
	
	//Close the file. Saves IO space and isn't needd for now
	resetAndClose();
	
	//If verbosity is enabled, print a nice message
	if(this->verbose == true) {
		std::cout << "Read " << m_filename << " Successful: " << byteCount 
		  << " bytes, " << this->lines() << " lines." << std::endl;
	}
	
	isOpenFlag = true;
	//Success
	return 0;
}

bool TeFiEd::isOpen() {
	isOpenFlag = m_file.is_open();	
	return isOpenFlag;
}

std::string TeFiEd::getLine(size_t index) {
	//If the index is 0, return a blank string
	if(index == 0) return "";
	
	//Always decriment index to fit the 1 index style
	--index;
	
	if(index > this->m_ramfile.size() - 1) {
		errorMsg("getLine", "Line", index + 1, "does not exist");
		return "";
	}
	
	//If everything is normal
	return this->m_ramfile[index];
}

int TeFiEd::overwrite() {
	//Open file as output, truncate
	this->m_file.open(m_filename, std::ios::out | std::ios::trunc);
	
	//Make sure file is open and exists
	if(m_file.is_open() == 0) { 
		errorMsg("overwrite", "Could not create file");
		
		return 1;
	}
	
	//Write parent object ram to file
	for(std::string lineStr : this->m_ramfile) {
		m_file << lineStr << std::endl;
	}
	
	//Close file and clear flags
	resetAndClose();
	
	//If verbosity is enabled, print a nice message
	if(this->verbose == true) {
		std::cout << "Overwrite " << m_filename << " Successful: wrote "
		  << this->bytes() << " bytes, " << this->lines() << " lines." 
		  << std::endl;
	}
	
	return 0;
}

//Write RAM into another TeFiEd Object
int TeFiEd::writeTo(TeFiEd &target) {
	//Open file as output, truncate
	target.m_file.open(target.m_filename, std::ios::out | std::ios::trunc);
	
	//Make sure file is open and exists
	if(target.m_file.is_open() == 0) { 
		errorMsg("writeTo", "Could not create file ", target.m_filename);
		
		return 1;
	}
	
	//Write parent ram to reference file
	for(std::string lineStr : this->m_ramfile) {
		target.m_file << lineStr << std::endl;
	}
	
	//Close file and clear flags
	target.resetAndClose();
	
	//If verbosity is enabled, print a nice message
	if(this->verbose == true) {
		std::cout << "Write to " << target.m_filename << " Successful: wrote "
			<< this->bytes() << " bytes, " << this->lines() << " lines."
			<< std::endl;
	}
	
	//Success
	return 0;
}

//Delete all RAM File data
void TeFiEd::flush() {
	//Empties out the vector and shrinks its size
	m_ramfile.clear();
	m_ramfile.shrink_to_fit();
}

/** File Edit Functions *******************************************************/
void TeFiEd::convertLineEnding(const LineEnding type) {
	//Keep track of how many lines were not the correct type for verbose prints
	size_t wrongLines = 0;
	
	//Go through every line in the RAM file
	size_t cLine = 0;
	while(cLine < m_ramfile.size()) {
		//Get the last char in this line
		char lastChar = m_ramfile[cLine].back();
		
		//In Unix convert mode, remove any 0x0D (\r or CR) chars
		if(type == LineEnding::Unix) {
			if(lastChar == 0x0D) {
				//Remove the last char
				m_ramfile[cLine].pop_back();		
				//Incriment wrongLines
				++wrongLines;
			}
		//In DOS mode, if a line does not have 0x0D (\r CR) then add one,
		} else if (type == LineEnding::DOS) {
			if(lastChar != 0x0D) {
				//Add the \r to the end of line
				m_ramfile[cLine].push_back(0x0D);
				//Incriment wrongLines
				++wrongLines;
			}
		}
		
		++cLine;
	}
	
	//Output verbose message about status and results
	if(this->verbose == true) {
		if(type == LineEnding::Unix) {
			std::cout << "Unix ";
		} else if(type == LineEnding::DOS) {
			std::cout << "DOS ";
		}
	
		std::cout << "Line Ending Conversion done. " << wrongLines 
		          << " lines changed. " << std::endl;
	}
}

//Append string to the end of the RAM File
int TeFiEd::append(const std::string inStr) {
	//Sanity check string and RAM size
	if(checkString(inStr) != 0) {
		return 1;
	}
	
	//push entry to back of the vector
	m_ramfile.push_back(inStr);
	
	//Complete
	return 0;
}

int TeFiEd::insertLine(size_t line, const std::string inStr) {
	//Decriment line if above 0, RAM File is indexed +1 from 'normal' notation
	if(line > 0) {
		--line;
	}

	//Make sure that the vector has enough elements to allow the insert
	if(line > m_ramfile.size()) {
		//Error message and return fail
		errorMsg("insertLine", "Line", line + 1, "does not exist");
		
		return 1;
	}
	
	//Sanity check string and RAM size
	if(checkString(inStr) != 0) {
		return 1;
	}
	
	m_ramfile.insert(m_ramfile.begin() + line, inStr);
	return 0;
}

//Append a string onto the end of a specific line
int TeFiEd::appendLine(size_t line, const std::string inStr) {
	//Decriment line if above 0, RAM File is indexed +1 from 'normal' notation
	if(line > 0) {
		--line;
	}
	
	//Combine lengths of both input and pre-existing string for length check
	std::string catString = m_ramfile[line] + inStr;
	//Sanity check string and RAM size
	if(checkString(catString) != 0) {
		return 1;
	}
	
	//append the string in the vector at index given
	m_ramfile[line].append(inStr);
	
	//Done
	return 0;
}

int TeFiEd::replace(size_t line, std::string inStr) {
	//Decriment line if above 0, RAM File is indexed +1 from 'normal' notation
	if(line > 0) {
		--line;
	}
	
	//Make sure that the line requested is valid
	if(line > m_ramfile.size()) {
		//Error message and return fail
		errorMsg("replsce", "Line", line + 1, "does not exist");
		
		return 1;
	}	
	
	//Sanity check string and RAM size
	if(checkString(inStr) != 0) {
		return 1;
	}
	
	//Change the RAM vectors string to inStr
	m_ramfile[line] = inStr;
	
	//Done
	return 0;
}

int TeFiEd::remove(size_t index) {
	//Decriment index if above 0, RAM File is indexed +1 from 'normal' notation
	if(index > 0) {
		--index;
	}
	
	//Make sure that the vector has the correct number of elements
	if(index > m_ramfile.size()) {
		//Error message and return error value
		errorMsg("removeLine", "Line", index + 1, "does not exist");
			
		return 1;
	}
		
	//Erase line specified
	m_ramfile.erase(m_ramfile.begin() + index);
	//Shrink the vector
	m_ramfile.shrink_to_fit();
	
	//Return success
	return 0;
}

std::string TeFiEd::getWord(const size_t line, unsigned int index) {
	//Get line string 
	std::string input = getLine(line);
	
	//Send input to getWord string version and retrun it
	return getWord(input, index);
}

//Overloaded version of getWord (string and index)
std::string TeFiEd::getWord(const std::string input, unsigned int index) {
	//If index is 0, set it to 1. always 1 indexed
	if(index == 0) index = 1;
	
	//Create output string object
	std::string output;

	//Set the delim string -- Regular delims, and Tab, Carriage Return (Windows)
	const std::string delim = " .,;\t\r";
	
	//Start and end of word, and current word found.
	size_t wordStart = 0, wordEnd = 0, wordIndex = 0;
	
	//Find the start and end of a word
	do {
		//Get the start and end of a word, detected by delim
		wordStart = input.find_first_not_of(delim, wordEnd);	
		wordEnd = input.find_first_of(delim, wordStart);
		
		//wordEnd can be allowed to overflow, but wordStart cannot.
		if(wordStart != std::string::npos) {
			//Incriment word index.
			++wordIndex;
			
			//If this index is the one requested, set output
			if(wordIndex == index) {
				output = input.substr(wordStart, wordEnd - wordStart);
				break;
			}
		}
	
	} while(wordStart < input.size());	
	
	//If the index could not be found, return an empty string
	if(wordIndex < index) {
		output = "";
	}
	
	return output;
}

size_t TeFiEd::find(std::string search, size_t offset) {
	//Force offset to be 1 if 0 is passed
	if(offset < 1) offset = 1;
	
	size_t lineCount = lines() + 1; //Get how many lines there are in the vector
	
	//Search through each of them until we match the search string
	for(size_t cLine = offset; cLine < lineCount; cLine++) {
		std::string lineStr = getLine(cLine);
		
		//When current line contains the search string
		if(lineStr.find(search) != std::string::npos) {
			return cLine;//Return the current line number
		}
	}
	
	//If no line matches, then return 0.
	return 0;
}

size_t TeFiEd::findFirst(std::string search) {
	size_t lineCount = lines() + 1; //Get how many lines there are in the vector
	
	//Search through each of them until we match the search string
	for(size_t cLine = 1; cLine < lineCount; cLine++) {
		std::string lineStr = getLine(cLine);
		
		//When current line contains the search string
		if(lineStr.find(search) != std::string::npos) {
			return cLine;//Return the current line number
		}
	}
	
	//If no line matches, then return 0.
	return 0;
}

size_t TeFiEd::findNext(std::string search) {
	/*** Setup ***/
	//Last seach string, when new search string is given, reset to beginning
	static std::string lastSearch;
	//Current line is retained per call. (Init to 1 to prevent random val)
	static size_t cLine = 0;
	
	//If the current and last search don't match, reset the cLine and lastSearch
	if(search != lastSearch) {
		cLine = 1;
		lastSearch = search;
	}
	
	/*** Runtime ***/
	//Vector line where search is found. Set to 0 to assume no match.
	size_t matchLine = 0;
	
	size_t lineCount = lines() + 1; //Get how many lines there are in the vector
	while(cLine < lineCount) {
		//Get current line string
		std::string lineStr = getLine(cLine);
		
		//When current line contains the search string
		if(lineStr.find(search) != std::string::npos) {
			//Match line is this line
			matchLine = cLine;
			//Inc past this line for the next loop
			++cLine;
			//Break the loop to go to the return section
			break;
		}
		
		//Inc to next line if no match on this line.
		++cLine;
	}
	
	//Return matchLine. 0 if no match, cLine if matched.
	return matchLine;
}

/** Internal only functions ***************************************************/
//Checks the validity of a passed string, and if it will exceed the failsafes
int TeFiEd::checkString(std::string testString) {
	//Check the number of chars in the input doesn't exceed MAX_STRING_SIZE
	size_t stringSize = testString.size();
	
	if(stringSize > MAX_STRING_SIZE) {
		errorMsg("Input string exceeds MAX_STRING_SIZE :",
			MAX_STRING_SIZE);
		
		return 1;
	}
	
	//Check if adding the input string to RAM File will cause a RAM failsafe.
	//NOTE +1 for newline, because this is a line operation
	if((this->bytes() + stringSize + 1) > MAX_RAM_BYTES) {
		errorMsg("String addition causes file to exceed MAX_RAM_BYTES :",
			MAX_RAM_BYTES);
		
		return 2;
	}
	
	//Otherwise exit with pass
	return 0;
}

void TeFiEd::resetAndClose() {
	//Private function. resets bit flags and closes the file
	//Clar flags
	m_file.clear();
	//Seek to 0th byte
	m_file.seekg(0, std::ios::beg);
	//Close file
	m_file.close();
	
	isOpenFlag = false;
}

/** Error Message *************************************************************/
//Three Argument version of errorMsg
template <typename T1, typename T2, typename T3>
void TeFiEd::errorMsg(std::string func, T1 msg1, T2 msg2, T3 msg3) {
	//Print generic error message explaining where and what caused the error
	std::cerr << "Error: " << this->m_filename << ": " << func << ": ";
	
	//Print each message with period and newline
	std::cerr << msg1 << " " << msg2 << " " << msg3 << '.' << std::endl;
}

//Two argument version of errorMsg
template <typename T1, typename T2>
void TeFiEd::errorMsg(std::string func, T1 msg1, T2 msg2) {
	//Print generic error message explaining where and what caused the error
	std::cerr << "Error: " << this->m_filename << ": " << func << ": ";
	
	//Print each message with period and newline
	std::cerr << msg1 << " " << msg2 << '.' <<std::endl;
}

//Single argument version of errorMsg
template <typename T1>
void TeFiEd::errorMsg(std::string func, T1 msg1) {
	//Print generic error message explaining where and what caused the error
	std::cerr << "Error: " << this->m_filename << ": " << func << ": ";
	
	//Print each message with period and newline
	std::cerr << msg1 << '.' <<std::endl;
}
