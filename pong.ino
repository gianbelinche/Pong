#include "LedControl.h"

/* * * * * * * * * * * * * * * * *
 *  CONSTANTES Y TIPOS DE DATOS
 */
 
#define PRIMERA_MATRIZ 0
#define SEGUNDA_MATRIZ 1
#define POSICION_FILA_MAXIMA 7
#define POSICION_PALETA_MINIMA 2

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
  char vel_x;
  char vel_y;
}pelota_t;

/* * * * * * * * * * * * * * * * *
 *  VARIABLES
 */
 
LedControl lcl=LedControl(PIN_MATRIZ_DATA_IN,PIN_MATRIZ_CLK,PIN_MATRIZ_LOAD,2);  //Maneja las matrices de leds

paleta_t paleta_izq;  //Inicializacion de las paletas y la pelota
paleta_izq.x = 1;
paleta_izq.y = 2;

paleta_t paleta_der;
paleta_der.x = 14;
paleta_der.y = 2;

pelota_t pelota;
pelota.x = 7;
pelota.y = 4;
pelota.vel_x = 1;
pelota.vel_y = 1;

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
  LedControl.clearDisplay(PRIMERA_MATRIZ); // 0 para la primer matriz, uno para la segunda  
  LedControl.clearDisplay(SEGUNDA_MATRIZ);

  inicializar();

  //Inicializacion de pines de botones
  const char pines = {PIN_BOTON_IZQ_ARRIBA,PIN_BOTON_IZQ_ABAJO,PIN_BOTON_DER_ARRIBA,PIN_BOTON_DER_ABAJO}
  for(char i = 0; i <4,i++){
      pinMode(pines[i], INPUT);
  }
}

void loop() {
 administrarEntrada();
 gestionarCondicionesDeVictoria();
}

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES SECUNDARIAS
 */

void moverPaleta(paleta_t* paleta,char vel){
  char nueva_y = paleta.y + vel;
  int led_a_apagar;   
  if(nueva_y >= POSICION_PALETA_MINIMA && nueva_y < POSICION_FILA_MAXIMA){
    if(nueva_y > paleta.y){
      led_a_apagar = nueva_y + 3;
    } else {
      led_a_apagar = nueva_y - 1;
    }
     paleta.y = nueva_y;
     actualizarPaleta(led_a_apagar,paleta); 
  }
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

void actualizarPaleta(int posicion_a_apagar,paleta_t* paleta){
    dibujar(paleta.x,paleta.y,1);
    dibujar(paleta.x,posicion_a_apagar,0);
  }

  

void actualizarPelota(int x_anterior,int y_anterior){
  dibujar(x_anterior,y_anterior,0);
  dibujar(pelota.x,pelota.y,1);
}
  
void moverPelota(){
  pelota.x += vel.x; //Gestionar colisiones
  pelota.y += vel.y;
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
