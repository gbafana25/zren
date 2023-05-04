# myvcs
version control system

## Layout

- compression: record all characters that are different, record space between each different character
	- linked list: one node for amount of space, pointer to node w/ different characters 
	- hex code/character to separate space and char nodes when written to file?
		- could this be included in the struct itself?
	- put each character/space different in struct, don't group characters/spaces
- base/reference files: (1st commit) will have their directory
