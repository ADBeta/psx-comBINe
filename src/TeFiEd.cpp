/* Te(xt) Fi(le) Ed(itor) TeFiEd 
* Simple Text File Editor, written be as fast and efficient as possible.
* -Beta version- 
* Some simple text editing features aren't built in quite yet, but I plan to
* impliment some simple search, replace, etc features.
*
* Please see the github page for this project: https://github.com/ADBeta/TeFiEd
* 
* (c) ADBeta 
* v3.0.0
* Last Modified 26 Nov 2022
*/

#include "TeFiEd.h"

#include <fstream>
#include <iostream>
#include <string>

TeFiEd::TeFiEd(const char* ip_filename) {
	m_filename = ip_filename;
}

/** File Metadata getters *****************************************************/
std::string TeFiEd::filename() {
	//Convert the const char to a string
	return (std::string)m_filename;
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
//Read file into RAM File. Includes byte and line length failsafe
int TeFiEd::read() {
	//Flush the vector
	this->flush();

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
	
	//Success
	return 0;
}

std::string TeFiEd::getLine(size_t index) {
	//Decriment index if above 0, RAM File is indexed +1 from 'normal' notation
	if(index > 0) {
		--index;
	}
	
	//TODO Segfaults if file isn't open. add failsafe
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
//Append string to the end of the RAM File
int TeFiEd::appendString(const std::string inStr) {
	//Sanity check string and RAM size
	if(checkString(inStr) != 0) {
		return 1;
	}
	
	//push entry to back of the vector
	m_ramfile.push_back(inStr);
	
	//Complete
	return 0;
}

int TeFiEd::insertString(const std::string inStr, size_t index) {
	//Decriment index if above 0, RAM File is indexed +1 from 'normal' notation
	if(index > 0) {
		--index;
	}

	//Make sure that the vector has enough elements to allow the insert
	if(index > m_ramfile.size()) {
		//Error message and return fail
		errorMsg("insertLine", "Line", index + 1, "does not exist");
		
		return 1;
	}
	
	//Sanity check string and RAM size
	if(checkString(inStr) != 0) {
		return 1;
	}
	
	m_ramfile.insert(m_ramfile.begin() + index, inStr);
	return 0;
}

//Append a string onto the end of a specific line
int TeFiEd::appendToLine(const std::string inStr, size_t index) {
	//Decriment index if above 0, RAM File is indexed +1 from 'normal' notation
	if(index > 0) {
		--index;
	}
	
	//Combine lengths of both input and pre-existing string for length check
	std::string catString = m_ramfile[index] + inStr;
	//Sanity check string and RAM size
	if(checkString(catString) != 0) {
		return 1;
	}
	
	//append the string in the vector at index given
	m_ramfile[index].append(inStr);
	
	//Done
	return 0;
}

//Insert a string to a sepcific line, starting at a specific position index.
int TeFiEd::insertToLine(const std::string inStr, size_t index, size_t pos) {
	//Decriment index if above 0, RAM File is indexed +1 from 'normal' notation
	if(index > 0) --index;
	
	//Decriment pos if above 0
	if(pos > 0) --pos;
	
	//Make sure that the vector has enough elements to allow the insert
	if(index > m_ramfile.size()) {
		errorMsg("insertString", "Line", index + 1, "does not exist");
		
		return 1;
	}
	
	//Make sure that pos doesn't go past the string in vector[index]
	if(pos > m_ramfile[index].size()) {
		errorMsg("insertString", "cannot insert to line", index + 1,
			"at position " + std::to_string(pos + 1));
		
		return 1;
	}
	
	//Combine lengths of both input and pre-existing string for length check
	std::string catString = m_ramfile[index] + inStr;
	//Sanity check string and RAM size
	if(checkString(catString) != 0) {
		return 1;
	}
	
	//If nothing goes wrong, append to the string
	m_ramfile[index].insert(pos, inStr);
	return 0;
}

int TeFiEd::removeLine(size_t index) {
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

/** Internal only functions ***************************************************/
//Checks the validity of a passed string, and if it will exceed the failsafes
int TeFiEd::checkString(std::string testString) {
	
	//Check the number of chars in the input doesn't exceed MAX_STRING_SIZE
	size_t stringSize = testString.size();
	
	if(stringSize > MAX_STRING_SIZE) {
		errorMsg("Input string exceeds MAX_STRING_SIZE :",
			MAX_STRING_SIZE);
		
		return -1;
	}
	
	//Check if adding the input string to RAM File will cause a RAM failsafe.
	//NOTE +1 for newline, because this is a line operation
	if((this->bytes() + stringSize + 1) > MAX_RAM_BYTES) {
		errorMsg("String addition causes file to exceed MAX_RAM_BYTES :",
			MAX_RAM_BYTES);
		
		return -2;
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
