#include "lib.h"

volatile bool musicIsRun = false;
volatile uint8_t counterTimer = 0;


//init all
void initPlayer()
{
  initPWM();
  initTimer8bit();
  initInterrupt();
}

//its calculating tone to pwm
uint16_t freqToOCR(uint16_t freq)
{
  // From datasheet
  return (uint16_t)(F_CPU / (16UL * (uint32_t)(freq)));
}


//What have to be done in Interrupt;
void intRutine()
{
  musicIsRun = !musicIsRun;
}

//Set sound value
void playTone(uint16_t value)
{
  uint16_t temp = freqToOCR(value);
  ICR1H = temp >> 8;
  ICR1L = temp; 
}

//playMusic();
void playMusic(const Tone *tone, uint8_t sizeOfTone)
{  
  setVolume(100);
  uint8_t i = 0;
  Tone tmp;
  for (i = 0; i < sizeOfTone && musicIsRun; ++i)
  {
    //Read From Flash
    readFromFlashOneNote(&tmp, &tone[i]);
    
    playTone(tmp.toneFreq);
    
    delayTimer(tmp.typeNote);
    playTone(0);
    delayTimer(SixteenthNote);
  }
  
  //stop playing after push button
  setVolume(0);
  playTone(0);
  musicIsRun = false;
}

//Read musicFromFlash
void readFromFlashOneNote(Tone *dest, const void *toneOnFlash)
{
  union 
  {
    uint8_t buf[sizeOfTone];
    Tone tone;
  } newTone;
  
  uint8_t i;
  for (i = 0; i < sizeOfTone; i++)
    newTone.buf[i] = pgm_read_byte(toneOnFlash++);
  
  *dest = newTone.tone;  
}

void setVolume(uint8_t percent)
{
  OCR1A = percent;
}

//Hardware initialization Phase and Frequency Correct
void initPWM()
{
  PWM_DDR   |= _BV(PWM_PIN);
   
  //Configure by yourself  
  //16-bit at 16MHz should be devided by 8  
  TCCR1A |= _BV(COM1A1);
  TCCR1B |= _BV(CS11) | _BV(WGM13); 
}


//Hardware initialization of Timer 8-bit
void initTimer8bit()
{
  //Configure by yourself  
  //8-bit timer  
  TCCR0A |= _BV(WGM01) | _BV(WGM00); 
  TCCR0B |= _BV(WGM02) | _BV(CS00) | _BV(CS02);
}

//Hardware initialization INT
void initInterrupt()
{
  SW_DDR  |= _BV(SW_PIN);
  SW_PORT |= _BV(SW_PIN);
  
  MCUCR |= _BV(ISC01);
  GIMSK |= _BV(INT0);
  
  OCR0A = 0xff;
}

//calculate delay
void delayTimer(TypeNote typeNote)
{ 
  uint8_t waitForTimes;
  counterTimer = 0;
  //clearBuffer;
  TCNT0 = 0;  
  switch(typeNote)
  {
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
  
  TIMSK  |= _BV(TOIE0);
  while (counterTimer < waitForTimes && musicIsRun)
  {
    sleep_cpu();
  }
  TIMSK  &= _BV(TOIE0);
}

//Interrupt for delay
ISR(TIMER0_OVF_vect)
{
  ++counterTimer;
}

//Interrupt to turn on/off music
ISR(INT0_vect)
{
  intRutine();
}

