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
uint8_t xPos = 0;

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
    _delay_ms(2000);
    while (xPos > 0) {
        stepper_move(x, LEFT, y, NONE);
        xPos--;
    }
    free(x);
    free(y);
}

int main() {
    setup();

    // for (;;) {
    // solenoid_on();
    // _delay_ms(1000);
    // solenoid_off();
    // _delay_ms(1000);
    // }
    // stepper_move(x, LEFT, y, NONE);
    // stepper_move(x, LEFT, y, NONE);
    // stepper_move(x, NONE, y, DOWN);
    // stepper_move(x, NONE, y, DOWN);
    // stepper_move(x, NONE, y, DOWN);

    // instruction_parseLine("write \"ONO\"");
    solenoid_on();
    instruction l[3] = {UP, DOWNRIGHT, END};
    instruction_executeList(l);
    solenoid_off();

    teardown();
    return 0;
}