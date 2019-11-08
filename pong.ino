#include "LedControl.h"

LedControl lcl=LedControl(12,11,10,1);



void setup() {
  // put your setup code here, to run once:
  for(int index=0 ; index < lcl.getDeviceCount() ; index++) {
      lcl.shutdown(index,false);

  LedControl.clearDisplay(0) // 0 para la primer matriz, uno para la segunda  
  lcl.setLed(0,2,7,true); //prende en led (2,7), 0 es la matriz a prender.
}

void loop() {
  // put your main code here, to run repeatedly:

}

   
