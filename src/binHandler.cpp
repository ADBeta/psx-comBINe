/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing, this is ideal for PSX/PS1 CD-ROMs
* e.g. Rayman to get them ready for cue2pops or some emulators. I also find it 
* improves reliabilty when buring to a disk to only have one .bin file.
*
* (c) ADBeta
*******************************************************************************/
#include <iostream>
#include <vector>
#include <fstream>

#include "helpers.h"
#include "binHandler.h"

//Each entity is at which byte a file transistion to the next file. This is for
//.cue output INDEX value (in time format which gets converted later)
std::vector<size_t> fileIndexByte;

int dumpBinFiles(std::vector<std::string> &binVect, const std::string outFn) {
	/*** Setup ***/
	//Input and output .bin files.
	std::fstream binFileOut, binFileIn;

	//Create the output binary file and check it is open
	binFileOut.open(outFn.c_str(), std::ios::out | std::ios::binary);
	if(!binFileOut) errorMsg(2, "Cannot create output .bin file. Check privelages");
	
	//Current byte in array, bytes in each file, and total bytes from all files
	size_t arrBytes = 0, fileBytes = 0, totalBytes = 0;
	
	//Create a heap byte array (From defined size in header)
	char *byteArray = new char[_def_ARR_SIZE];
	//TODO error handle
	//errorMsg(2, "Failed to allocate the bin buffer array (Requires 100MiB free RAM)");
	
	
	/*** runtime ***/
	//Go through all bin filenames in the vector of strings
	for(size_t indx = 0; indx < binVect.size(); indx++) {
		
		//Print message about file (flush to ensure the text is printed first)
		std::cout << "Dumping: " << binVect.at(indx) << "    " << std::flush;
		
		//Open input binary file to the current string in the vecor.
		binFileIn.open(binVect[indx].c_str(), std::ios::in | std::ios::binary);
		//error check
		if(!binFileIn) errorMsg(2, "Can not open the input .bin file");
		//Seek to the beginning of the in file to clear flags. Belt and braces
		binFileIn.seekg(0, std::ios::beg);
		
		//Get all the bytes from the current input and push them to output.
		char cByte;
		while(binFileIn.get(cByte)) {
			//Put the read byte into the array
			byteArray[arrBytes] = cByte;
			++arrBytes;
			
			//If the array is full, dump it to the output file, and reset.
			if(arrBytes == _def_ARR_SIZE) {
				binFileOut.write(byteArray, arrBytes);
				arrBytes = 0;
			}
			
			//Keep track of how many bytes read so far, fileByte gets reset at
			//next loop, totalBytes does not get reset
			++totalBytes;
			++fileBytes;
		}
		
		//Log the transistion byte of the current file for INDEX in .cue
		fileIndexByte.push_back(totalBytes);
		
		//Close the current file for next loop
		binFileIn.close();
		
		//Report how many megabytes the file is, that it is done, then reset.
		std::cout << padMiBStr(fileBytes, 3) << "    Done" << std::endl;
		fileBytes = 0;
	}
	
	//Flush what is left of the byte array to the output file
	if(arrBytes != 0) binFileOut.write(byteArray, arrBytes);

	//Delete heap byte array
	delete[] byteArray;
	
	//Close the output file
	binFileOut.close();
	
	//Report completion and bytes written to the output bin file
	std::cout << std::endl << "Successfully dumped " << padByteStr(totalBytes) 
	          << " to " << outFn << std::endl;

	//Return 0 for success
	return 0;
}