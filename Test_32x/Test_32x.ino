
#include "LedControl.h"

LedControl lc = LedControl(11,13,10,4);

void setup(){
 
  lc.shutdown(0,false);
  lc.setIntensity(0, 15);

 
  lc.shutdown(1,false);
  lc.setIntensity(1,15);
 
  
  lc.shutdown(2,false);
  lc.setIntensity(2,15);

  
  lc.shutdown(3,false);
  lc.setIntensity(3,15);

  
}
  
void loop(){
  
    lc.setLed(0,1,1,true);
    lc.setLed(1,1,1,true);
    lc.setLed(2,1,1,true);
    lc.setLed(3,1,1,true);
}
