#include "../include/stepper.h"
#include "../include/solenoid.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

extern stepper* x;
extern stepper* y;
float xPos = 0.0;
float yPos = 0.0;
float sizeMult = 1.0;

// map of motor instructions that correspond to characters (A-Z)
// this is an easy way to draw characters 
instruction charMap[CHAR_MAP_ROWS][MAX_INSTRUCTION_LEN] = {
    {HEADDOWN, T_UP, T_RIGHT, T_DOWN, HEADUP, UP, HEADDOWN, T_LEFT, HEADUP, DOWNRIGHT, RIGHT, END}, // A
    {HEADDOWN, T_UP, T_RIGHT, T_DOWN, T_LEFT, HEADUP, UP, HEADDOWN, T_RIGHT, HEADUP, DOWN, END}, // B
    {HEADUP, T_RIGHT, HEADDOWN, T_LEFT, T_UP, T_RIGHT, HEADUP, T_DOWN, END}, // C
    {HEADDOWN, T_UP, RIGHT, DOWNRIGHT, DOWNLEFT, LEFT, HEADUP, T_RIGHT, END}, // D
    {HEADDOWN, T_UP, T_RIGHT, HEADUP, DOWNLEFT, LEFT, HEADDOWN, T_RIGHT, HEADUP, DOWNLEFT, LEFT, HEADDOWN, T_RIGHT, HEADUP, END}, // E
    {HEADDOWN, T_UP, T_RIGHT, HEADUP, DOWNLEFT, LEFT, HEADDOWN, T_RIGHT, HEADUP, DOWN, END}, // F
    {HEADUP, T_UPRIGHT, HEADDOWN, T_LEFT, T_DOWN, T_RIGHT, UP, LEFT, HEADUP, DOWNRIGHT, END}, // G
    {HEADDOWN, UP, T_RIGHT, UP, HEADUP, T_LEFT, HEADDOWN, DOWN, HEADUP, T_RIGHT, HEADDOWN, DOWN, HEADUP, END}, // H
    {HEADDOWN, RIGHT, T_UP, LEFT, HEADUP, RIGHT, HEADDOWN, RIGHT, HEADUP, DOWNLEFT, DOWN, HEADDOWN, RIGHT, HEADUP, END}, // I
    {HEADDOWN, RIGHT, T_UP, LEFT, HEADUP, RIGHT, HEADDOWN, RIGHT, HEADUP, T_DOWN, END}, // J
    {HEADDOWN, T_UP, HEADUP, DOWN, HEADDOWN, RIGHT, UPRIGHT, HEADUP, DOWNLEFT, HEADDOWN, DOWNRIGHT, HEADUP, END}, // K
    {HEADUP, T_UP, HEADDOWN, T_DOWN, T_RIGHT, HEADUP, END}, // L
    {HEADDOWN, T_UP, DOWNRIGHT, UPRIGHT, T_DOWN, HEADUP, END}, // M
    {HEADDOWN, T_UP, DOWNRIGHT, DOWNRIGHT, T_UP, HEADUP, T_DOWN, END}, // N
    {HEADDOWN, T_RIGHT, T_UP, T_LEFT, T_DOWN, HEADUP, T_RIGHT, END}, // O
    {HEADDOWN, T_UP, T_RIGHT, DOWN, T_LEFT, HEADUP, DOWNRIGHT, RIGHT, END}, // P
    {HEADDOWN, T_RIGHT, T_UP, T_LEFT, T_DOWN, HEADUP, T_RIGHT, HALF, UPLEFT, HEADDOWN, DOWNRIGHT, HEADUP, HALF, UPLEFT, END}, // Q
    {HEADDOWN, UP, T_RIGHT, UP, T_LEFT, DOWN, HEADUP, RIGHT, HEADDOWN, DOWNRIGHT, HEADUP, END}, // R
    {HEADDOWN, T_RIGHT, UP, T_LEFT, UP, T_RIGHT, HEADUP, T_DOWN, END}, // S
    {HEADUP, RIGHT, HEADDOWN, T_UP, LEFT, HEADUP, RIGHT, HEADDOWN, RIGHT, HEADUP, T_DOWN, END}, // T
    {HEADDOWN, T_RIGHT, T_UP, HEADUP, T_LEFT, HEADDOWN, T_DOWN, HEADUP, T_RIGHT, END}, // U
    {HEADUP, T_UP, HEADDOWN, DOWN, DOWNRIGHT, UPRIGHT, UP, HEADUP, T_DOWN, END}, // V
    {HEADUP, T_UP, HEADDOWN, T_DOWN, UPRIGHT, DOWNRIGHT, T_UP, HEADUP, T_DOWN, END}, // W
    {HEADDOWN, T_UPRIGHT, HEADUP, LEFT, HEADDOWN, T_DOWNRIGHT, HEADUP, END}, // X
    {HEADUP, RIGHT, HEADDOWN, UP, UPLEFT, HEADUP, T_RIGHT, HEADDOWN, DOWNLEFT, HEADUP, DOWNRIGHT, END}, // Y
    {HEADUP, T_UP, HEADDOWN, T_RIGHT, T_DOWNLEFT, T_RIGHT, HEADUP, END} // Z
};
instruction nextChar[6] = {HEADUP, HALF, RIGHT, END};
instruction space[3] = {HEADUP, RIGHT, END};
uint8_t halfCount = 0;

const instructionFunctions instructionFunctionList[INSTRUCTION_FUNCTION_COUNT] = {
    {"write", instruction_write},
    {"square", instruction_square},
    {"s", instruction_s},
    {"cube", instruction_cube},
    {"size", instruction_size}
};

void instruction_nextLine() {
    instruction_resetX();
    instruction_executeMovement(T_DOWN);
    instruction_executeMovement(DOWN);
    yPos += 3;
}

// function to execute the write instruction
// takes a string 
void instruction_write(char* letters) {
    while(*letters != '\0') {
        if (*letters == '\n') {
            instruction_nextLine();
            letters++;
            continue;
        }
        instruction_executeList(instruction_getList(*letters));
        instruction_executeList(nextChar); // shifts to the next starting location
        xPos += 2.5;
        letters++;
    }
    // xPos += 0.5;
}

void instruction_square(char* dimensions) {}

// instructions to draw a 3d cube
void instruction_cube(char* dimensions) {
    static instruction cubeInstructions[] = {HEADDOWN, T_UP, T_RIGHT, T_DOWN, T_LEFT, HEADUP, T_UP, HEADDOWN, UPRIGHT, T_RIGHT, DOWNLEFT, HEADUP, T_DOWN, HEADDOWN, UPRIGHT, T_UP, HEADUP, END};
    instruction_executeList(cubeInstructions);
}

// this instruction takes a char* that is 0-3
// this maps to a index into a size array and this size multiplier gets set to that value
void instruction_size(char* size) {
    static const int sizeArray[4] = {0.7, 1.0, 1.2, 1.6};
    int index;
    sscanf(size, "%d", &index);
    if (index < 0 || index > 3) return;
    sizeMult = sizeArray[index];
}

void instruction_s() {
    static instruction sInstructions[] = {HEADDOWN, DOWN, DOWNRIGHT, UPRIGHT, UP, UPLEFT, UP, HEADUP, DOWNRIGHT, HEADDOWN, UP, UPLEFT, DOWNLEFT, DOWN, DOWNRIGHT, DOWN, HEADUP, END};
    instruction_executeList(sInstructions);
}

// returns the instruction list for the character passed in
instruction* instruction_getList(char c) {
    if (c == ' ') return space;
    return charMap[c - (int)('A')];
}

// executes a list of instructions
// determines what motor movements are required for each movement
void instruction_executeList(instruction* instructions) {
    for (int i = 0; instructions[i] != END; i++) {
        instruction_executeMovement(instructions[i]);
    }
}

// determines what motor movements need to happen based on the input movement
void instruction_executeMovement(movement m) {
    static movement lastMovement = NONE;
    if (m != lastMovement && m < T_DOWN) {
        lastMovement = m;
        _delay_ms(BETWEEN_MOVE_DELAY);
    }
    switch(m) {
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
            return;
    }
    if (m >= T_DOWN && m <= T_UPLEFT && m) {
        instruction_executeMovement((movement)m-T_DOWN);
        instruction_executeMovement((movement)m-T_DOWN);
    }
    while (halfCount > 0) {
        sizeMult *= 2.0;
        halfCount--;
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

// parses all instructions
void instruction_parse(char** instructions) {
    while (strcmp(*instructions, "end") != 0) {
        instruction_parseLine(*instructions);
        instructions++;
    }
}


// this resets the axisCount and the stepper motor positions to the default
void instruction_resetAxis(float* axisCount, movement m) {
    _delay_ms(2000);
    while (*axisCount > 0.5) {
        instruction_executeMovement(m);
        (*axisCount)--;
    }

    // determines if there
    if (*axisCount == 0.5) {
        instruction halfDirection[3] = {HALF, m, END};
        instruction_executeList(halfDirection);
    }
    *axisCount = 0;
}


inline void instruction_resetX() {
    instruction_resetAxis(&xPos, LEFT);
}

inline void instruction_resetY() {
    instruction_resetAxis(&yPos, UP);
}