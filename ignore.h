#ifndef IGNORE_H
#define IGNORE_H

#include <stdbool.h>

#define IGNORE_FILE ".ignore"


typedef enum i_type_enum {
	REG_FILE,
	EXT_WILDCARD,
} i_type;


char *openIgnoreFile();
char **parseIgnoreFile(char *data, int *size);
bool inIgnore(char *filename, char **ign, int i_size);

#endif
