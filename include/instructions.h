#include <stdint.h>

#ifndef INSTRUCTION_DEFINITIONS
#define INSTRUCTION_DEFINITIONS

#define MAX_INSTRUCTION_NAME_LEN 15
#define CHAR_MAP_ROWS 28
#define MAX_INSTRUCTION_LEN 23
#define INSTRUCTION_FUNCTION_COUNT 3
#define EQUAL_STR(x, y) (strcmp(x, y) == 0)
#define SIZE_ARRAY_SIZE 4

#define LENGTH 1000
#define BETWEEN_MOVE_DELAY 4000
typedef enum movement {DOWN, UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWNLEFT, LEFT, UPLEFT, HEADDOWN, HEADUP, HALF, NONE, END} movement;
typedef movement direction;
typedef uint8_t instruction;
typedef instruction* instructionList;
typedef struct instructionFunctions {
    char name[MAX_INSTRUCTION_NAME_LEN];
    void (*f)(char*);
} instructionFunctions;

#endif

instruction* instruction_getList(char c);
void instruction_write(char* letters);
void instruction_square(char* dimensions);
void instruction_size(char* size);

void instruction_executeList(instruction* instructions);
void instruction_parseLine(char* stringInstructions);