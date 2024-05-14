#include <stdint.h>

#ifndef SERVO_DEFINITIONS
#define SERVO_DEFINITIONS

typedef struct servo {
    volatile uint8_t* port;
    uint8_t pin;
} servo;

#endif

servo* servo_init(volatile uint8_t* port, uint8_t pin);

void servo_move(const servo* s);