/* Te(xt) Fi(le) Ed(itor) TeFiEd 
* Simple Text File Editor, written be as fast and efficient as possible.
* -Beta version- 
* Some simple text editing features aren't built in quite yet, but I plan to
* impliment some simple search, replace, etc features.
*
* Please see the github page for this project: https://github.com/ADBeta/TeFiEd
* 
* (c) ADBeta 
*/

#ifndef TeFiEd_H
#define TeFiEd_H

#include <fstream>
#include <string>
#include <vector>

class TeFiEd {
	public:
	//Object constructor is used to pass the filename.
	TeFiEd(const char*);
	
	//Destructor cleans up RAM.
	~TeFiEd();
	
	/** Configuration Functions ***********************************************/
	//Sets the verbose flag.
	void setVerbose(bool verb) { this->verbose = verb; }
	
	//Sets the maximum -BYTES- the file can have until failsafe triggers.	
	void setByteLimit(size_t bytes) { this->MAX_RAM_BYTES = bytes; }
	
	//Sets the maximum number of -CHARS- a line can have before failsafe.
	void setStringLimit(size_t chars) { this->MAX_STRING_SIZE = chars; }
	
	/** File Metadata getters *************************************************/
	//Return the filename string (converted from const char* to string)
	std::string filename();
	
	//Return the filename string as a c string
	const char* filename_c_str();
	
	//Scan the vector and return the number of bytes used. (Can be slow)
	size_t bytes();
	
	//Return number of elements in the vector, which is 1:1 for lines of output
	size_t lines();
	
	/** Basic Functions *******************************************************/
	//Creates an empty file from the filename
	int create();
	
	//Reads input file into the RAM vector
	int read();
	
	//Returns if the file is open correctly. Preferably check return status of 
	//read(), but this is an okay second option.
	bool isOpen();
	
	//Return the line string at the passed index
	std::string getLine(size_t);
	
	//Overwrite the original file with the RAM file
	int overwrite();
	
	//Writes the RAM file out to another TeFiEd Object.
	int writeTo(TeFiEd &);
	
	//Deletes all the data in the RAM File, and shrinks the vector to save space
	void flush();
	
	/** File Editing Functions ************************************************/
	//Appends a string to the end of the RAM File
	int appendString(const std::string);
	
	//Append a string onto the end of a specific line
	int appendToLine(const std::string, size_t line);
	
	//Inserts a line of text into the vector at index.
	int insertString(const std::string, size_t index);
	
	//Inserts a string of text, line index and start pos index.
	int insertToLine(const std::string, size_t line, size_t pos);
	
	//Remove the specified line from RAM File.
	int removeLine(size_t line);
	
	//Find the first line containing a string. Return 0 when no match is found.
	size_t findFirst(std::string);
	
	//Find the next instance of a line containing string. Returns 0 when no 
	//match is found.
	size_t findNext(std::string);
	
	private:
	/** Configuration variables ***********************************************/
	//All of the configuration variables can be edited, using API calls.
	
	//Maximum number of bytes before the software will failsafe.
	size_t MAX_RAM_BYTES = 8388608; //8MB 	
	
	//Maximum length (in chars) of any one line before failsafe triggets.
	size_t MAX_STRING_SIZE = 5000;
	
	//If verbosity is enabled, the API will print updates to std::cout.
	//Error messages are enabled regardless. 
	bool verbose = false;
	
	/** File Variables ********************************************************/
	const char* m_filename; //Filename as char array
	std::fstream m_file; //fsteam object of file
	std::vector<std::string> m_ramfile; //File RAM vector	
	
	//Flag to see if the file is open successfully.
	bool isOpenFlag = false;
	
	/** Internal use functions ************************************************/
	//Reset flags/bits then close the file
	void resetAndClose();
	
	//Perform sanity checks on input string check if it will activate a failsafe
	int checkString(std::string inputStr);	
	
	//TODO redo this this
	/** Error Message Handling*************************************************/
	//Three message input
	template <typename T1, typename T2, typename T3>
	void errorMsg(std::string, T1, T2, T3);
	//Two message input
	template <typename T1, typename T2>
	void errorMsg(std::string, T1, T2);
	//One message input
	template <typename T1>
	void errorMsg(std::string, T1);
};

#endif
