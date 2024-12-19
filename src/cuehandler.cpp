/*******************************************************************************
* This file is part of cuehandler. 
* See the GitHub for more information: https://github.com/ADBeta/cuehandler
* cuehandler is under GPL 2.0. See LICENSE for more information
*
* A simple & efficient Library to create, modify and impliment .CUE files in C++
* with detailed support of the GNU Specification, with ability to combine .cue
* file FILE's into one. 
*
* See example.cpp for an example how to use the library
*******************************************************************************/
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <cctype>
#include <limits>

#include "cuehandler.hpp"

/*** Cue File/Sheet Exceptions ************************************************/
CueException file_invalid("Filename given cannot be opened or is invalid");
CueException internal_file_invalid("A file defined in .cue cannot be opened or is invalid");
CueException line_invalid("A TRACK type in the cue file is not recognised");

CueException file_push_null_input("Push To File has nullptr input pointer");
CueException track_push_null_input("Push To Track has nullptr input pointer");
CueException index_push_null_input("Push To Index has nullptr input pointer");

CueException track_id_too_high("Track has an ID greater than 99");
CueException index_id_too_high("Index has an ID greater than 99");
CueException timestamp_invalid_minutes("Index has a Timestamp greater than 99 minutes");
CueException timestamp_invalid_track("Invalid TrackType passed to timestamp conversion");
CueException timestamp_invalid_bytes("Invalid bytes passed to timestamp conversion");
CueException timestamp_bytes_mismatch("Bytes given and bytes-per-sector are not matched");

CueException track_push_null_file("Push To Track has no valid parent File");
CueException index_push_null_track("Push To Index has no valid parent Track");

/*** Staitc Helpers ***********************************************************/
//Change file delim based on the host OS. eg / on Linux \ on Windows
#ifdef _WIN32
	//#pragma message("Using Windows")
	static const char _file_delim = '\\';
#else
	//#pragma message("using Linux")
	static const char _file_delim = '/';
#endif

static bool IsValidTimestamp(const std::string &ts) {
	bool valid = false;
	
	//Check length, then each char one at a time. 
	if(ts.length() == 8 && isdigit(ts[0]) && isdigit(ts[1]) && ts[2] == ':'
	                    && isdigit(ts[3]) && isdigit(ts[4]) && ts[5] == ':'
	                    && isdigit(ts[6]) && isdigit(ts[7])) {
		valid = true;
	}
	return valid;	
}

//Convers an unsigned value to a padded string of n length
static std::string NumToStrPad(const size_t num, const size_t len) {
	std::string num_str = std::to_string(num);
	
	//If len is larger than str.length, padd with '0' for remaining chars.
	if(len > num_str.length()) num_str.insert(0, (len - num_str.length()), '0');
	return num_str;
}

//Strip and return the parent directory of a given filename/directory
static std::string GetParentDir(const std::string &input) {
	std::string dir;
	//Only if there IS a delim char
	size_t delim;
	if( (delim = input.find_last_of(_file_delim)) != std::string::npos) {
		dir = input.substr(0, delim + 1); //+1 to get the / after dir
	}
	
	return dir;
}

//Gets nth word starting from the front of an input string
static std::string GetWordFromFront(const size_t word_index,
                                                     const std::string &input) {
	const char *delims = " \t\r";
	std::string word_str;
	
	//Don't execute if requested word is 0
	if(word_index > 0) {	
		//Current word_start(ws), word_end(we), and word_count(wc)
		size_t ws = 0, we = 0, wc = 0;	
		
		//While the start of a new word exists
		while((ws = input.find_first_not_of(delims, we)) != std::string::npos) {
			//Find the end of that word, incriment word count, find match and return
			we = input.find_first_of(delims, ws);
		
			if(++wc == word_index) {
				word_str = input.substr(ws, we - ws);
				break;
			}
		}
	}
	
	return word_str;
}

//Gets nth word starting from the back of an input string
static std::string GetWordFromBack(const size_t word_index,
                                                     const std::string &input) {	
	const char *delims = " \t\r";
	std::string word_str;
	
	//Don't execute if requested word is 0
	if(word_index > 0) {	
		//Current word_start(ws), word_end(we), and word_count(wc)
		size_t ws = std::string::npos, we = std::string::npos, wc = 0;	
		
		do {
			we = input.find_last_not_of(delims, ws);
			ws = input.find_last_of(delims, we);
		
			if(++wc == word_index) {
				word_str = input.substr(ws + 1, we - ws);
				break;
			} 
			
		} while(ws != std::string::npos && we != std::string::npos);
	}
	
	return word_str;
}

//Takes a value and divisor, (uint32_t) and sets passed quotient & remainder 
//variables (uint32_t)
static void DivideWithRemainder32(const uint32_t value, const uint32_t divisor,
                                      uint32_t &quotient, uint32_t &remainder) {
	quotient = value / divisor;
	remainder = value % divisor;
}

//Takes a string with text in double quote marks, removes the first child in
//quotes, returns it without quotes.
static std::string GetTextInQuotes(const std::string &input) {
	//Empty string. Return on error, or set to substr of input if success
	std::string substr;
	
	//Find the first and last Double Quote mark, and get a substring from it
	size_t first = input.find_first_of("\"");	
	size_t last = input.find_last_of("\"");
		
	//Only get a substr if both "s are present. Only first will be set if there
	//is only one ", if there's non, first & last will be string::npos
	if(first != last && first != std::string::npos) {
		//Incriment first to skip first "
		++first;
		substr = input.substr(first, last-first);
	}
	
	return substr;
}

/*** Cue File Functions *******************************************************/
//Reads the given file, Parses it, sets up the CueData structure tree
int CueFile::ReadCueData(CueSheet &cs) {
	//Guard against use without a set filename, Attempt to open the file.
	if(this->filename.empty() || this->OpenRead() != 0) {
		throw file_invalid;
		return -1;
	}
		
	//Reset file pointer/flags, then read the file into a RAM File
	this->cue_file.clear();
	this->cue_file.seekg(0, std::ios::beg);
	
	//Read the File line by line.
	std::string line_str;
	while(getline(this->cue_file, line_str)) {
		//Skip Blank Lines
		if(line_str.empty()) continue;
		
		//Get the type of line this current line is
		CueSheet::LineType l_type = 
			             CueSheet::StrToLineType(GetWordFromFront(1, line_str));
		
		//Exit if the line is invalid
		if(l_type == CueSheet::LineType::Invalid) {
			throw line_invalid;
			return -2;
		}
		
		//Strip the Filename and Type, then push it to the CueSheet FileList
		if(l_type == CueSheet::LineType::File) {
			std::string fname = GetTextInQuotes(line_str);
			std::string ftype = GetWordFromBack(1, line_str);
			
			
			CueSheet::FileObj temp_file(fname, ftype, 0);
			cs.PushFile(&temp_file);
		}
		
		//Strip the Track ID and type and push it to a TrackList
		if(l_type == CueSheet::LineType::Track) {
			uint16_t t_id = (uint16_t)std::stoi(GetWordFromFront(2, line_str));
			
			CueSheet::TrackType t_ty = 
			            CueSheet::StrToTrackType(GetWordFromFront(3, line_str));
			
			CueSheet::FileObj::TrackObj temp_track(t_id, t_ty);
			cs.PushTrack(&temp_track);
		}
		
		//Strip Index ID and Bytes and push it to an IndexList
		if(l_type == CueSheet::LineType::Index) {
			//CueSheet::FileObj::TrackObj *t_ptr = ;
			uint16_t i_id = (uint16_t)std::stoi(GetWordFromFront(2, line_str));
						
			uint32_t i_offset = CueSheet::TimestampToBytes(
			            GetWordFromFront(3, line_str), cs.GetLastTrack()->type);
			
			CueSheet::FileObj::TrackObj::IndexObj tmp(i_id, i_offset);
			cs.PushIndex(&tmp);
		}
	}
	
	//Close the File and return success
	this->Close();
	return 0;
}

int CueFile::GetCueFileSizes(CueSheet &cs, std::string base_dir) {
	if(base_dir.empty()) base_dir = GetParentDir(this->filename);
	
	for(auto &f_itr : cs.FileList) {
		if(f_itr.filename.empty()) {throw internal_file_invalid; return -1;}
		f_itr.bytes = GetFileBytes(base_dir + f_itr.filename);
	}
	
	return 0;
}

int CueFile::WriteCueData(const CueSheet &cs) {
	//Guard against use without a set filename, Attempt to open the file.
	if(this->filename.empty() || this->OpenWrite() != 0) {
		throw file_invalid;
		return -1;
	}
	
	//Reset file pointer/flags, then read the file into a RAM File
	this->cue_file.clear();
	this->cue_file.seekg(0, std::ios::beg);
	
	//this->cue_file << cs.ToString();
	// Write the CueSheet data with Windows-style line endings (\r\n)
	std::string cueData = cs.ToString();
	this->cue_file.write(
		cueData.c_str(), 
		static_cast<std::streamsize>(cueData.length())
	);

	//Close the File and return success
	this->Close();
	return 0;
}

uint32_t CueFile::GetFileBytes(const std::string &fname) {	
	uint32_t bytes = 0;
	
	//flag ok state if filename is not empty
	bool ok_flag = !fname.empty();
	
	if(ok_flag) {
		//Attempt to open the file. Set ok_flag to false if it wasn't open
		std::fstream file(fname, std::ios::in | std::ios::binary);
		ok_flag = file.is_open();
		
		if(ok_flag) {
			file.clear();
			file.seekg(0, file.end);
			bytes = static_cast<uint32_t>(file.tellg());
			file.close();
		}
	}
	
	if(ok_flag == false) throw internal_file_invalid;
	return bytes;
}

/*** Cue File Private Member Functions ****************************************/
int CueFile::OpenRead() {
	int errcode = 0;
	
	this->cue_file.open(
		this->filename, 
		std::ios::in
	);

	if(this->cue_file.is_open() == false) errcode = -1;
	return errcode;
}

int CueFile::OpenWrite() {
	int errcode = 0;

	// Open file in Binary Mode to avoid extra \r's from being added
	this->cue_file.open(
		this->filename, 
		std::ios::out | std::ios::binary | std::ios::trunc
	);

	if(this->cue_file.is_open() == false) errcode = -1;
	return errcode;
}

int CueFile::Close() {
	int errcode = 0;
	
	this->cue_file.flush();
	this->cue_file.close();
	if(this->cue_file.fail()) errcode = -1;
	return errcode;
}

/*** API Functions ************************************************************/
CueSheet::~CueSheet() {
	this->Clear();
}

CueSheet::FileObj *CueSheet::GetLastFile() {
	FileObj *file_ptr = nullptr;
	if(!this->FileList.empty()) file_ptr = &this->FileList.back();
	return file_ptr;
}

CueSheet::FileObj::TrackObj *CueSheet::GetLastTrack() {
	FileObj *file_ptr = GetLastFile();
	FileObj::TrackObj *track_ptr = nullptr;
	
	if(file_ptr && !file_ptr->TrackList.empty()) {
		track_ptr = &file_ptr->TrackList.back();
	}
	return track_ptr;
}

CueSheet::FileObj::TrackObj::IndexObj *CueSheet::GetLastIndex() {
	FileObj::TrackObj *track_ptr = GetLastTrack();
	FileObj::TrackObj::IndexObj *index_ptr = nullptr;
	
	if(track_ptr && !track_ptr->IndexList.empty()) {
		index_ptr = &track_ptr->IndexList.back();
	}
	return index_ptr;
}

int CueSheet::PushFile(FileObj *file_ptr) {
	//Guard against nullptr input
	if(file_ptr == nullptr) {throw file_push_null_input; return -1;}
	
	//Push the file to the list
	this->FileList.push_back(*file_ptr);
	return 0;
}

int CueSheet::PushTrack(FileObj::TrackObj *track_ptr) {
	//Guard null input
	if(track_ptr == nullptr) {throw track_push_null_input; return -1;}
	//Guard against over 99 track IDs
	if(track_ptr->id > 99) {throw track_id_too_high; return -2;}
	//Get the last file in list, make sure it is a valid pointer
	FileObj *f_last = this->GetLastFile();
	if(f_last == nullptr) {throw track_push_null_file; return -3;}
	
	//Push the track passed to the file pointer
	f_last->TrackList.push_back(*track_ptr);
	return 0;
}

int CueSheet::PushIndex(FileObj::TrackObj::IndexObj *index_ptr) {
	//Guard null input
	if(index_ptr == nullptr) {throw index_push_null_input; return -1;}
	//Guard against over 99 Index IDs
	if(index_ptr->id > 99) {throw index_id_too_high; return -2;}
	//Get the last track in the list, make sure it is a valid pointer
	FileObj::TrackObj *t_last = this->GetLastTrack();
	if(t_last == nullptr) {throw index_push_null_track; return -3;}
	
	//Push the index passed to the track
	t_last->IndexList.push_back(*index_ptr);
	return 0;
}

int CueSheet::PopFile() {

	while(this->PopTrack() == 0);
	
	if(!this->FileList.empty()) this->FileList.pop_back();
	
	return 0;
}

int CueSheet::PopTrack() {
	//Return -1 for nullptr errors and -2 for empty errors
	FileObj *f_last = this->GetLastFile();
	if(!f_last) return -1;
	
	//Clear the index array to dealocate RAM If it is not empty
	FileObj::TrackObj *t_last = this->GetLastTrack();
	if(t_last && !t_last->IndexList.empty()) t_last->IndexList.clear();
	
	//Pop off the track
	if(f_last->TrackList.empty()) return -2;
	f_last->TrackList.pop_back();
	return 0;
}

int CueSheet::PopIndex() {
	FileObj::TrackObj *t_last = this->GetLastTrack();
	
	//Return -1 for nullptr errors and -2 for empty errors
	if(t_last == nullptr) return -1;
	if(t_last->IndexList.empty()) return -2;
	
	t_last->IndexList.pop_back();
	return 0;
}

std::string CueSheet::ToString() const {
	std::string output;
	
	if(!this->FileList.empty()) {
		for(const auto &f_itr : this->FileList) {
			output.append(FileToStr(&f_itr) + "\r\n");
			for(const auto &t_itr : f_itr.TrackList) {
				output.append(TrackToStr(&t_itr) + "\r\n");
				for(const auto &i_itr : t_itr.IndexList) {
					output.append(IndexToStr(&i_itr, t_itr.type) + "\r\n");
				}
			}
		}
	}
	
	return output;
}

int CueSheet::Print() const {
	if(this->FileList.empty()) return -1;

	for(const auto &f_itr : this->FileList) {
		std::cout << FileToStr(&f_itr) + "    " << f_itr.bytes << " bytes\n";
		for(const auto &t_itr : f_itr.TrackList) {
			std::cout << TrackToStr(&t_itr) << "\n";
			for(const auto &i_itr : t_itr.IndexList) {
				std::cout << IndexToStr(&i_itr, t_itr.type) << "    " 
					      << i_itr.offset << " bytes offset\n";
			}
		}
	}
	
	return 0;
}

void CueSheet::Combine(std::string op_filename, std::string op_filetype) {
	//If the file list is empty, exit early
	if(this->FileList.empty()) return;	
	
	//Check input strings and set them to the parent FileObjs strings if they
	//were not specified
	if(op_filename.empty()) op_filename = this->FileList.begin()->filename;
	if(op_filetype.empty()) op_filetype = this->FileList.begin()->filetype;
	
	//Create a temp cue sheet to copy to, this will be the new Combined File
	CueSheet temp_cue;
	FileObj temp_file(op_filename, op_filetype, 0);
	if(temp_cue.PushFile(&temp_file) != 0) return;
	
	//Keep track of the total bytes in all files so far, for index offsets
	uint32_t total_file_bytes = 0;
	
	//Go through all Files
	for(auto &f_itr : this->FileList) {
	
		//Copy the track data to a new obj and push it to the temp sheet
		for(auto &t_itr : f_itr.TrackList) {
			FileObj::TrackObj temp_track(t_itr.id, t_itr.type);
			temp_cue.PushTrack(&temp_track);
			
			//Copy the index data, but offset from total bytes in the new file
			for(auto &i_itr : t_itr.IndexList) {
				FileObj::TrackObj::IndexObj temp_index
				                    (i_itr.id, i_itr.offset + total_file_bytes);
				temp_cue.PushIndex(&temp_index);
			}
		}
		//Update total bytes so far, for index offsets
		total_file_bytes += f_itr.bytes;
	}
	
	//Fix the file bytes to reflect its actual size
	temp_cue.FileList.begin()->bytes = total_file_bytes;
	temp_cue.CopyTo(*this);
}

void CueSheet::Clear() {
	for(auto &f_itr : this->FileList) {
		for(auto &t_itr : f_itr.TrackList) {
			t_itr.IndexList.clear();
		}
		f_itr.TrackList.clear();
	}
	
	this->FileList.clear();
}

int CueSheet::CopyTo(CueSheet &target) {	
	//Clear the input first, then copy all data to it
	target.Clear();
	
	//NOTE: I could do this by just pushing a pointer to each file iterator,
	//but I am not sure if this is a safe thing to do if the object calling the
	//copy goes out of scope or invalidates its own pointers
	for(auto &f_itr : this->FileList) {
		FileObj tf(f_itr.filename, f_itr.filetype, f_itr.bytes);
		if(target.PushFile(&tf) != 0) return -1;

		for(auto &t_itr : f_itr.TrackList) {
			FileObj::TrackObj tt(t_itr.id, t_itr.type);
			if(target.PushTrack(&tt) != 0) return -2;
			
			for(auto &i_itr : t_itr.IndexList) {
				FileObj::TrackObj::IndexObj ti(i_itr.id, i_itr.offset);
				if(target.PushIndex(&ti) != 0) return -3;
			}
		}
	}
	
	return 0;
}

/*** Structure Functions ******************************************************/
CueSheet::LineType CueSheet::StrToLineType(const std::string &input) {	
	//Default to Invalid. Set to value if compare matches
	LineType type = LineType::Invalid;
	     if(input.compare("FILE") == 0)    type = LineType::File;
	else if(input.compare("TRACK") == 0)   type = LineType::Track;
	else if(input.compare("INDEX") == 0)   type = LineType::Index;
	else if(input.compare("REM") == 0)     type = LineType::Remark;
	
	return type;
}

std::string CueSheet::LineTypeToStr(const LineType type) {
	std::string type_str;
	     if(type == LineType::Invalid)    type_str = "INVALID";
	else if(type == LineType::File)       type_str = "FILE";
	else if(type == LineType::Track)      type_str = "TRACK";
	else if(type == LineType::Index)      type_str = "INDEX";
	else if(type == LineType::Remark)     type_str = "REM";
	
	return type_str;
}

CueSheet::TrackType CueSheet::StrToTrackType(const std::string &input) {
	//Default to Invalid. Set to value if compare matches
	TrackType type = TrackType::Invalid;
	     if(input.compare("AUDIO") == 0)      type = TrackType::AUDIO;
	else if(input.compare("CDG") == 0)        type = TrackType::CDG;
	else if(input.compare("MODE1/2048") == 0) type = TrackType::MODE1_2048;
	else if(input.compare("MODE1/2352") == 0) type = TrackType::MODE1_2352;
	else if(input.compare("MODE2/2336") == 0) type = TrackType::MODE2_2336;
	else if(input.compare("MODE2/2352") == 0) type = TrackType::MODE2_2352;
	else if(input.compare("CDI/2336") == 0)   type = TrackType::CDI_2336;
	else if(input.compare("CDI/2352") == 0)   type = TrackType::CDI_2352;
	
	return type;
}

std::string CueSheet::TrackTypeToStr(const TrackType type) {
	std::string type_str;
	     if(type == TrackType::Invalid)       type_str = "INVALID";
	else if(type == TrackType::AUDIO)         type_str = "AUDIO";
	else if(type == TrackType::CDG)           type_str = "CDG";
	else if(type == TrackType::MODE1_2048)    type_str = "MODE1/2048";
	else if(type == TrackType::MODE1_2352)    type_str = "MODE1/2352";
	else if(type == TrackType::MODE2_2336)    type_str = "MODE2/2336";
	else if(type == TrackType::MODE2_2352)    type_str = "MODE2/2352";
	else if(type == TrackType::CDI_2336)      type_str = "CDI/2336";
	else if(type == TrackType::CDI_2352)      type_str = "CDI/2352";
	
	return type_str;
}

uint16_t CueSheet::GetSectorBytesInTrackType(const TrackType type) {
	uint16_t bytes = 0;
	     if(type == TrackType::Invalid)       bytes = 0;
	else if(type == TrackType::AUDIO)         bytes = 2352;
	else if(type == TrackType::CDG)           bytes = 2448;
	else if(type == TrackType::MODE1_2048)    bytes = 2048;
	else if(type == TrackType::MODE1_2352)    bytes = 2352;
	else if(type == TrackType::MODE2_2336)    bytes = 2336;
	else if(type == TrackType::MODE2_2352)    bytes = 2352;
	else if(type == TrackType::CDI_2336)      bytes = 2336;
	else if(type == TrackType::CDI_2352)      bytes = 2352;
	
	return bytes;
}

std::string CueSheet::BytesToTimestamp(const uint32_t bytes,
                                       const TrackType type) {
	std::string timestamp;
	
	if(type == TrackType::Invalid || bytes == timestamp_nval) {
		if(type == TrackType::Invalid) throw timestamp_invalid_track;
		if(bytes == timestamp_nval) throw timestamp_invalid_bytes;
		return timestamp;
	}
	
	uint32_t sector_quo, sector_rem;
	uint16_t sect_bytes = GetSectorBytesInTrackType(type);
	DivideWithRemainder32(bytes, sect_bytes, sector_quo, sector_rem);
	
	//Make sure the remainder is 0 to make sure the bytes-per-sector and bytes
	//align properly
	if(sector_rem != 0) {
		throw timestamp_bytes_mismatch;
		return timestamp;
	}
	
	//Convert bytes into minutes seconds and frames only if the bytes and 
	//tracktype bytes-per-sector match.
	uint32_t minutes, seconds, frames;
	
	//Get seconds and frames from sectors (75 sectors per sec)
	DivideWithRemainder32(sector_quo, 75, seconds, frames);
	//Convert Seconds into minutes while keeping remainder seconds
	DivideWithRemainder32(seconds, 60, minutes, seconds);

	//Guard against out of spec minutes value (over 99)
	if(minutes > 99) {
		throw timestamp_invalid_minutes;
		return timestamp;
	}
	
	timestamp = NumToStrPad(minutes, 2) + ":" + NumToStrPad(seconds, 2) 
		                                    + ":" + NumToStrPad(frames, 2);
	
	return timestamp;
}

uint32_t CueSheet::TimestampToBytes(const std::string &timestamp, 
                                    const TrackType type) {
    uint32_t total_bytes = timestamp_nval;
        
	if(type != TrackType::Invalid && IsValidTimestamp(timestamp)) {
		//Strip values from the timestamp. "MM:SS:ff"
		//NOTE: Minutes are already guarded from being over 99 because 
		//of IsValidTimestamp
		uint32_t minutes, seconds, frames;
		minutes = static_cast<uint32_t>(std::stoul(timestamp.substr(0, 2)));
		seconds = static_cast<uint32_t>(std::stoul(timestamp.substr(3, 2)));
		frames  = static_cast<uint32_t>(std::stoul(timestamp.substr(6, 2)));
		
		//75 sectors per second, convert mins to secs, plus the leftover frames
		//Which are 1 sector each
		uint32_t sectors = ((seconds + (minutes * 60)) * 75) + frames;
		uint16_t sect_bytes = GetSectorBytesInTrackType(type);
		total_bytes = sectors * sect_bytes;
	}
		
	return total_bytes;
}

std::string CueSheet::FileToStr(const FileObj *file_ptr) {
	std::string output;
	if(file_ptr != nullptr) {
		output = "FILE \"" + file_ptr->filename + "\" " + file_ptr->filetype;
	}
	
	return output;
}

std::string CueSheet::TrackToStr(const FileObj::TrackObj *track_ptr) {
	std::string output;
	if(track_ptr != nullptr) {
		output = "  TRACK " + NumToStrPad(track_ptr->id, 2) + " " 
							+ TrackTypeToStr(track_ptr->type);
	}
	
	return output;
}				          

std::string CueSheet::IndexToStr(const FileObj::TrackObj::IndexObj *index_ptr,
                                                         const TrackType type) {
	std::string output;
	if(index_ptr != nullptr) {
		output = "    INDEX " + NumToStrPad(index_ptr->id, 2) + " " 
		            		  + BytesToTimestamp(index_ptr->offset, type);
	}
	
	return output;
}
