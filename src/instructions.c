#include "../include/stepper.h"
#include "../include/solenoid.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

extern stepper* x;
extern stepper* y;
extern uint8_t xPos;
float sizeMult = 1.0;

// map of motor instructions that correspond to ASCII characters
// this is an easy way to draw characters 
instruction charMap[CHAR_MAP_ROWS][MAX_INSTRUCTION_LEN] = {
    {HEADDOWN, UP, UP, RIGHT, DOWN, DOWN, HEADUP, UP, HEADDOWN, LEFT, HEADUP, DOWN, END}, // A
    {HEADDOWN, UP, UP, RIGHT, DOWN, DOWN, LEFT, HEADUP, UP, HEADDOWN, RIGHT, HEADUP, LEFT, DOWN, HEADUP, END}, // B
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, RIGHT, HEADUP, DOWNLEFT, DOWN, END}, // C
    {HEADDOWN, UP, UP, DOWNRIGHT, DOWNLEFT, HEADUP, END}, // D
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, RIGHT, HEADUP, DOWN, HEADDOWN, LEFT, HEADUP, DOWN, END}, // E
    {HEADDOWN, UP, UP, RIGHT, HEADUP, DOWN, HEADDOWN, LEFT, HEADUP, DOWN, END}, // F
    {HEADDOWN, UP, UP, RIGHT, HEADUP, DOWNLEFT, HALF, RIGHT, HEADDOWN, HALF, RIGHT, DOWN, LEFT, HEADUP, END}, // G
    {HEADDOWN, UP, UP, HEADUP, RIGHT, HEADDOWN, DOWN, DOWN, HEADUP, UP, HEADDOWN, LEFT, HEADUP, DOWN, END}, // H
    {HEADDOWN, HALF, RIGHT, UP, UP, HEADUP, HALF, LEFT, HEADDOWN, RIGHT, HEADUP, DOWN, DOWN, HEADDOWN, HALF, LEFT, HEADUP, END}, // I
    {HEADDOWN, }, // J
    {HEADUP, RIGHT, HEADDOWN, UPLEFT, UPRIGHT, HEADUP, LEFT, HEADDOWN, DOWN, DOWN, HEADUP, END}, // K
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, HEADUP, DOWN, DOWN, END}, // L
    {HEADDOWN, UP, UP, HALF, DOWNRIGHT, HALF, UPRIGHT, DOWN, DOWN, HEADUP, LEFT, END}, // M
    {HEADDOWN, }, // N
    {HEADDOWN, UP, UP, RIGHT, DOWN, DOWN, LEFT, HEADUP, END}, // O
    {HEADDOWN, UP, RIGHT, UP, LEFT, DOWN, HEADUP, DOWN, END}, // P
    {HEADDOWN, }, // Q
    {HEADDOWN, UP, RIGHT, UP, LEFT, DOWN, DOWNRIGHT, HEADUP, LEFT, END}, // R
    {HEADDOWN, RIGHT, UP, LEFT, UP, RIGHT, HEADUP, DOWNLEFT, DOWN, END}, // S
    {HEADUP, HALF, RIGHT, HEADDOWN, UP, UP, HEADUP, HALF, LEFT, HEADDOWN, RIGHT, HEADUP, DOWN, DOWN, HEADDOWN, HALF, LEFT, HEADUP, END}, // T
    {HEADDOWN, RIGHT, UP, UP, HEADUP, LEFT, HEADDOWN, DOWN, DOWN, HEADUP, END}, // U
    {HEADUP, HALF, RIGHT, HEADDOWN, HALF, UPRIGHT, HALF, UP, UP, HEADUP, LEFT, HEADDOWN, DOWN, HALF, DOWN, HALF, DOWNRIGHT, HEADUP, HALF, RIGHT, END}, // V
    {HEADDOWN, HALF, UPRIGHT, HALF, DOWNRIGHT, UP, UP, HEADUP, LEFT, HEADDOWN, DOWN, DOWN, HEADUP, END}, // W
    {HEADDOWN, }, // X
    {HEADDOWN, }, // Y
    {HEADDOWN, } // Z
};
instruction nextChar[6] = {HEADUP, RIGHT, HALF, RIGHT, END};
instruction space[4] = {HEADUP, HALF, RIGHT, END};

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
        instruction_executeList(instruction_getList(*letters));
        // if (*letters != ' ') {
            instruction_executeList(nextChar); // shifts to the next starting location
        // }
        xPos++;
        letters++;
    }
    xPos++;
}

void instruction_square(char* dimensions) {}

// this instruction takes a char* that is 0-3
// this maps to a index into a size array and this size multiplier gets set to that value
void instruction_size(char* size) {
    static const int sizeArray[4] = {0.7, 1.0, 1.2, 1.6};
    int index;
    sscanf(size, "%d", &index);
    if (index < 0 || index > 3) return;
    sizeMult = sizeArray[index];
}

// void instruction_swag() {}

// returns the instruction list for the character passed in
instruction* instruction_getList(char c) {
    if (c == ' ') return space;
    return charMap[c - (int)('A')];
}

// executes a list of instructions
// determines what motor movements are required for each movement
void instruction_executeList(instruction* instructions) {
    #define STEPPER_MOVE(a, b) stepper_move(x, a, y, b); break;
    uint8_t halfCount = 0;
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
            case HEADDOWN:  solenoid_on();  break;
            case HEADUP:    solenoid_off(); break;
            case HALF:      
                sizeMult /= 2.0; 
                halfCount++;
                continue;
        }
        while (halfCount > 0) {
            sizeMult *= 2.0;
            halfCount--;
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