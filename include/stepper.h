#include <stdint.h>
#include "../include/instructions.h"

#ifndef CPU_FREQ
#define CPU_FREQ
#define F_CPU 1000000UL
#endif

#ifndef STEPPER_DEFINITIONS
#define STEPPER_DEFINITIONS
#define MOVEMENT_COUNT 12
#define STEPPER_DELAY 10
#define DISTANCE (7000 / (STEPPER_DELAY * 2))
#define TOGGLE(s, mode) if (s ## d != NONE) stepper_ ## mode(s)
typedef enum power {OFF, ON} power; 
typedef struct stepper {
    volatile uint8_t* port;
    uint8_t pulsePin;
    uint8_t directionPin;
    uint8_t dMask;
} stepper;

#endif


stepper* stepper_init(volatile uint8_t* reg, volatile uint8_t* port, int pulsePin, int directionPin);

void stepper_setDirectionMask(stepper* s, direction d);
void stepper_initMove(stepper* s, direction d);
void stepper_move(stepper* x, direction xd, stepper* y, direction yd);
void stepper_on(stepper* s);
void stepper_off(stepper* s);

void blink();