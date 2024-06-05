#include "../include/stepper.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

extern stepper* x;
extern stepper* y;
extern float sizeMult;

void blink() {
    DDRB = 0xff; 
    int delay = 10000;
    for (;;) {
        PORTB = (1 << PINB5);
        _delay_ms(1000);
        PORTB = (0 << PINB5);
        _delay_ms(1000);
    }
}

// function to init a stepper motor struct
stepper* stepper_init(volatile uint8_t* reg, volatile uint8_t* port, int pulsePin, int directionPin) {
    stepper* newStepper = (stepper*)malloc(sizeof(stepper));
    newStepper->port = port;
    newStepper->pulsePin = pulsePin;
    newStepper->directionPin = directionPin;
    newStepper->dMask = 0x00;
    *reg |= (1 << newStepper->directionPin) | (1 << newStepper->pulsePin);
    return newStepper;
}

// this sets the bit mask for 
void stepper_setDirectionMask(stepper* s, direction d) {
    uint8_t bit = d;
    if (d == LEFT || d == RIGHT) {
        bit = (d == LEFT) ? UP : DOWN;
    }
    s->dMask = 0x00 | (bit << s->directionPin);
}

// function to reset the port and set the direction masks for the motors
void stepper_initMove(stepper* s, direction d) {
    *s->port = 0x00;
    stepper_setDirectionMask(s, d);
}

// function to turn the stepper motor on in the specified direction
void stepper_on(stepper* s) {
    *s->port |= (1 << s->pulsePin) | (s->dMask);
}

// turns the motor off
void stepper_off(stepper* s) {
    *s->port &= (0 << s->pulsePin) | (s->dMask);
}

// function to move both x and y stepper motors in their respective directions
// they can move at the same time (diagonal movements) or separately 
void stepper_move(stepper* x, direction xd, stepper* y, direction yd) {
    stepper_initMove(x, xd);
    stepper_initMove(y, yd);
    int dist = DISTANCE * sizeMult;
	for (int i = 0; i < dist; i++) {
        TOGGLE(x, on);
        TOGGLE(y, on);
        _delay_ms(STEPPER_DELAY);
        TOGGLE(x, off);
        TOGGLE(y, off);
        _delay_ms(STEPPER_DELAY);
	}
}