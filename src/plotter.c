#include <avr/io.h>
#include "../include/stepper.h"
#include "../include/instructions.h"
#include "../include/servo.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

stepper* x;
stepper* y;
servo* s;

void setup() {
    DDRD = 0x00;
    x = stepper_init(&DDRD, &PORTD, PIND5, PIND6);
    y = stepper_init(&DDRD, &PORTD, PIND2, PIND4);
    DDRB = 0x00;
    s = servo_init(&DDRB, PINB1);
    // move to starting location
}

void teardown() {
    // reset to origin
    // cleanup memory
    free(x);
    free(y);
    free(s);
}

void test_servo() {
    DDRB = 0x00 | (1 << DDB1) | (1 << DDB2);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS12);
    ICR1 = F_CPU / 50;  
    OCR1A = ICR1 - 2500;
}

int main() {
    // setup();
	test_servo();
    // teardown();
    return 0;
}