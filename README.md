# myvcs
version control system

## Layout

- (anti-)compression: record all characters that are different, record space between each different character
	- if a change is made in the middle of the file, it will make the diff file really big. However, if data is just appended or not added at all, then the file will be very small
	- linked list: one node for amount of space, pointer to node w/ different characters 
	- hex code/character to separate space and char nodes when written to file?
	- put each character/space different in struct, don't group characters/spaces until writing final commit file

	- base/reference files: (separate from 1st commit) will have their own directory
	- hash/identifier for now: random string generator
	


## Directory Structure
- base `.rep` folder in directory
	- [x] loop through `DIR` object of `commit` directory 
	- [x] each commit identifier will be a directory inside `commits`
	- [x] `base` directory will have files from first commit
	- [x] have `.gitignore`-like file
	- [ ] file with all commit identifiers?

## Dev. notes
- if there's a malloc error, `make clean` and compile again

## TODO
- make help menu
- add function to move `HEAD` back to specified commit
	- similar to get revert, compare whatever user types
