#include "../include/solenoid.h"
#include <util/delay.h>

solenoid z;

// inits the register for output
// sets the values for the 'z' axis solenoid
void solenoid_init(volatile uint8_t* reg, volatile uint8_t* port, uint8_t pin) {
    *reg |= (1 << pin);
    z.port = port;
    z.pin = pin;
    z.state = SOLENOID_OFF;
}

void solenoid_control(solenoid_state s) {
    _delay_ms(SOLENOID_PRE_DELAY);
    if (z.state != s) {
        *z.port |= (s << z.pin);
        z.state = s;
    }
}

// turns the solenoid on
inline void solenoid_on() {
    _delay_ms(SOLENOID_PRE_DELAY);
    if (z.state == SOLENOID_OFF) {
        *z.port |= (1 << z.pin);
        z.state = SOLENOID_ON;
    }
    _delay_ms(SOLENOID_PRE_DELAY);
    // solenoid_control(SOLENOID_ON);
}

// turns the solenoid off
inline void solenoid_off() {
    _delay_ms(SOLENOID_PRE_DELAY);
    if (z.state == SOLENOID_ON) {
        *z.port &= (0 << z.pin);
        z.state = SOLENOID_OFF;
    }
    _delay_ms(SOLENOID_PRE_DELAY);
    // solenoid_control(SOLENOID_OFF);
}