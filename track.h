
typedef enum types_enum {
	STR,
	SPACE,
} types;

typedef struct baseobject_struct {
	char *data;
	int pos;

} baseobject;

typedef struct strobject_struct {
	char *data;
	struct strobject *next;
	int len;

} strobject;

typedef struct spaceobject_struct {
	int num_spaces;
	struct spaceobject *next;

} spaceobject;

typedef union commit_union {
	strobject *str;
	spaceobject *space;
	types t;
	// put pointer to next union
} commit;

void initBaseObject(baseobject *bo, char *data);
void initStrObject(strobject *so, char *data, strobject* n, int len);
void initSpaceObject(spaceobject *so, int num, spaceobject* n);
void insertStrObject(strobject *so, strobject *n);
void insertSpaceAfter(spaceobject *so, spaceobject *n);
void printStrObject(strobject *so);
void printSpaces(spaceobject *so);
void applyStrChange(char *current, strobject *so, int *p);
void findDiff(char *base, char *current, commit *ca);
