#include "LedControl.h"


/* * * * * * * * * * * * * * * * *
 *  CONSTANTES Y TIPOS DE DATOS
 */


#define PRIMERA_MATRIZ 0
#define SEGUNDA_MATRIZ 1
#define POSICION_FILA_MAXIMA 7
#define POSICION_PALETA_MINIMA 0
#define POSICION_PALETA_MAXIMA 6
#define LARGO_PALETA 3



#define PIN_BOTON_IZQ_ARRIBA 4
#define PIN_BOTON_IZQ_ABAJO  5
#define PIN_BOTON_DER_ARRIBA 6
#define PIN_BOTON_DER_ABAJO  7



#define PIN_MATRIZ_DATA_IN   10
#define PIN_MATRIZ_CLK       11
#define PIN_MATRIZ_LOAD      12



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

pelota_t pelota = {7,rand() % 8,-1,1} ;

int vel_paleta_der = 0;

int vel_paleta_izq = 0;




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
  }    


  //Inicializa variable de control de matriz

  lcl.clearDisplay(PRIMERA_MATRIZ); // 0 para la primer matriz, uno para la segunda  

  lcl.clearDisplay(SEGUNDA_MATRIZ);

  inicializar();



  //Inicializacion de pines de botones



 const char pines[] = {PIN_BOTON_IZQ_ARRIBA,PIN_BOTON_IZQ_ABAJO,PIN_BOTON_DER_ARRIBA,PIN_BOTON_DER_ABAJO};

  for(char i = 0; i <4;i++){
      pinMode(pines[i], INPUT_PULLUP);
  }

}


void loop() {

 administrarEntrada();

 moverPaleta(&paleta_izq,vel_paleta_izq);

 moverPaleta(&paleta_der,vel_paleta_der);

 gestionarCondicionesDeVictoria();

 delay(200);

}







/* * * * * * * * * * * * * * * * *
 *  FUNCIONES SECUNDARIAS
 */


void moverPaleta(paleta_t* paleta,int vel){
 
  int nueva_y = paleta->y + vel;
  dibujar(paleta->x,nueva_y,1);
  int led_a_apagar;
  int led_a_prender;

  if(nueva_y >= POSICION_PALETA_MINIMA and nueva_y < POSICION_PALETA_MAXIMA){

    if(nueva_y > paleta->y){

      led_a_apagar = nueva_y - 1;
      led_a_prender = nueva_y + 2;

    } else {

      led_a_apagar = nueva_y + 3;
      led_a_prender = nueva_y;
     
    }

    paleta->y = nueva_y;
    actualizarPaleta(led_a_apagar,paleta,led_a_prender); 

  }

}



void dibujar(int x, int y, int estado){

  int matriz = PRIMERA_MATRIZ; 

  if (x > POSICION_FILA_MAXIMA) {

    matriz = SEGUNDA_MATRIZ;
    x -= POSICION_FILA_MAXIMA + 1;

  }

  lcl.setLed(matriz,x,y,estado);

}



void administrarEntrada(){

  vel_paleta_der =  !digitalRead(PIN_BOTON_DER_ARRIBA) - !digitalRead(PIN_BOTON_DER_ABAJO);
  vel_paleta_izq =  !digitalRead(PIN_BOTON_IZQ_ARRIBA) - !digitalRead(PIN_BOTON_IZQ_ABAJO);
  moverPelota();

}



void actualizarPaleta(int posicion_a_apagar,paleta_t* paleta,int led_a_prender){

    dibujar(paleta->x,led_a_prender,1);
    dibujar(paleta->x,posicion_a_apagar,0);

}




void actualizarPelota(int x_anterior,int y_anterior){

  dibujar(x_anterior,y_anterior,0);
  dibujar(pelota.x,pelota.y,1);

}




bool colisionConPaleta(){

  bool cond1 = pelota.x + pelota.vel_x == paleta_izq.x;  

  bool cond2 = (pelota.y >= paleta_izq.y + pelota.vel_y) and (pelota.y < paleta_izq.y + LARGO_PALETA + pelota.vel_y);

  bool cond3 = pelota.x + pelota.vel_x == paleta_der.x;

  bool cond4 = (pelota.y >= paleta_der.y + pelota.vel_y) and (pelota.y < paleta_der.y + LARGO_PALETA + pelota.vel_y);


  return (cond1 and cond2) or ( cond3 and cond4);

}



void reiniciar(){

  lcl.clearDisplay(PRIMERA_MATRIZ); // 0 para la primer matriz, uno para la segunda  
  lcl.clearDisplay(SEGUNDA_MATRIZ);

  pelota.x = 7;
  pelota.y = rand() % 8;
  pelota.vel_x = 2 * (rand() % 2) -1;
  pelota.vel_y = 2 * (rand() % 2) -1;

  inicializar();

}



  
void moverPelota(){

  int x_anterior = pelota.x;
  int y_anterior = pelota.y;  

  if ((pelota.y + pelota.vel_y) > POSICION_FILA_MAXIMA or (pelota.y + pelota.vel_y) < 0){

    pelota.vel_y *= -1;

  }

  pelota.y += pelota.vel_y;

  if (colisionConPaleta()){

    pelota.vel_x *= -1;

  }

  pelota.x += pelota.vel_x;

  actualizarPelota(x_anterior,y_anterior);

  if (pelota.x == 0 or pelota.x == 15){

    reiniciar();

  }

}




void gestionarCondicionesDeVictoria(){



}




void dibujarPaleta(paleta_t paleta){

  dibujar(paleta.x,paleta.y,1);
  dibujar(paleta.x,paleta.y+1,1);
  dibujar(paleta.x,paleta.y+2,1);

}




void inicializar(){

  dibujarPaleta(paleta_izq);
  dibujarPaleta(paleta_der);
  dibujar(pelota.x,pelota.y,1);

}
