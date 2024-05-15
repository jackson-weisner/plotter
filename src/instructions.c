#include "../include/stepper.h"
#include <util/delay.h>

extern stepper* x;
extern stepper* y;

// map of motor instructions that correspond to ASCII characters
// this is an easy way to draw characters 
instruction charMap[CHAR_MAP_ROWS][MAX_INSTRUCTION_LEN] = {
    {HEADDOWN, UP, UP, RIGHT, RIGHT, DOWN, DOWN, HEADUP, UP, HEADDOWN, LEFT, LEFT, HEADUP, END}, //     A
    {HEADDOWN},
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, RIGHT, HEADUP, END}, // C
    {HEADDOWN, UPRIGHT, UPLEFT, DOWN, DOWN, HEADUP, END},
};

// hash function to get the index in the charMap for the character specified
int instruction_charHash(char c) {
    return c - (int)('A');
}

// function to execute the write instruction
// takes a string 
void instruction_write(char* letters) {
    // TODO: preprocess
    // copy?
    while(*letters != '\0') {
        instruction_execute(charMap[instruction_charHash(*letters)]);
        letters++;
    }
}

// function to dispatch movements to the motors
void instruction_dispatch(movement m) {
    #define STEPPER_MOVE(a, b) stepper_move(x, a, y, b); break;
    switch(m) {
        case UP:        STEPPER_MOVE(NONE, UP);
        case DOWN:      STEPPER_MOVE(NONE, DOWN);
        case LEFT:      STEPPER_MOVE(LEFT, NONE);
        case RIGHT:     STEPPER_MOVE(RIGHT, NONE);
        case UPRIGHT:   STEPPER_MOVE(RIGHT, UP);
        case DOWNRIGHT: STEPPER_MOVE(RIGHT, DOWN);
        case DOWNLEFT:  STEPPER_MOVE(LEFT, DOWN);
        case UPLEFT:    STEPPER_MOVE(LEFT, UP);
    }
}

// executes a list of instructions
// sends them to the dispatch method to be run on their respective motors
void instruction_execute(instruction* instructions) {
    for (int i = 0; instructions[i] != END; i++) {
        _delay_ms(BETWEEN_MOVE_DELAY);
        instruction_dispatch(instructions[i]);
    }
}