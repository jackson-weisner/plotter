#include <stdint.h>

#ifndef SOLENOID_DEFINITIONS
#define SOLENOID_DEFINITIONS
#define SOLENOID_ON 1
#define SOLENOID_OFF 0
#define SOLENOID_PRE_DELAY 10000
#define F_CPU 1000000UL
typedef uint8_t solenoid_state;
typedef struct solenoid {
    volatile uint8_t* port;
    uint8_t pin;
    solenoid_state state;
} solenoid;
#endif

void solenoid_init(volatile uint8_t* reg, volatile uint8_t* port, uint8_t pin);
void solenoid_on();
void solenoid_off();