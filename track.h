
typedef enum types_enum {
	CHANGE,
	STAY,
} types;

typedef struct baseobject_struct {
	char *data;
	int pos;

} baseobject;

typedef struct strobject_struct {
	char data;
	struct strobject *next;
	types t;	

} strobject;

void initBaseObject(baseobject *bo, char *data);
void initStrObject(strobject *so, char data, strobject* n, types tp);
void insertStrObject(strobject *so, strobject *n);
void printStrObject(strobject *so);
void printBaseObject(baseobject *bo);
void findDiff(char *base, char *current, strobject *head);
