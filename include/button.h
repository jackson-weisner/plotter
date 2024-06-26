#include <stdint.h>
#ifndef CPU_FREQ
#define CPU_FREQ
#define F_CPU 16000000UL
#endif

#ifndef BUTTON_DEFINITIONS
#define BUTTON_DEFINITIONS
#define BUTTON_WAIT_DELAY 25
typedef struct button {
    volatile uint8_t* readReg;
    uint8_t togglePin;
} button;
#endif

button* button_init(volatile uint8_t* ddr, volatile uint8_t* readReg, uint8_t togglePin);
void button_wait(button* b);