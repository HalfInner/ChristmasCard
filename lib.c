#include "lib.h"

// Description We WishYouAMarryChristmas
// from: http://abcnotation.com/tunePage?a=www.pghardy.net/concertina/tunebooks/pgh_xmas_tunebook/0055
const Tone myMusic[]
PROGMEM =
        {
                {D2,   3},
                {G2,   3},
                {G2,   4},
                {A2,   4},
                {G2,   4},
                {Fis2, 4},
                {E2,   3},
                {C2,   3},
                {E2,   3},

                {A2,   3},
                {A2,   4},
                {B2,   4},
                {A2,   4},
                {G2,   4},
                {Fis2, 3},
                {D2,   3},
                {D2,   3},

                {B2,   3},
                {B2,   4},
                {C3,   4},
                {B2,   4},
                {A2,   4},
                {G2,   3},
                {C2,   3},
                {D2,   4},
                {D2,   4},

                {E2,   3},
                {A2,   3},
                {Fis2, 3},
                {G2,   2},
                {Fis2, 3},
                {G2,   3},
                {G2,   3},
                {G2,   3},
                {Fis2, 2},
                {Fis2, 3},

                {G2,   3},
                {Fis2, 3},
                {E2,   3},
                {D2,   2},
                {A2,   3},
                {C3,   3},
                {A2,   4},
                {A2,   4},
                {G2,   3},

                {D3,   3},
                {D2,   3},
                {D2,   4},
                {D2,   4},
                {E2,   3},
                {A2,   3},
                {Fis2, 3},
                {G2,   1}
        };


volatile bool musicIsRun = false;
volatile uint8_t counterTimer = 0;


//For playing in background
//pointer to play in background;
static const Tone *toneMusic;

//global number of notes
static uint8_t globalSizeOfTones;

//flag Delay
static bool flagDelay;

//flag break between notes
static bool breakNote;

//counter for waiting
static uint8_t counterWaiting;

//global index
static uint8_t iMusic;
////////////////////////////////


//init all
void initPlayer() {
    initPWM();
    initTimer8bit();
    initInterrupt();
}

//its calculating tone to pwm
uint16_t freqToOCR(uint16_t freq) {
    // From datasheet
    return (uint16_t)(F_CPU / (16UL * (uint32_t)(freq)));
}


//What have to be done in Interrupt;
void intRutine() {
    musicIsRun = !musicIsRun;
}

//Set sound value
void playTone(uint16_t value) {
    uint16_t temp = freqToOCR(value);
    ICR1H = temp >> 8;
    ICR1L = temp;
}

//playMusic();
void playMusic(const Tone *tone, uint8_t sizeOfTone) {
    TIMSK |= _BV(TOIE0);
    iMusic = 0;
    toneMusic = tone;
    globalSizeOfTones = sizeOfTone;
}

//Read musicFromFlash
void readFromFlashOneNote(Tone *dest, const void *toneOnFlash) {
    union {
        uint8_t buf[sizeOfToneStructure];
        Tone tone;
    } newTone;

    uint8_t i;
    for (i = 0; i < sizeOfToneStructure; i++)
        newTone.buf[i] = pgm_read_byte(toneOnFlash++);

    *dest = newTone.tone;
}

void setVolume(uint8_t percent) {
    OCR1A = percent;
}

//Hardware initialization Phase and Frequency Correct
void initPWM() {
    PWM_DDR |= _BV(PWM_PIN);

    //Configure by yourself
    //16-bit at 16MHz should be devided by 8
    TCCR1A |= _BV(COM1A1);
    TCCR1B |= _BV(CS11) | _BV(WGM13);
}


//Hardware initialization of Timer 8-bit
void initTimer8bit() {
    //Configure by yourself
    //8-bit timer
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B |= _BV(WGM02) | _BV(CS00) | _BV(CS02);
}

//Hardware initialization INT
void initInterrupt() {
    SW_DDR |= _BV(SW_PIN);
    SW_PORT |= _BV(SW_PIN);

    MCUCR |= _BV(ISC01);
    GIMSK |= _BV(INT0);

    OCR0A = 0xff;
}

//calculate delay
void delayTimer(TypeNote typeNote) {
    uint8_t waitForTimes;
    counterTimer = 0;
    //clearBuffer;
    TCNT0 = 0;
    switch (typeNote) {
        case WholeNote:
            waitForTimes = 64;
            break;

        case HalfNote:
            waitForTimes = 32;
            break;

        case QuarterNote:
            waitForTimes = 16;
            break;

        case EigthNote:
            waitForTimes = 8;
            break;

        case SixteenthNote:
            waitForTimes = 4;
            break;

        default:
            waitForTimes = 1;
            break;
    }

    flagDelay = true;
    counterWaiting = waitForTimes;
}

//Interrupt for delay
ISR(TIMER0_OVF_vect)
        {
                if (musicIsRun)
                {
                    if (flagDelay) {
                        if (counterTimer >= counterWaiting) {
                            flagDelay = false;
                            breakNote = !breakNote;
                        }
                    } else if (iMusic < globalSizeOfTones && !breakNote) {
                        setVolume(100);
                        Tone tmp;

                        //Read From Flash
                        readFromFlashOneNote(&tmp, &toneMusic[iMusic++]);
                        playTone(tmp.toneFreq);
                        delayTimer(tmp.typeNote);
                    } else if (breakNote) {
                        playTone(0);
                        delayTimer(SixteenthNote);
                    } else {
                        setVolume(0);
                        playTone(0);
                        musicIsRun = false;
                    }
                    ++counterTimer;
                }
                else
                {
                    //stop playing after push button
                    setVolume(0);
                    playTone(0);
                    musicIsRun = false;
                    TIMSK &= ~_BV(TOIE0);
                }
        }

//Interrupt to turn on/off music
ISR(INT0_vect)
        {
                intRutine();
        if (musicIsRun)
        playMusic(myMusic, sizeof(myMusic) / sizeOfToneStructure);
        }

