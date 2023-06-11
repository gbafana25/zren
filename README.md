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
	- `base`: directory holds the original file versions
	- `branches`: holds sub folders for each commit 
	- commits: identifier will be a directory inside their respective branch
	- `logs`: logfiles for each branch, holds commit id, message, action type, and timestamp 
	- `STAGE`: file that holds files ready to be committed
	- `HEAD`: holds current repository head (commit id and branch) 

## Branching
- branches are created with the `rollback` command, which copies all commit files from the specified commit id into a separate branch. These branches are prefixed with `sub-`, along with the first 8 characters of the source commit id. Branches must be manually switched to with the `checkout` command, and all subsequent commit folders are placed in the new branch's folder. A new log function is created and  log information is printed for that branch only.

Article link: https://dev.to/gbafana25/building-a-version-control-system-2e11
