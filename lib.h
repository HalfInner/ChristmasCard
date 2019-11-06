#ifndef LIB_H
#define LIB_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>

#include <stdint.h>
#include <stdbool.h>

//DEFINE YOUR HARDWARE
#define PWM_PORT PORTB
#define PWM_DDR  DDRB
#define PWM_PIN  PB3
#define PWM_OCR  OCR1A

#define SW_PORT  PORTD
#define SW_DDR   DDRD
#define SW_PIN   PD2

//Current state of player
extern volatile bool musicIsRun;

//Counter of timer overflow
extern volatile uint8_t counterTimer;


//Enumeration of common notes
typedef enum {
    A1 = 110,  //5th string
    B1 = 123,
    C2 = 130,
    D2 = 146,  //4th string
    E2 = 164,
    F2 = 175,
    Fis2 = 184,
    G2 = 196,  //3r string
    A2 = 220,
    B2 = 246,  //2nd string
    C3 = 261,
    D3 = 293,
    E3 = 330,  //1st string
    F3 = 350,
    Fis3 = 369,
    G3 = 392,
    A3 = 440   //1st string
} ToneFreq;

//Enumartion of note's length
typedef enum {
    WholeNote = 1,
    HalfNote,
    QuarterNote,
    EigthNote,
    SixteenthNote
} TypeNote;


//structure of one Tone
typedef struct {
    //frequency
    ToneFreq toneFreq;

    //Divider of note -> Whole note / half note ect.
    TypeNote typeNote;
} Tone;

static const uint8_t sizeOfToneStructure = sizeof(Tone);

//Setting volume
void setVolume(uint8_t percent);

//SOFT
//Calculating tone frequency to value of pwm
uint16_t freqToOCR(uint16_t freq);

//What have to be done in Interrupt;
void inline intRutine();

//Set sound value
void playTone(uint16_t value);

//playMusic();
void playMusic(const Tone *tone, uint8_t sizeOfTone);

//HARD

//Hardware initialization PWM
void initPWM();

//Hardware initialization of Timer 8-bit
void initTimer8bit();

//calculate delay
void delayTimer(TypeNote typeNote);

//Hardware initialization INT
void initInterrupt();

//Init player
void initPlayer();

//Read musicFromFlash
void readFromFlashOneNote(Tone *dest, const void *toneOnFlash);

#endif // LIB_H
