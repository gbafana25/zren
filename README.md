# myvcs
version control system

## Layout

- compression: record all characters that are different, record space between each different character
	- linked list: one node for amount of space, pointer to node w/ different characters 
	- hex code/character to separate space and char nodes when written to file?
- commit: struct with list of unions, union value can either be space struct or character struct
- base/reference files: (1st commit) will have their directory
