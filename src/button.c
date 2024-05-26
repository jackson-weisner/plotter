#include "../include/button.h"
#include <stdlib.h>
#include<avr/io.h>
#include <util/delay.h>

button* button_init(volatile uint8_t* ddr, volatile uint8_t* readReg, uint8_t togglePin) {
    button* newButton = (button*)malloc(sizeof(button));
    *ddr |= (1 << togglePin);
    newButton->readReg = readReg;
    newButton->togglePin = togglePin;
    return newButton;
}

void button_wait(button* b) {
    while (!(*b->readReg & (1 << b->togglePin))) {
        _delay_ms(BUTTON_WAIT_DELAY);   
    }
}