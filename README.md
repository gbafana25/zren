# myvcs
version control system

## Layout

- compression: record all characters that are different, record space between each different character
	- linked list: one node for amount of space, pointer to node w/ different characters 
	- hex code/character to separate space and char nodes when written to file?
		- could this be included in the struct itself?
	- put each character/space different in struct, don't group characters/spaces
- base/reference files: (1st commit) will have their directory
- switch type from enum to int (in struct itself, don't delete enum)

## Directory Structure
- base `.rep` folder in directory
	- file w/ list of commit hashes/identifiers
	- each commit identifier will be a directory inside `commits`
	- each changefile will have same name as source file, but with different extentions
	- `base` directory will have files from first commit
