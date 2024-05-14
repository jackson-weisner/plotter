#include "../include/stepper.h"
#include <util/delay.h>

extern stepper* x;
extern stepper* y;

instruction charMap[CHAR_MAP_ROWS][MAX_INSTRUCTION_LEN] = {
    {HEADDOWN, UP, UP, RIGHT, RIGHT, DOWN, DOWN, HEADUP, UP, HEADDOWN, LEFT, LEFT, HEADUP, END}, //     A
    {HEADDOWN},
    {HEADUP, RIGHT, HEADDOWN, LEFT, UP, UP, RIGHT, HEADUP, END}, // C
    {HEADDOWN, UPRIGHT, UPLEFT, DOWN, DOWN, HEADUP, END},
};

int instruction_charHash(char c) {
    return c - (int)('A');
}

void instruction_write(char* letters) {
    // TODO: preprocess
    // copy?
    while(*letters != '\0') {
        instruction_execute(charMap[instruction_charHash(*letters)]);
        letters++;
    }
}

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

void instruction_execute(instruction* instructions) {
    for (int i = 0; instructions[i] != END; i++) {
        _delay_ms(BETWEEN_MOVE_DELAY);
        instruction_dispatch(instructions[i]);
    }
}