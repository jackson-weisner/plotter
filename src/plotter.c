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

// function to init all motors and clear ports
void setup() {
    DDRD = 0x00;
    x = stepper_init(&DDRD, &PORTD, PIND5, PIND6);
    y = stepper_init(&DDRD, &PORTD, PIND2, PIND4);
    DDRB = 0x00;
    s = servo_init(&DDRB, PINB1);
    // move to starting location
}

// frees memory
void teardown() {
    free(x);
    free(y);
    free(s);
}

void test_servo(int x) {
    DDRB = 0x00 | (1 << DDB1) | (1 << DDB2);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS12);
    ICR1 = F_CPU / 50;  
    OCR1A = ICR1 - x;
}

int main() {
    // setup();
    for(;;) {
	    test_servo(3000);
        _delay_ms(2000);
	    test_servo(3999);
        _delay_ms(2000);
    }
    // teardown();
    return 0;
}