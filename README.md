# zren 
version control system

## Layout

- changefile format: record all characters that are different, record space between each different character
	- if a change is made in the middle of the file, it will make the diff file really big. However, if data is just appended or not added at all, then the file will be very small
	- store each character/space in a linked list
	- hex code/character to separate space and char nodes when written to file
	- put each character/space different in struct, don't group characters/spaces until writing final commit file

	- base/reference files: (separate from 1st commit) will have their own directory and extension (`.bas`)
	
## Directory Structure
- base `.rep` folder in directory
	- [x] loop through `DIR` object of `commit` directory 
	- [x] each commit identifier will be a directory inside `commits`
	- [x] `base` directory will have files from first commit
	- [x] have `.gitignore`-like file
	- [x] config/log file for commit id, message, and timestamp
	- [x] `STAGE` file that holds files ready to be committed

## New logfile format
- `[prev. commit]:[current commit id] [time] [action] : [message]`

## TODO
- change logfile format to include previous commit, show other actions (revert,rollback)
- make `createCommit` use logfile
- times should be in UTC?
