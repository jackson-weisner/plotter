#include <stdint.h>

#ifndef INSTRUCTION_DEFINITIONS
#define INSTRUCTION_DEFINITIONS

#define CHAR_MAP_ROWS 26
#define MAX_INSTRUCTION_LEN 20
#define LENGTH 1000
#define BETWEEN_MOVE_DELAY 15000
typedef enum movement {DOWN, UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWNLEFT, LEFT, UPLEFT, HEADDOWN, HEADUP, NONE, END} movement;
typedef movement direction;
typedef uint8_t instruction;
typedef instruction* instructionList;

#endif

int instruction_charHash(char c);
void instruction_dispatch(movement m);
void instruction_execute(instruction* instructions);


// void readInstructionList(char** instructions);
// void parseInstruction(char* instruction);
void instruction_write(char* letters);
// void