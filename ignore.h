#ifndef IGNORE_H
#define IGNORE_H

#define IGNORE_FILE ".ignore"

typedef enum i_type_enum {
	REG_FILE,
	EXT_WILDCARD,
} i_type;


char *openIgnoreFile(char **ignore);
char **parseIgnoreFile(char *data, int *size);

#endif
