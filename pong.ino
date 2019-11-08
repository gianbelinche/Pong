#include "LedControl.h"

//Actualizar los pines una vez montemos el dispositivo
LedControl lcl=LedControl(12,11,10,1);

//
int pinDelBoton = 8;
int estadoBoton = 0;

void setup() {

  //Apaga todos los LEDS
  for(int index=0 ; index < lcl.getDeviceCount() ; index++) {
      lcl.shutdown(index,false);

  //Inicializa variable de control de matriz
  LedControl.clearDisplay(0) // 0 para la primer matriz, uno para la segunda  
  lcl.setLed(0,2,7,true); //prende en led (2,7), 0 es la matriz a prender.

  //Inicializa boton
  pinMode(pinDelBoton, INPUT);
}

void loop() {
  estadoBoton = digitalRead(pinDelBoton);
  
  if( estadoBoton == 1) {

  } else {

  }

}

   
