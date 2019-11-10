#include "LedControl.h"

/* * * * * * * * * * * * * * * * *
 *  CONSTANTES Y TIPOS DE DATOS
 */
 
#define PRIMERA_MATRIZ 0
#define SEGUNDA_MATRIZ 1
#define POSICION_FILA_MAXIMA 7
#define POSICION_PALETA_MINIMA 2
#define LARGO PALETA 3

#define PIN_BOTON_IZQ_ARRIBA 8
#define PIN_BOTON_IZQ_ABAJO  9
#define PIN_BOTON_DER_ARRIBA 10
#define PIN_BOTON_DER_ABAJO  11

#define PIN_MATRIZ_DATA_IN   12
#define PIN_MATRIZ_LOAD      13
#define PIN_MATRIZ_CLK       14

typedef struct paleta{
  char x;
  int y;
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

paleta_t paleta_izq = {1,2};  //Inicializacion de las paletas y la pelota

paleta_t paleta_der = {14,2};

pelota_t pelota = {7,4,1,1} ;

char vel_paleta_der = 0;
char vel_paleta_izq = 0;

/******************************
 * Declaraciones de Funciones
 */

 void inicializar();

/* * * * * * * * * * * * * * * * *
 *  FUNCIONES PRINCIPALES
 */
 
void setup() {

  //Apaga todos los LEDS
  for(int index=0 ; index < lcl.getDeviceCount() ; index++) {
      lcl.shutdown(index,false);

  //Inicializa variable de control de matriz
  lcl.clearDisplay(PRIMERA_MATRIZ); // 0 para la primer matriz, uno para la segunda  
  lcl.clearDisplay(SEGUNDA_MATRIZ);

  inicializar();

  //Inicializacion de pines de botones
 const char pines = {PIN_BOTON_IZQ_ARRIBA,PIN_BOTON_IZQ_ABAJO,PIN_BOTON_DER_ARRIBA,PIN_BOTON_DER_ABAJO}; // No compila
  for(char i = 0; i <4;i++){
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
  if(nueva_y >= POSICION_PALETA_MINIMA and nueva_y < POSICION_FILA_MAXIMA){
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

bool colisionConPaleta(){
  bool cond1 = pelota.x + pelota.vel_x == paleta_izq.x;  
  bool cond2 = (pelota.y += pelota.vel_y >= paleta_izq.y) and (pelota.y += pelota.vel_y < paleta_izq.y + LARGO_PALETA);
  bool cond3 = pelota.x + pelota.vel_x == paleta_der.x;
  bool cond4 = (pelota.y += pelota.vel_y >= paleta_der.y) and (pelota.y += pelota.vel_y < paleta_der.y + LARGO_PALETA);

  return (cond1 and cond2) or ( cond3 and cond4);
}
  
void moverPelota(){
  int x_anterior = pelota.x;
  int y_anterior = pelota.y;  

  if (pelota.y + pelota.vel_y > POSICION_FILA_MAXIMA or pelota.y + pelota.vel_y < 0){
    pelota.vel_y *= -1;
  }
  pelota.y += pelota.vel_y;
  if (colisionConPaleta()){
    pelota.vel_x *= -1;
  }
  pelota.x += pelota.vel_x;

  if (pelota.x == 0 or pelota.x == 15){
    reiniciar();
  }
  actualizarPelota(x_anterior,y_anterior);
  
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
