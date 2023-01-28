/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing, this is ideal for PSX/PS1 CD-ROMs
* e.g. Rayman to get them ready for cue2pops or some emulators. I also find it 
* improves reliabilty when buring to a disk to only have one .bin file.
*
* Cue File manager. Uses TeFiEd to handle output file cue file track, file and 
* index text
* 
* this might be turned into its own project if I see it useful in future.
*
* (c) ADBeta
*******************************************************************************/
#include "cueHandler.hpp"

#include <iostream>
#include <vector>

#include "helpers.hpp"
#include "TeFiEd.hpp"

/*** Enum mapped strings (only for printFILE) *********************************/
const std::string t_FILE_str[] = {"UNKNOWN", "BINARY"};   
const std::string t_TRACK_str[] = {
	"UNKNOWN", "AUDIO", "CDG", "MODE1/2048", "MODE1/2352", "MODE2/2336", 
	"MODE2/2352", "CDI/2336", "CDI/2352"
};

/*** CueHandler Functions *****************************************************/
CueHandler::CueHandler(const char* filename) {
	//Open the cue file passed
	cueFile = new TeFiEd(filename);
	
	//Set safety flag on the .cue input file. 100KB
	cueFile->setByteLimit(102400);
	
	cueFile->setVerbose(true);
}

CueHandler::~CueHandler() {
	std::cout << "destruct cuehandler" << std::endl;
	
	//Delete the TeFiEd object
	delete cueFile;
	
	//Clear the FILE vector and de-allocate its RAM
	FILE.clear();
	FILE.shrink_to_fit();
}

void CueHandler::read() {
	//Read the cueFile TeFiEd Obj into RAM
	if(cueFile->read() != 0) {
		exit(EXIT_FAILURE); 
	}
	
	//Make sure the Line Ending type is Unix
	cueFile->convertLineEnding(le_Unix);	
}

void CueHandler::create() {
	if(cueFile->create() != 0) {
		exit(EXIT_FAILURE);
	}
}


/*** FILE Vector Functions ****************************************************/
t_LINE CueHandler::getLINEType(const std::string lineStr) {	
	//If line is empty return EMPTY
	if(lineStr.length() == 0) return ltEMPTY;

	//Check against known strings for types
	if(lineStr.find("FILE \"") != std::string::npos) return ltFILE;
	
	if(lineStr.find("  TRACK") != std::string::npos) return ltTRACK;
	
	if(lineStr.find("    INDEX") != std::string::npos) return ltINDEX;
	
	//Failure to find any known string means it's an invalid line
	return ltINVALID;
}

//Returns the t_TRACK of the string passed
t_TRACK CueHandler::getTRACKType(const std::string trackStr) {
	//Get the TYPE string out of the line (last ' ' to the last char)
	unsigned int lastSpace = trackStr.find_last_of(' ') + 1;
	std::string typeStr = trackStr.substr(lastSpace, std::string::npos);
	
	//Get number of strings in t_TRACK_str array.
	unsigned int TRACKTypes = sizeof(t_TRACK_str)/sizeof(t_TRACK_str[0]);
	
	//Go through all elements in t_TRACK_str (current track string)
	for(unsigned int cts = 0; cts < TRACKTypes; cts++) {
		//If the string from line and the TRACKType string match, return that
		//index as enum variables
		if(typeStr.compare(t_TRACK_str[cts]) == 0) {
			//Return the matched type in enum
			return (t_TRACK)cts;
		}
	}
	
	//If nothing matches, return UNKNOWN
	return ttUNKNOWN;
}

void CueHandler::pushFILE(const std::string FN, const t_FILE TYPE) {
	//Temporary FILE object
	FileData tempFILE;
	//Set the FILE Parameters
	tempFILE.FILENAME = FN;
	tempFILE.TYPE = TYPE;
	
	//Push tempFILE to the FILE vect
	FILE.push_back(tempFILE);
}

void CueHandler::pushTRACK(const unsigned int ID, const t_TRACK TYPE) {
	if(ID > 99) {
		errorMsg(2, "pushTRACK: TRACK ID Greater than 99. Ignoring this push");
		return;
	}
	
	//Temporary TRACK object
	TrackData tempTRACK;
	//Set the TRACK Parameters
	tempTRACK.ID = ID;
	tempTRACK.TYPE = TYPE;
	
	//Get a pointer to the last entry in the FILE object
	FileData *pointerFILE = &FILE.back();
	//Push the tempTRACK to the back of the pointer 
	pointerFILE->TRACK.push_back(tempTRACK);
}

void CueHandler::pushINDEX(const unsigned int ID, const unsigned long BYTES) {
	if(ID > 99) {
		errorMsg(2, "pushINDEX: INDEX ID Greater than 99. Ignoring this push");
		return;
	}
	
	//Temporary INDEX object
	IndexData tempINDEX;
	//Set INDEX parameters
	tempINDEX.ID = ID;
	tempINDEX.BYTES = BYTES;
	
	//Get a pointer to the last FILE and TRACK Object
	TrackData *pointerTRACK = &FILE.back().TRACK.back();
	//Push the INDEX to the end of current file
	pointerTRACK->INDEX.push_back(tempINDEX);
}


void CueHandler::printFILE(FileData & pFILE) {
	//Check if pFILE is empty 
	if(pFILE.FILENAME == "") {
		errorMsg(1, "Selected FILE element is not valid.");
		return;
	}

	//Print filename and data
	std::cout << "FILENAME: " << pFILE.FILENAME;
	std::cout << "\t\tTYPE: " << t_FILE_str[(int)pFILE.TYPE] << std::endl;
	//Seperator
	std::cout << "-----------------------------------------------" << std::endl;

	//Print all TRACKs in vector held by FILE
	for(size_t tIdx = 0; tIdx < pFILE.TRACK.size(); tIdx++) {
		//Set TrackData object to point to current TRACK
		TrackData pTRACK = pFILE.TRACK[tIdx];
		
		//Print track number (TrackIndex + 1 to not have 00 track) and PREGAP:
		std::cout << "TRACK " << padIntStr(pTRACK.ID, 2);
		
		//Print TRACK TYPE variable
		std::cout << "        TYPE: " << t_TRACK_str[pTRACK.TYPE] << std::endl;
		
		//Print all INDEXs contained in that TRACK
		for(size_t iIdx  = 0; iIdx < pTRACK.INDEX.size(); iIdx++) {
			IndexData pINDEX = pFILE.TRACK[tIdx].INDEX[iIdx];
			
			//Print the index number
			std::cout << "  INDEX " << padIntStr(pINDEX.ID, 2)
			//Print the raw BYTES format 
			<< "    BYTES: " << padIntStr(pINDEX.BYTES, 9, ' ')
			//Print the timestamp version
			<< "    TIMESTAMP: " << bytesToTimestamp(pINDEX.BYTES) << std::endl;
		}
		
		//Print a blank line to split the TRACK fields
		std::cout << std::endl;
	}
}

int CueHandler::getCueData() {	
	//Go through all the lines in the cue file.
	for(size_t lineNo = 1; lineNo <= cueFile->lines(); lineNo++) {
		//Copy the current line to a new string
		std::string cLineStr = cueFile->getLine(lineNo);
		
		//Get the type of the current line and exec based on that
		t_LINE cLineType = getLINEType(cLineStr);
		
		/*** Line Type state execution ****************************************/
		//Invalid line cleans up then exits
		if(cLineType == ltINVALID) {
			errorMsg(2, "Cue file contains invalid line");
		}
		
		//FILE line type
		if(cLineType == ltFILE) {
			//TODO BINARY detection stuff
			//std::string fTypeStr = cLineStr.substr(
			
			//Make sure the FILE TYPE is known
			if(cLineStr.find("BINARY") == std::string::npos) {
				errorMsg(0, "File is not of type BINARY");
			}
			
			//Set the FILENAME
			size_t fQuote = cLineStr.find('\"') + 1;
			size_t lQuote = cLineStr.find('\"', fQuote);
			//push new FILE to the stack
			pushFILE(cLineStr.substr(fQuote, lQuote - fQuote), ftBINARY);
		}
		
		//TRACK line type
		if(cLineType == ltTRACK) {
			unsigned int lineID = std::stoi(cLineStr.substr(8, 2));
			t_TRACK lineTYPE = getTRACKType(cLineStr);
			pushTRACK(lineID, lineTYPE);
		}
		
		//INDEX line type	
		if(cLineType == ltINDEX) {
			unsigned int lineID = std::stoi(cLineStr.substr(10, 2));
			unsigned long lineBytes = timestampToBytes(cLineStr.substr(13, 8));
			pushINDEX(lineID, lineBytes);
		}
	}
	return 0;
}













/** Writing functions *********************************************************/
void CueHandler::write() {
	cueFile->overwrite();
}

void CueHandler::newFILE(const std::string fileName, const std::string type) {
	cueFile->appendString("FILE \"" + fileName + "\" " + type);
}

void CueHandler::newTRACK(const unsigned int num, const std::string type) {
	if(num == 99) {
		std::cerr << "Max TRACK number reached (99)" << std::endl;
		return;
	}
	
	cueFile->appendString("  TRACK " + padIntStr(num, 2) + " " + type);
}

void CueHandler::newINDEX(const unsigned int num, const std::string indexStr) {
	if(num == 99) {
		std::cerr << "Max INDEX number reached (99)" << std::endl;
		return;
	}
	
	//Otherwise, just append the string to the file as it is
	cueFile->appendString("    INDEX " + padIntStr(num, 2) + " " + indexStr);
}

/** AUX functions *************************************************************/
std::string CueHandler::bytesToTimestamp(const unsigned long bytes) {
	/* The timestamp is in Minute:Second:Frame format.
	There are 75 sectors per second, 2352 bytes per sector. If the input number 
	is not divisible by 1 sector, then exit the program.
	
	I am trying to use divide then modulo so the compiler stands some chance of
	optimizing my code to use the remainder of the ASM div operator. */
	
	std::string timestamp;	
	
	//There are 2352 byte per sector. Calculate how many sectors are in the file
	unsigned long sectors = bytes / 2352;
	
	//Error check if the input is divisible by a sector. Exit if not
	if(bytes % 2352 != 0) {
		errorMsg(2, "bin file is not divisible by SECTOR size (Corrupt bin)");
	}
	
	//75 sectors per second. Frames are the left over sectors from a second
	unsigned short seconds = sectors / 75;
	unsigned short rFrames = sectors % 75;
	
	//Convert seconds to minutes. Seconds is the remainder of itself after / 60
	unsigned short minutes = seconds / 60;
	seconds = seconds % 60;
	
	//If minutes exceeds 99, there is probably an error due to Audio CD Standard
	if(minutes > 99) errorMsg(1, "Total bin file size exceeds 99 minutes");
	
	//Now the string can be formed from the values. Need to 0 pad each value
	timestamp.append(padIntStr(minutes, 2) + ":" + padIntStr(seconds, 2) + ":"
	                           + padIntStr(rFrames, 2));
	
	return timestamp;
}

unsigned long CueHandler::timestampToBytes(std::string timestamp) {
	//Strip values from the timestamp. "MM:SS:ff" ff = sectors
	unsigned short minutes = std::stoi(timestamp.substr(0, 2));
	unsigned short seconds = std::stoi(timestamp.substr(3, 2));
	unsigned short frames  = std::stoi(timestamp.substr(6, 2));
	
	//Add minutes to the seconds for sector calculation
	seconds += (minutes * 60);
	
	//75 sectors per second, plus frames left over in the timestamp	
	unsigned long sectors = (seconds * 75) + frames;
	
	//There are 2352 bytes per sector.
	unsigned long bytes = sectors * 2352;
	
	//Error check if the input is divisible by a sector. Exit if not
	if(bytes % 2352 != 0) {
		errorMsg(2, "timestamp is not aligned to SECTOR size (Corrupt timestamp)");
	}
	
	return bytes;
} 

std::string CueHandler::addTimestamps(const std::string ts1, const std::string ts2) {
	//total bytes from both timestamps
	size_t totalBytes = timestampToBytes(ts1) + timestampToBytes(ts2);

	//Convert the bytes to a timestamp again and reutn it
	return bytesToTimestamp(totalBytes);
}





/** Private *******************************************************************/
std::string CueHandler::padIntStr(const unsigned long val, 
                                  const unsigned int len, const char pad) {
	std::string intStr = std::to_string(val);
	
	//Pad if selected
	if(len != 0 && len > intStr.length()) {
		unsigned int padDelta = len - intStr.length();
		intStr.insert(0, padDelta, pad);
	}
	
	return intStr;
}




