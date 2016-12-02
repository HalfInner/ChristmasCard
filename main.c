#include "lib.h"


// Description We WishYouAMarryChristmas
// from: http://abcnotation.com/tunePage?a=www.pghardy.net/concertina/tunebooks/pgh_xmas_tunebook/0055
const Tone myMusic[] PROGMEM = 
{
  {D2, 3}, {G2, 3},{G2, 4},{A2, 4},{G2, 4},{F2, 4}, {E2, 3},{C2, 3},{E2, 3}, //9
  
  {A2, 3},{A2, 4},{B2, 4},{A2, 4},{G2, 4}, {F2, 3},{D2, 3},{D2, 3}, // 8
   
  {B2, 3},{B2, 4},{C3, 4},{B2, 4},{A2, 4}, {G2, 3},{C2, 3},{D2, 4},{D2, 4}, //9
  
  {E2, 3},{A2, 3},{F2, 3}, {G2, 2},{F2, 3}, {G2, 3},{G2, 3},{G2, 3}, {F2, 2},{F2, 3}, //10
  
  {G2, 3},{F2, 3},{E2, 3}, {D2, 2},{A2, 3}, {C3, 3},{A2, 4},{A2, 4},{G2, 3}, //9
  
  {D3, 3},{D2, 3},{D2, 4},{D2, 4}, {E2, 3},{A2, 3},{F2, 3}, {G2, 1} //9
};

//MAIN PROGRAM LOOP
int 
main()
{
  initPlayer();
  
  sleep_enable();
  sei();
    
  for(;;)
  {  
    playMusic(myMusic, 54u);
    sleep_cpu(); 
  }
  
  return 0;
}
