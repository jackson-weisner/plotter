#include "../include/stepper.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

extern stepper* x;
extern stepper* y;
float sizeMult = 1.0;

// map of motor instructions that correspond to ASCII characters
// this is an easy way to draw characters 
instruction charMap[CHAR_MAP_ROWS][MAX_INSTRUCTION_LEN] = {
    {HEADUP}, // space
    {HEADDOWN, UP, UP, RIGHT, RIGHT, DOWN, DOWN, HEADUP, UP, HEADDOWN, LEFT, LEFT, HEADUP, END}, //     A
    {HEADDOWN},
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, RIGHT, HEADUP, END}, // C
    {HEADDOWN, UPRIGHT, UPLEFT, DOWN, DOWN, HEADUP, END},
};

const instructionFunctions instructionFunctionList[INSTRUCTION_FUNCTION_COUNT] = {
    {"write", instruction_write},
    {"square", instruction_square},
    {"size", instruction_size}
};

// function to execute the write instruction
// takes a string 
void instruction_write(char* letters) {
    letters++;
    while(*letters != '\"') {
        instruction_executeList(charMap[instruction_charHash(*letters)]);
        letters++;
    }
}

void instruction_square(char* dimensions) {}

void instruction_size(char* size) {
    if (EQUAL_STR(size, "large"))     sizeMult = 1.6; return; 
    if (EQUAL_STR(size, "medium"))    sizeMult = 1.2; return;
    if (EQUAL_STR(size, "small"))     sizeMult = 0.7; return;
    sizeMult = 1.0;
}

// void instruction_swag() {}

// hash function to get the index in the charMap for the character specified
int instruction_charHash(char c) {
    if (c == ' ') return 0;
    return c - (int)('A');
}

// executes a list of instructions
// determines what motor movements are required for each movement
void instruction_executeList(instruction* instructions) {
    #define STEPPER_MOVE(a, b) stepper_move(x, a, y, b); break;
    bool halfBool = FALSE;
    for (int i = 0; instructions[i] != END; i++) {
        _delay_ms(BETWEEN_MOVE_DELAY);
        switch(instructions[i]) {
            case UP:        STEPPER_MOVE(NONE, UP);
            case DOWN:      STEPPER_MOVE(NONE, DOWN);
            case LEFT:      STEPPER_MOVE(LEFT, NONE);
            case RIGHT:     STEPPER_MOVE(RIGHT, NONE);
            case UPRIGHT:   STEPPER_MOVE(RIGHT, UP);
            case DOWNRIGHT: STEPPER_MOVE(RIGHT, DOWN);
            case DOWNLEFT:  STEPPER_MOVE(LEFT, DOWN);
            case UPLEFT:    STEPPER_MOVE(LEFT, UP);
            case HALF:      
                sizeMult /= 2; 
                halfBool = TRUE;
                continue;
        }
        if (halfBool == TRUE) {
            halfBool = FALSE;
            sizeMult *= 2;
        }
    }
}


// read a instruction line
// grab the name of the instruction, then call the proper function for the instruction
// the instruction function will parse the parameters and convert it to motor instructions
void instruction_parseLine(char* stringInstructions) {
    char instructionName[MAX_INSTRUCTION_NAME_LEN];
    sscanf(stringInstructions, "%s ", instructionName);

    // loop over function list
    for (int i = 0; i < INSTRUCTION_FUNCTION_COUNT; i++) {
        if (strcmp(instructionFunctionList[i].name, instructionName) == 0) {
            instructionFunctionList[i].f(stringInstructions + strlen(instructionName) + 1);
        }
    }
}