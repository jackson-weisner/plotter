#include <avr/io.h>
#include "../include/stepper.h"
#include "../include/instructions.h"
#include "../include/solenoid.h"
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

    instruction_executeMovement(HEADDOWN);
    _delay_ms(10000);
    instruction_executeMovement(HEADUP);
    _delay_ms(10000);
}

// frees memory
void teardown() {
    // reset the x and y position
    instruction_resetX();
    instruction_resetY();
    free(x);
    free(y);
}

int main() {
    setup();

    instruction_executeMovement(LEFT);

    char* instructions[] = {
        "write JACKSON\nWEISNER",
        "end"
    };
    instruction_parse(instructions);

    teardown();
    return 0;
}