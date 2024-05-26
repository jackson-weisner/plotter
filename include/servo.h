#include <stdint.h>

#ifndef SERVO_DEFINITIONS
#define SERVO_DEFINITIONS

#define SERVO_DOWN 2.0
#define SERVO_UP 2.5

#endif

void servo_init(volatile uint8_t* reg, uint8_t pin);

void servo_move(float dutyCycleTime);