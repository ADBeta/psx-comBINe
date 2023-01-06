#include <iostream>
#include "TeFiEd.h"
#include "fileHandler.h"

//Cue file TeFiEd Object. Gets Set later.
TeFiEd *cueFile;

//Vector of filenames pulled from the cueFile.
std::vector<char*> binFile;

int main(int argc, char *argv[]){
	test();

	//Test the user input is correct ** TODO
	if(argc == 1) {
		std::cout << "not enough args" << std::endl;
		return 1;
	}
	
	//Set cueFile object to point to the cue file passed via args.
	cueFile = new TeFiEd(argv[1]);
	
	//Read the cueFile, exit if any errors occur.
	if(cueFile->read() != 0) return 1;
	
	//Populate the binFile vector with filenames from the cue file
	std::cout << cueFile->getLine(1) << std::endl;
	
	return 0;
}
