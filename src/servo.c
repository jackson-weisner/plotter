#include "../include/servo.h"

servo* servo_init(volatile uint8_t* port, uint8_t pin) {
    servo* newServo = (servo*)malloc(sizeof(servo));
    newServo->port = port;
    newServo->pin = pin;
    return newServo;
}

void servo_move(const servo* s) {
}