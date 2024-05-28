#include <avr/io.h>
#include "../include/stepper.h"
#include "../include/instructions.h"
#include "../include/servo.h"
#include "../include/button.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

stepper* x;
stepper* y;

// function to init all motors and clear ports
void setup() {
    DDRD = 0x00;
    x = stepper_init(&DDRD, &PORTD, PIND5, PIND6);
    y = stepper_init(&DDRD, &PORTD, PIND2, PIND4);
    // DDRB = 0x00;
    // servo_init(&DDRB, PINB1);

    // don't return until button is pressed
    // button* waitButton = button_init(&DDRB, &PINB, PINB4);
    // button_wait(waitButton);
    // free(waitButton);

    // move to starting location
}

// frees memory
void teardown() {
    free(x);
    free(y);
}

int main() {
    setup();
    // blink();
    // instruction list[3] = {UP, UP, END};
    // instruction_executeList();
    stepper_move(x,LEFT,y,NONE);
    // stepper_move()
    // instruction_write("\"ABC\"");
    teardown();
    return 0;
}