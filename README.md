# Overview
This project controls two stepper motors on a x/y axis to create a "CNC" type plotter that can draw and write. \
\
This project was written in C without using any Arduino libraries.

# Instructions
- write `STRING TO WRITE`
    - writes the string 
- size <0-3>
    - sets the size for the rest of the instructions
- s
    - draws a cool s
- square
    - draws a square
- cube
    - draws a cube


# Hardware Overview
- Two NEMA 23 stepper motors with 400mm ball screws
- Two TB6600 stepper motor drivers
- 5v 3.6A push pull solenoid
- ATMega328p micro controller 