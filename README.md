# Christmas Card
## Description

Prototype of 8bit music player. 
First soundtrack is hardcoded in flash memory since there is not enough place in RAM.

## Manual
Click the button to start or stop playing the song 
> We Wish You a Marry christmas 

## HW Connection
```textmate
+--------+
| 16MHz  |
|        |          +----+
|        +--INT0----| SW |------GND
|   uC   |          +----+
| Attiny |
| 2313   |
|        |
|        |          +-----+
|        +--OC1A----| Buz |-----GND
|        |          +-----+
+--------+
```
 * SW - Switch/button 
 * Buz - buzzer
 * uC - Micro controller AtTiny 2313
 
 ## Soft
 1. main.cc - program entries
 2. lib.h - lib interface
 3. lib.cc - lib implementation
 4. LoadFW.sh - script for loading the project into uC 