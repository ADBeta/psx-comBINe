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
#include <iostream>
#include <string>
#include <vector>

#include "TeFiEd.h"

#ifndef CUE_HANDLER_H
#define CUE_HANDLER_H

/*** Enums and strings of enums ***********************************************/
//Valid CUE line types, including INVALID and EMPTY string types.
enum t_LINE { ltEMPTY, ltFILE, ltTRACK, ltINDEX, ltINVALID };

//Valid FILE formats. (only binary is supported for now)
enum t_FILE { ftUNKNOWN, ftBINARY};
//String of LINE types mapped to enum
extern const char* t_FILE_str[];

//Valid TRACK types  
/*	AUDIO		Audio/Music (2352 — 588 samples)
	CDG			Karaoke CD+G (2448)
	MODE1/2048	CD-ROM Mode 1 Data (cooked)
	MODE1/2352	CD-ROM Mode 1 Data (raw)
	MODE2/2336	CD-ROM XA Mode 2 Data (form mix)
	MODE2/2352	CD-ROM XA Mode 2 Data (raw)
	CDI/2336	CDI Mode 2 Data
	CDI/2352	CDI Mode 2 Data                                           */
enum t_TRACK {AUDIO, CDG, MODE1_2048, MODE1_2352, MODE2_2336, 
              MODE2_2352, CDI_2336, CDI_2352 };
extern const char* t_TRACK_str[];

/*** CueHandler Class *********************************************************/
class CueHandler {
	public:
	//Constructor takes a filename (char *) and passes it to the TeFiEd cueFile
	//Also creates the metadata handler structures
	CueHandler(const char* filename);
	
	//Destructor, deletes metadata array and cleans up TeFiEd object
	~CueHandler();
	
	/*** CUE file data handler ************************************************/
	//Child TRACK (2nd level) object. Max 99
	struct TrackData {
		//Track ID
		unsigned int ID;
	
		//Track Object type
		t_TRACK TYPE = AUDIO;
		
		//Does this track have a pregap?
		bool PREGAP = false;
		
		//INDEXs. Grandchild (3rd level) value. Maximum 99 INDEX bytes
		std::vector <unsigned long> INDEX_BYTE;
	}; //struct TrackData
	
	//Parent FILE (Top level) object
	struct FileData {
		//FILE data - Default values
		std::string FILENAME;
		t_FILE TYPE = ftUNKNOWN;
		
		//Vector of TRACKs for each FILE
		std::vector <TrackData> TRACK;
	}; //struct FileData
	
	//Vector of FILEs
	std::vector <FileData> FILE;
	
	/*** FILE Vector Functions ************************************************/
	//TODO clear function for any FileData struct object
	
	//Returns the t_LINE the line at -number-'s string contains.
	t_LINE getLineType(std::string lineStr);



	void testVect();
	
	
	void printFILE(FileData &);

	//Gets all the data from a .cue file and populates the FILE vector.
	//Returns error status TODO
	int getCueData();
	
	
	
	
	/*** **********************************************************************/
	//Read in an existing cue file
	void read();
	
	//Create a new cue file
	void create();
	
	
	
	/*** Reading Functions ****************************************************/	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//TODO NONE OF THIS IS CORRECT
	
	//
	bool lineIsValid(const size_t lineNo);
	
	//Pulls the filename string out of a valid .cue FILE command and returns it
	std::string getFILE(const std::string line);

	//Finds all lines with FILE in them, then pushed the filename on that line,
	//appended with the parent directory to a vector, (for dumping bin files)
	void pushFILEToVector(std::vector<std::string> &vect);





	/*** Writing Functions ****************************************************/
	//Output (overwrite) the TeFiEd object RAM into the file.
	void write();
	
	//Adds a FILE command entry to the cueFile. type can be BINARY or AUDIO etc
	void newFILE(const std::string fileName, const std::string type);
	
	//Adds a TRACK, Numbered as num, to the cueFile. Type should be AUDIO etc
	void newTRACK(const unsigned int num, const std::string type);
	
	//Adds a new INDEX, numbered as num, and the timestamp to the cueFile
	void newINDEX(const unsigned int num, const std::string indexStr);







	/** AUX Functions *********************************************************/
	//Converts a number of bytes into an Audio CD timestamp.
	std::string bytesToTimestamp(const unsigned long bytes);
	
	//Converts an Audio CD timestamp into number of bytes
	unsigned long timestampToBytes(std::string timestamp);
	
	//Adds two INDEX strings together and returns a timestamp string.
	std::string addTimestamps(const std::string ts1, const std::string ts2);
	
	
	
	
	
	
	
	/*** Private functions and variables **************************************/
	//private:
	/** Variables **/
	TeFiEd *cueFile; //TeFiEd text file object
	
	/** Functions **/
	//Takes an input uint32_t, zero-pads to -pad- then return a string
	std::string padIntStr(const unsigned long val, const unsigned int len = 0,
	                      const char pad = '0');

}; //class CueHandler
#endif
