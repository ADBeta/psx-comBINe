# File Combination
**Note** the following examples can be used interchangably on Windows and Linux,
all that needs to be done is swapping `/` with `\`, for example `directory/path`
becomes `directory\path`.

## Windows Demo
`psx-combine.exe C:\games\files\game.cue`

## General Demos
The most basic use of psx-comBINe is to pass it a .cue file, for example,  
`psx-combine /games/files/game.cue`

You can pass a directory to psx-comBINe and it will look for a .cue file, for
example,  
`psx-combine /games/files/`

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

----
ADBeta (c)    Nov 2023
