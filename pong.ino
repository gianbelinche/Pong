#include "LedControl.h"
#include <binary.h>

/* * * * * * * * * * * * * * * * *
 *  CONSTANTES Y TIPOS DE DATOS */

#define PRIMERA_MATRIZ 0
#define SEGUNDA_MATRIZ 1

#define X_MINIMA 0
#define X_MAXIMA 7
#define Y_MINIMA 0
#define Y_MAXIMA 7
#define LARGO_MATRIZ 8

#define X_PALETA_IZQ 1
#define X_PALETA_DER 14
#define Y_PALETA_MINIMA 0
#define Y_PALETA_MAXIMA 5
#define Y_PALETA_INICIAL 2
#define LARGO_PALETA 3

#define CANTIDAD_BOTONES 4
#define PIN_BOTON_IZQ_ARRIBA 3
#define PIN_BOTON_IZQ_ABAJO  5
#define PIN_BOTON_DER_ARRIBA 4
#define PIN_BOTON_DER_ABAJO  6

#define PIN_MATRIZ_DATA_IN   10
#define PIN_MATRIZ_CLK       12
#define PIN_MATRIZ_LOAD      11

typedef struct paleta{
  int x;
  int y;
  int vel;
} paleta_t;

typedef struct pelota{
  int x;
  int y;
  int vel_x;
  int vel_y;
} pelota_t;

/* * * * * * * * * * * * * * * * *
 *  VARIABLES */
 
LedControl lcl=LedControl(PIN_MATRIZ_DATA_IN,PIN_MATRIZ_CLK,PIN_MATRIZ_LOAD,2);  //Maneja las matrices de leds.

paleta_t paleta_izq = {X_PALETA_IZQ,Y_PALETA_INICIAL,0};
paleta_t paleta_der = {X_PALETA_DER,Y_PALETA_INICIAL,0};
pelota_t pelota = {X_MAXIMA,rand() % LARGO_MATRIZ,-1,1} ;

int puntaje_izq = 0;
int puntaje_der = 0;

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES AUXILIARES */

void dibujar(int x, int y, int estado){
  int matriz; 
  if (x > X_MAXIMA) {
    matriz = SEGUNDA_MATRIZ;
    x -= LARGO_MATRIZ;
  } else matriz = PRIMERA_MATRIZ;

  lcl.setLed(matriz,x,y,estado);
}

void dibujarEnParalelo(int x,int y,int estado){
  dibujar(x,y,estado);
  dibujar(x + LARGO_MATRIZ,y,estado);
}

void dibujarPaleta(paleta_t paleta){
  for(int i=0;i <= Y_MAXIMA;i++){ //Apaga los led de la columna de la paleta. Ver si funciona.
    dibujar(paleta.x,i,0);
  }
  for(int i=0;i < LARGO_PALETA;i++){ //Prende solo los led de la paleta.
    dibujar(paleta.x,paleta.y + i,1);
  }
}

void moverPaleta(paleta_t* paleta){ 
  int nueva_y = paleta->y + paleta->vel;
  if(nueva_y >= Y_PALETA_MINIMA and nueva_y <= Y_PALETA_MAXIMA){
    paleta->y = nueva_y;
    dibujarPaleta(*paleta);
  }
}

bool colisionConPaleta(){
  bool condicion1 = pelota.x + pelota.vel_x == paleta_izq.x;  
  bool condicion2 = (pelota.y >= paleta_izq.y + pelota.vel_y) and (pelota.y < paleta_izq.y + LARGO_PALETA + pelota.vel_y);
  bool condicion3 = pelota.x + pelota.vel_x == paleta_der.x;
  bool condicion4 = (pelota.y >= paleta_der.y + pelota.vel_y) and (pelota.y < paleta_der.y + LARGO_PALETA + pelota.vel_y);
  return (condicion1 and condicion2) or ( condicion3 and condicion4);
}

void moverPelota(){
  int x_anterior = pelota.x;
  int y_anterior = pelota.y;  

  if ((pelota.y + pelota.vel_y) > Y_MAXIMA or (pelota.y + pelota.vel_y) < Y_MINIMA){
    pelota.vel_y *= -1;
  }
  pelota.y += pelota.vel_y;

  if (colisionConPaleta()){
    pelota.vel_x *= -1;
  }
  pelota.x += pelota.vel_x;

  dibujar(x_anterior,y_anterior,0);
  dibujar(pelota.x,pelota.y,1);
}

void dibujarPuntaje(int matriz,int puntaje){

  if (puntaje == 0){
    byte digito0[] = {B00011000,B00100100,B01000010,B01000010,B01000010,B01000010,B00100100,B00011000};
    for (int i=0;i < LARGO_MATRIZ;i++){
      lcl.setRow(matriz,i,digito0[i]);
    }
  }

  if (puntaje == 1){
    byte digito1[] = {B00011000,B01111000,B00011000,B00011000,B00011000,B00011000,B00011000,B01111110};
    byte digito1_[] = {B01111110,B00011000,B00011000,B00011000,B00011000,B00011000,B00011110,B00011000};
    for (int i=0;i < LARGO_MATRIZ;i++){
      if (matriz == 0){
         lcl.setRow(matriz,i,digito1_[i]);
      }else{
      lcl.setRow(matriz,i,digito1[i]);
    }
  }
  }

  if (puntaje == 2){
    byte digito2[] = {B01111110,B00000010,B00000010,B00000010,B01111110,B01000000,B01000000,B01111110};
    byte digito2_[] = {B01111110,B00000010,B00000010,B01111110,B01000000,B01000000,B01000000,B01111110};
    for (int i=0;i < LARGO_MATRIZ;i++){
      if (matriz == 0){
         lcl.setRow(matriz,i,digito2_[i]);
      }else{
      lcl.setRow(matriz,i,digito2[i]);
    }
  }
  }

  if (puntaje == 3){
    byte digito3[] = {B01111110,B00000010,B00000010,B00111110,B00111110,B00000010,B00000010,B01111110};
    byte digito3_[] = {B01111110,B01000000,B01000000,B01111100,B01111100,B01000000,B01000000,B01111110};
      for (int i=0;i < LARGO_MATRIZ;i++){
        if (matriz == 0){
         lcl.setRow(matriz,i,digito3_[i]);
      }else{
      lcl.setRow(matriz,i,digito3[i]);
    }
      }
  }

  if (puntaje == 4){
    byte digito4[] = {B11000110,B11000110,B11000110,B11000110,B11111110,B00000110,B00000110,B00000110};
    byte digito4_[] = {B01100000,B01100000,B01100000,B01111111,B01100011,B01100011,B01100011,B01100011};
      for (int i=0;i < LARGO_MATRIZ;i++){
        if (matriz == 0){
         lcl.setRow(matriz,i,digito4_[i]);
      }else{
      lcl.setRow(matriz,i,digito4[i]);
    }
      }
  }

  if (puntaje == 5){
    byte digito5[] = {B11111110,B11111110,B11000000,B11000000,B11111110,B00000110,B00000110,B11111110};
    byte digito5_[] = {B01111111,B01100000,B01100000,B01111111,B00000011,B00000011,B01111111,B01111111};
      for (int i=0;i < LARGO_MATRIZ;i++){
        if (matriz == 0){
         lcl.setRow(matriz,i,digito5_[i]);
      }else{
      lcl.setRow(matriz,i,digito5[i]);
    }
      }
  }
}

void inicializar(){
  dibujarPaleta(paleta_izq);
  dibujarPaleta(paleta_der);
  dibujar(pelota.x,pelota.y,1);
}

void administrarEntrada(paleta_t* paleta_der,paleta_t* paleta_izq){
  paleta_der->vel = !digitalRead(PIN_BOTON_DER_ARRIBA) - !digitalRead(PIN_BOTON_DER_ABAJO);
  paleta_izq->vel = !digitalRead(PIN_BOTON_IZQ_ARRIBA) - !digitalRead(PIN_BOTON_IZQ_ABAJO);
}

void transicion(){
  //Fase 1

  delay(165);
  
  dibujarEnParalelo(3,3,1);
  dibujarEnParalelo(3,4,1);  //Prendo leds en forma radial 1
  dibujarEnParalelo(4,3,1);
  dibujarEnParalelo(4,4,1);

  delay(165);
  //Fase 2
  dibujarEnParalelo(3,3,0);
  dibujarEnParalelo(3,4,0);  //Apago leds anteriores
  dibujarEnParalelo(4,3,0);
  dibujarEnParalelo(4,4,0);


  dibujarEnParalelo(3,2,1);
  dibujarEnParalelo(4,2,1);
  dibujarEnParalelo(5,3,1);
  dibujarEnParalelo(5,4,1);  //Prendo segunda tanda de leds
  dibujarEnParalelo(4,5,1);
  dibujarEnParalelo(3,5,1);
  dibujarEnParalelo(2,4,1);
  dibujarEnParalelo(2,3,1);

  delay(165);

  //Fase 3
  dibujarEnParalelo(3,2,0);
  dibujarEnParalelo(4,2,0);
  dibujarEnParalelo(5,3,0);
  dibujarEnParalelo(5,4,0);  //Apago tanda anterior
  dibujarEnParalelo(4,5,0);
  dibujarEnParalelo(3,5,0);
  dibujarEnParalelo(2,4,0);
  dibujarEnParalelo(2,3,0);


  dibujarEnParalelo(3,1,1);
  dibujarEnParalelo(4,1,1);
  dibujarEnParalelo(5,1,1);
  dibujarEnParalelo(5,2,1);
  dibujarEnParalelo(6,2,1);
  dibujarEnParalelo(6,3,1);
  dibujarEnParalelo(6,4,1);
  dibujarEnParalelo(6,5,1);
  dibujarEnParalelo(5,5,1);
  dibujarEnParalelo(5,6,1); //Prendo nueva tanda
  dibujarEnParalelo(4,6,1);
  dibujarEnParalelo(3,6,1);
  dibujarEnParalelo(2,6,1);
  dibujarEnParalelo(2,5,1);
  dibujarEnParalelo(1,5,1);
  dibujarEnParalelo(1,4,1);
  dibujarEnParalelo(1,3,1);
  dibujarEnParalelo(1,2,1);
  dibujarEnParalelo(2,2,1);
  dibujarEnParalelo(2,1,1);

  delay(165);

  //Fase 4

  dibujarEnParalelo(3,1,0);
  dibujarEnParalelo(4,1,0);
  dibujarEnParalelo(5,1,0);
  dibujarEnParalelo(5,2,0);
  dibujarEnParalelo(6,2,0);
  dibujarEnParalelo(6,3,0);
  dibujarEnParalelo(6,4,0);
  dibujarEnParalelo(6,5,0);
  dibujarEnParalelo(5,5,0);
  dibujarEnParalelo(5,6,0); //Apago tanda anterior
  dibujarEnParalelo(4,6,0);
  dibujarEnParalelo(3,6,0);
  dibujarEnParalelo(2,6,0);
  dibujarEnParalelo(2,5,0);
  dibujarEnParalelo(1,5,0);
  dibujarEnParalelo(1,4,0);
  dibujarEnParalelo(1,3,0);
  dibujarEnParalelo(1,2,0);
  dibujarEnParalelo(2,2,0);
  dibujarEnParalelo(2,1,0);
  

  dibujarEnParalelo(1,0,1);
  dibujarEnParalelo(2,0,1);
  dibujarEnParalelo(3,0,1);
  dibujarEnParalelo(4,0,1);
  dibujarEnParalelo(5,0,1);
  dibujarEnParalelo(6,0,1);
  dibujarEnParalelo(6,1,1);
  dibujarEnParalelo(7,1,1);
  dibujarEnParalelo(7,2,1);
  dibujarEnParalelo(7,3,1);
  dibujarEnParalelo(7,4,1);
  dibujarEnParalelo(7,5,1);
  dibujarEnParalelo(7,6,1);
  dibujarEnParalelo(6,6,1); //Ultima tanda
  dibujarEnParalelo(6,7,1);
  dibujarEnParalelo(5,7,1);
  dibujarEnParalelo(4,7,1);
  dibujarEnParalelo(3,7,1);
  dibujarEnParalelo(2,7,1);
  dibujarEnParalelo(1,7,1);
  dibujarEnParalelo(1,6,1);
  dibujarEnParalelo(0,6,1);
  dibujarEnParalelo(0,5,1);
  dibujarEnParalelo(0,4,1);
  dibujarEnParalelo(0,3,1);
  dibujarEnParalelo(0,2,1);
  dibujarEnParalelo(0,1,1);
  dibujarEnParalelo(1,1,1);

  delay(165);
  dibujarEnParalelo(6,1,0); 
  dibujarEnParalelo(6,6,0);
  dibujarEnParalelo(1,6,0); 
  dibujarEnParalelo(1,1,0); 

  dibujarEnParalelo(0,7,1);
  dibujarEnParalelo(7,7,1);
  dibujarEnParalelo(0,0,1);
  dibujarEnParalelo(7,0,1);

  delay(165);
  
  for(int i = 0;i < lcl.getDeviceCount();i++) {
      lcl.clearDisplay(i);
  }
}

void informarVictoria(){
  int matriz_ganadora;
  int matriz_perdedora;
  if(puntaje_izq > puntaje_der){
    matriz_ganadora = PRIMERA_MATRIZ;
    matriz_perdedora = SEGUNDA_MATRIZ;
  } else {
    matriz_ganadora = SEGUNDA_MATRIZ;
    matriz_perdedora = PRIMERA_MATRIZ;    
  }
  
  byte victoria[] = {B11000011,B11000011,B11011011,B11011011,B11011011,B11111111,B11111111,B11100111};
  byte victoria_[] = {B11100111,B11111111,B11111111,B11011011,B11011011,B11011011,B11000011,B11000011};
  for (int i=0;i < LARGO_MATRIZ;i++){
    if (matriz_ganadora == 0){
      lcl.setRow(matriz_ganadora,i,victoria_[i]);
    }else{
      lcl.setRow(matriz_ganadora,i,victoria[i]);
    }
  }

  
  byte derrota[] = {B00110000,B00110000,B00110000,B00110000,B00110000,B00110000,B00111110,B00111110};
  byte derrota_[] = {B01111100,B01111100,B00001100,B00001100,B00001100,B00001100,B00001100,B00001100};
  for (int i=0;i < LARGO_MATRIZ;i++){
    if (matriz_perdedora == 0){
      lcl.setRow(matriz_perdedora,i,derrota_[i]);
    }else{
      lcl.setRow(matriz_perdedora,i,derrota[i]);
    }
  }
  
  delay(1300);
}

void reiniciar(){
  for(int i = 0;i < lcl.getDeviceCount();i++) {
      lcl.clearDisplay(i);
  }
  
  paleta_izq.y = Y_PALETA_INICIAL;
  paleta_der.y = Y_PALETA_INICIAL;
  
  pelota.x = X_MAXIMA;
  pelota.y = rand() % LARGO_MATRIZ;
  pelota.vel_x = 2 * (rand() % 2) -1;
  pelota.vel_y = 2 * (rand() % 2) -1;

  transicion();

  for(int i = 0;i < lcl.getDeviceCount();i++) {
      lcl.clearDisplay(i);
  }  
  delay(500);

  dibujarPuntaje(PRIMERA_MATRIZ,puntaje_izq);
  dibujarPuntaje(SEGUNDA_MATRIZ,puntaje_der);  
  
  delay(1300);
  
  for(int i = 0;i < lcl.getDeviceCount();i++) {
      lcl.clearDisplay(i);
  }

  if(puntaje_der == 5 || puntaje_izq == 5){
    informarVictoria();
    puntaje_der = 0;
    puntaje_izq = 0;

    for(int i = 0;i < lcl.getDeviceCount();i++) {
      lcl.clearDisplay(i);
    }
  }

  inicializar();
  delay(1000);
}

void gestionarCondicionesDeVictoria(){
  if (pelota.x == 0 or pelota.x == 15){
    if(pelota.x == 0)
      puntaje_der++;
    else
      puntaje_izq++;
      
    reiniciar();
  }
}

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES PRINCIPALES */

void setup() {
  for(int index=0 ; index < lcl.getDeviceCount() ; index++) { //Apaga todos los LEDS e inicializa la variable de control de matriz
      lcl.shutdown(index,false);
      lcl.clearDisplay(index);
  }
  inicializar();
  
  //Inicializacion de pines de botones
  const char pines[] = {PIN_BOTON_IZQ_ARRIBA,PIN_BOTON_IZQ_ABAJO,PIN_BOTON_DER_ARRIBA,PIN_BOTON_DER_ABAJO};
  for(char i = 0;i < CANTIDAD_BOTONES;i++){
      pinMode(pines[i], INPUT_PULLUP);
  }
}

void loop() {
 administrarEntrada(&paleta_der,&paleta_izq);
 moverPaleta(&paleta_izq);
 moverPaleta(&paleta_der);
 moverPelota();
 gestionarCondicionesDeVictoria();
 delay(200);
}
