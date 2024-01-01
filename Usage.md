# File Combination
**Note** the following examples can be used interchangably on Windows and Linux,
all that needs to be done is swapping `/` with `\`, for example `directory/path`
becomes `directory\path`.

## Windows Demo
`psx-combine.exe C:\games\files\game.cue`

## General Demos
To time how long it takes to combine files, you can use `-t` or `--time`  
`psx-combine /games/files/game.cue -t`

To get a verbose output of the .cue data, you can use `-v` or `--verbose`  
`psx-combine /games/files/game.cue -v`

To specify the output filename, use `-f` or `--filename`, the default filename
is the same as the input filename  
`psx-combine /games/files/game.cue -f combined`

To specify the output directory for the filed to be put into, use `-d` or 
`--directory`. The default behaviour is to create a folder in the parent folder
of the files called `psx-comBINe`, e.g `/games/game.cue` will be output to
`/games/psx-comBINe/game.cue`  
`psx-combine /games/files/game.cue -d /games/combined/`

`--filename` and `--directory` can be combined, as can `-v` and `-t`

# Cue (re)creation
**IMPORTANT** The .cue recreation functions of this program are not guaranteed 
to fix PSX game dumps, but it can be useful to get the process started. 
The process make a large number of assumptions about the game files. 
These include:

## If creating a .cue from a single file
* It assumes that the TRACKs mode is `MODE2/2352`, this is because the vast 
majority of PSX games follow this pattern.
* It assumes that the TRACK has only one offset, as this is common.

## If creating a .cue from a directory
* It creates the .cue filename from the first binary file it comes across
* It assumes the first TRACK is `MODE2/2352` with no offset.
* Then all sequential TRACKs are assumed to be `Audio`, with an offset of 
00:00:00, then another at 00:02:00 as this is a common case with PSX games

## General Notes
* This functionality cannot be used on a combined file, only "As ripped" PSX 
data will work, as combined files have a lot of TRACKs per file, and these
functions cannot detect that.

## Examples
`psx-combine /games/files/ -c` to create a .cue from a directory  
`psx-combine /games/files/file1.bin` to create a .cue from a single binary file  
`-v/--verbose`, `-d/--directory`, and `-f/--filename` arguments work within the
create mode, but `-t/--timed` does not. For example  
`psx-combine /games/files/ -c --filename recreated`  
`psx-combine /games/files/ -c --directory /games/recreated/`  
`psx-combine /games/files/ -c -v`  
