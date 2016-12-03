#include "lib.h"



//////////////////////////////////////////
// Current HW implementation
//
// +--------+
// | 16MHz  |
// |        |          +----+
// |        +--INT0-----|SW|------GND
// |   uC   |          +----+
// | Attiny |
// | 2313   |
// |        |
// |        |           +--------+
// |        +--OC1A------|Buzzer|--GND
// |        |           +--------+
// +--------+
//
//
//////////////////////////////////////////

//MAIN PROGRAM LOOP
int main()
{
  initPlayer();
  
  sleep_enable();
  sei();
    
  for(;;)
  {      
    sleep_cpu(); 
  }
  
  return 0;
}
