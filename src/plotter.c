#include <avr/io.h>
#include "../include/stepper.h"
#include "../include/instructions.h"
#include "../include/solenoid.h"
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

    solenoid_init(&DDRB, &PORTB, PINB0); 

    // don't start until button is pressed
    button* waitButton = button_init(&DDRB, &PINB, PINB5);
    button_wait(waitButton);
    free(waitButton);

    // move to starting location
}

// frees memory
void teardown() {
    // reset the x position
    instruction_resetX();
    instruction_resetY();
    free(x);
    free(y);
}

int main() {
    setup();

    instruction_parseLine("write COOLEO");

    teardown();
    return 0;
}