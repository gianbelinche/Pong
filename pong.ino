#include "LedControl.h"

/* * * * * * * * * * * * * * * * *
 *  CONSTANTES Y TIPOS DE DATOS
 */
 
#define PRIMERA_MATRIZ 0
#define SEGUNDA_MATRIZ 1
#define POSICION_FILA_MAXIMA 7

#define PIN_BOTON_IZQ_ARRIBA 8
#define PIN_BOTON_IZQ_ABAJO  9
#define PIN_BOTON_DER_ARRIBA 10
#define PIN_BOTON_DER_ABAJO  11

#define PIN_MATRIZ_DATA_IN   12
#define PIN_MATRIZ_LOAD      13
#define PIN_MATRIZ_CLK       14

typedef struct paleta{
  char x;
  char y;
}paleta_t;

typedef struct pelota{
  char x;
  char y;
}pelota_t;

/* * * * * * * * * * * * * * * * *
 *  VARIABLES
 */
 
LedControl lcl=LedControl(PIN_MATRIZ_DATA_IN,PIN_MATRIZ_CLK,PIN_MATRIZ_LOAD,2);

paleta_t paleta_izq;
paleta_izq.x = 1;
paleta_izq.y = 2;

paleta_t paleta_der;
paleta_der.x = 14;
paleta_der.y = 2;

pelota_t pelota;
pelota.x = 7;
pelota.y = 4;

char vel_paleta_der = 0;
char vel_paleta_izq = 0;

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES PRINCIPALES
 */
 
void setup() {

  //Apaga todos los LEDS
  for(int index=0 ; index < lcl.getDeviceCount() ; index++) {
      lcl.shutdown(index,false);

  //Inicializa variable de control de matriz
  LedControl.clearDisplay(0) // 0 para la primer matriz, uno para la segunda  
  lcl.setLed(0,2,7,true); //prende en led (2,7), 0 es la matriz a prender.

  //Inicializacion de pines de botones
  const char pines = {PIN_BOTON_IZQ_ARRIBA,PIN_BOTON_IZQ_ABAJO,PIN_BOTON_DER_ARRIBA,PIN_BOTON_DER_ABAJO}
  for(char i = 0; i <4,i++){
      pinMode(pines[i], INPUT);
  }
}

void loop() {
 administrarEntrada()
 actualizarPaleta()
 actualizarPelota()
 gestionarCondicionesDeVictoria()
}

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES SECUNDARIAS
 */

void moverPaleta(paleta_t* paleta,char vel){
  char nueva_y = paleta.y + vel;
  if(nueva_y >= 0 && nueva_y < POSICION_FILA_MAXIMA)
     paleta.y = nueva_y;
}

void dibujar(int x, int y, int estado){
  int matriz = PRIMERA_MATRIZ; 

  if (x > POSICION_FILA_MAXIMA) {
    matriz = SEGUNDA_MATRIZ;
    x -= POSICION_FILA_MAXIMA;
  }

  lcl.setLed(matriz,x,y,estado);
}

void administrarEntrada(){
  vel_paleta_der =  digitalRead(PIN_BOTON_DER_ABAJO) - digitalRead(PIN_BOTON_DER_ARRIBA); 
  vel_paleta_izq =  digitalRead(PIN_BOTON_IZQ_ABAJO) - digitalRead(PIN_BOTON_IZQ_ARRIBA);
}

void actualizarPaleta(){
}

void actualizarPelota(){
}

void gestionarCondicionesDeVictoria(){
}

void dibujarPaleta(paleta_t paleta){
  dibujar(paleta.x,paleta.y,1):
  dibujar(paleta.x+1,paleta.y,1):
  dibujar(paleta.x+2,paleta.y,1):
}

void inicializar(){
  dibujarPaleta(paleta_izq);
  dibujarPaleta(paleta_der);
  dibujar(pelota.x,pelota.y);
}
