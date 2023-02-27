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
* (c) ADBeta
*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>

#include "TeFiEd.hpp"

#ifndef CUE_HANDLER_H
#define CUE_HANDLER_H

/*** Enums and strings of enums ***********************************************/
/* Using plain enums to allow them to convert to int */
//Valid CUE line types, including INVALID, REM and EMPTY string types.
enum t_LINE { ltEMPTY, ltFILE, ltTRACK, ltINDEX, ltREM, ltINVALID };

//Valid FILE formats. (only binary is supported for now)
enum t_FILE { ftUNKNOWN, ftBINARY};
//String of LINE types mapped to enum
extern const std::string t_FILE_str[];

//Valid TRACK types  
/*	AUDIO		Audio/Music (2352 — 588 samples)
	CDG			Karaoke CD+G (2448)
	MODE1/2048	CD-ROM Mode 1 Data (cooked)
	MODE1/2352	CD-ROM Mode 1 Data (raw)
	MODE2/2336	CD-ROM XA Mode 2 Data (form mix)
	MODE2/2352	CD-ROM XA Mode 2 Data (raw)
	CDI/2336	CDI Mode 2 Data
	CDI/2352	CDI Mode 2 Data                                           */
enum t_TRACK {ttUNKNOWN, ttAUDIO, ttCDG, ttMODE1_2048, ttMODE1_2352,
              ttMODE2_2336, ttMODE2_2352, ttCDI_2336, ttCDI_2352 };
extern const std::string t_TRACK_str[];

/*** CueHandler Class *********************************************************/
class CueHandler {
	public:
	//Constructor takes a filename (char *) and passes it to the TeFiEd cueFile
	//Also creates the metadata handler structures
	CueHandler(const std::string filename);
	
	//Destructor, deletes metadata array and cleans up TeFiEd object
	~CueHandler();
	
	TeFiEd *cueFile; //TeFiEd text file object
	
	/*** Cue file data structs ************************************************/
	//INDEXs. Grandchild (3rd level) value. Max 99
	struct IndexData {
		unsigned int ID = 0;		
		//Number of bytes the current index holds
		unsigned long BYTES = 0;
	};
	
	//Child TRACK (2nd level) object. Max 99
	struct TrackData {
		unsigned int ID = 0;
		t_TRACK TYPE = ttUNKNOWN;
		//Vector of INDEXs
		std::vector <IndexData> INDEX;
	};
	
	//Parent FILE (Top level) object
	struct FileData {
		//FILE data - Default values
		std::string FILENAME;
		t_FILE TYPE = ftUNKNOWN;
		//Vector of TRACKs for each FILE
		std::vector <TrackData> TRACK;
	};
	
	//Vector of FILEs (Cue Data is stored in this nested vector)
	std::vector <FileData> FILE;
	
	//Clears the FILE vector and deallocates its RAM
	void cleanFILE();
	
	/*** Cue file data functions **********************************************/
	//Returns the t_LINE of the string passed (whole line from cue file)
	t_LINE LINEStrToType(const std::string lineStr);
	
	//Return the t_FILE of the string passed
	t_FILE FILEStrToType(const std::string fileStr);	
	
	//Returns the t_TRACK of the string passed
	t_TRACK TRACKStrToType(const std::string trackStr);
	
	//Returns the FILE type string from t_FILE_str via enum
	std::string FILETypeToStr(const t_FILE);
	
	//Returns the TRACK type string from t_TRACK_str via enum
	std::string TRACKTypeToStr(const t_TRACK);
	
	////////////////////////////////////////////////////////////////////////////
	//Validation functions. Returns specific error codes
	//Validate an input .cue file string (argv[1])
	int validateCueFilename(std::string);
	
	//Validate FILE
	int validateFILE(const FileData &);
	
	//Validate TRACK
	int validateTRACK(const TrackData &);
	
	//Validate INDEX
	int validateINDEX(const IndexData &);
	
	////////////////////////////////////////////////////////////////////////////
	//Push a new FILE to FILE[]
	void pushFILE(const std::string FN, const t_FILE TYPE);
	
	//Push a new TRACK to the last entry in FILE[]
	void pushTRACK(const unsigned int ID, const t_TRACK TYPE);
	
	//Push a new INDEX to the last entry in FILE[].TRACK[]
	void pushINDEX(const unsigned int ID, const unsigned long BYTES);

	////////////////////////////////////////////////////////////////////////////
	//Converts FileData Object into a string which is a CUE file line
	std::string generateFILELine(const FileData &);
	
	//Converts TrackData Object into a string which is a CUE file line
	std::string generateTRACKLine(const TrackData &);
	
	//Converts IndexData Object into a string which is a CUE file line
	std::string generateINDEXLine(const IndexData &);
	
	/*** Input / Output CUE Handling ******************************************/
	//Gets all the data from a .cue file and populates the FILE vector.
	void getCueData();
	
	//Combines all the cue FILE data (removes seperate files) and pushes it
	//to the CueHandler object passed via reference.
	//Returns error status TODO
	int combineCueFiles(CueHandler &combined, const std::string outFN,
	                    const std::vector <unsigned long> offsetBytes);
	
	//Outputs the CueData to the cueFile
	int outputCueData();
	
	//Prints a passed FileData Object to the cli
	void printFILE(FileData &);
		
	/*** **********************************************************************/
	//Read in an existing cue file
	void read();
	
	//Create a new cue file
	void create();

	/*** Writing Functions ****************************************************/
	//Output (overwrite) the TeFiEd object RAM into the file.
	void write();

	/** AUX Functions *********************************************************/
	//Converts a number of bytes into an Audio CD timestamp.
	std::string bytesToTimestamp(const unsigned long bytes);
	
	//Converts an Audio CD timestamp into number of bytes
	unsigned long timestampToBytes(const std::string timestamp);
	
	//Modified from TeFiEd. Returns -index- word in a string
	std::string getWord(const std::string input, unsigned int index);
	
	//Takes an input uint32_t, zero-pads to -pad- then return a string
	std::string padIntStr(const unsigned long val, const unsigned int len = 0,
	                      const char pad = '0');

}; //class CueHandler

#endif
