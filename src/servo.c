#include "../include/servo.h"
#include <stdlib.h>
#include <avr/io.h>

void servo_init(volatile uint8_t* reg, uint8_t pin) {
    *reg = 0x00 | (1 << pin);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);                 // set up for fast PWM with TCR1 as TOP
	TCCR1B |= (1 << CS11) |(1 << WGM12) | (1 << WGM13);     // pre scale = 8

    // ICR1 = ((period ms * (F_CPU/1,000,000)) / pre scale) - 1;  
    ICR1 = 39999;
}

void servo_move(float dutyCycleTime) {
    // set OCR1A to a percentage of the ICR1 value
    OCR1A = (dutyCycleTime/20)*ICR1;
}