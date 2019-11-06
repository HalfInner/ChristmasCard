#include "lib.h"

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
