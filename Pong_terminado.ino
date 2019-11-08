//Basado en el codigo de Xonay Labs.
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Definicion de pines
#define OLED_RESET 4
#define BEEPER 3
#define CONTROL_A A0
#define CONTROL_B A1

//Deficion de graficos
#define FONT_SIZE 2
#define SCREEN_WIDTH 127  //tamaño real menos 1, ya que el sistema de coordenadas se inicia con 0
#define SCREEN_HEIGHT 63  //tamaño real menos 1, ya que el sistema de coordenadas se inicia con 0
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 10
#define PADDLE_PADDING 10
#define BALL_SIZE 3
#define SCORE_PADDING 10

#define EFFECT_SPEED 0.5
#define MIN_Y_SPEED 0.5
#define MAX_Y_SPEED 2

//Definicion de variables
Adafruit_SSD1306 display(OLED_RESET);

int paddleLocationA = 0;
int paddleLocationB = 0;

float ballX = SCREEN_WIDTH/2;
float ballY = SCREEN_HEIGHT/2;
float ballSpeedX = 2;
float ballSpeedY = 1;

int lastPaddleLocationA = 0;
int lastPaddleLocationB = 0;

int scoreA = 0;
int scoreB = 0;

void(* resetFunc) (void) = 0; //funcion de reset por software

//Setup 
void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // inicializar display oled con la direccion I2C 0x3C 
  display.clearDisplay();   // limpia buffer y pantalla
  display.display();   
  display.setTextWrap(false);

    splash();

  display.setTextColor(WHITE);
  display.setTextSize(FONT_SIZE);
  display.clearDisplay(); 
}

//Pantalla de inicio
void splash()
{
  soundStart();
  display.clearDisplay(); 

  display.setTextColor(WHITE);
  centerPrint("PONG",0,3);
  centerPrint("Por: Daniel F.R.",24,1);
  centerPrint("Visitanos en:",33,1);
  centerPrint("dafrelectronics.com",42,1);

  display.fillRect(0,SCREEN_HEIGHT-10,SCREEN_WIDTH,10,WHITE);
  display.setTextColor(BLACK);
  centerPrint(" Gira para empezar! ",SCREEN_HEIGHT-9,1);

  display.display();

  int controlA = analogRead(CONTROL_A);
  int controlB = analogRead(CONTROL_B);

  while (abs(controlA - analogRead(CONTROL_A) + controlB - analogRead(CONTROL_B)) < 10) {
    // se muestra mientras el cambio entre ambos potenciometros sea menor de 5 
    
  }

  soundStart();
} 

//Ciclo
void loop()
{
  calculateMovement();
  draw();
} 

void calculateMovement() 
{
  int controlA = analogRead(CONTROL_A);
  int controlB = analogRead(CONTROL_B);

  paddleLocationA = map(controlA, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
  paddleLocationB = map(controlB, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  int paddleSpeedA = paddleLocationA - lastPaddleLocationA;
  int paddleSpeedB = paddleLocationB - lastPaddleLocationB;

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  //rebote superior e inferior
  if (ballY >= SCREEN_HEIGHT - BALL_SIZE || ballY <= 0) {
    ballSpeedY *= -1;
    soundBounce();
  }

  //rebote paleta A
  if (ballX >= PADDLE_PADDING && ballX <= PADDLE_PADDING+BALL_SIZE && ballSpeedX < 0) {
    if (ballY > paddleLocationA - BALL_SIZE && ballY < paddleLocationA + PADDLE_HEIGHT) {
      soundBounce();
      ballSpeedX *= -1;

      addEffect(paddleSpeedA);
    }

  }

  //rebote paleta B
  if (ballX >= SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING-BALL_SIZE && ballX <= SCREEN_WIDTH-PADDLE_PADDING-BALL_SIZE && ballSpeedX > 0) {
    if (ballY > paddleLocationB - BALL_SIZE && ballY < paddleLocationB + PADDLE_HEIGHT) {
      soundBounce();
      ballSpeedX *= -1;

      addEffect(paddleSpeedB);
    }

  }

  //anotacion de puntos cuando se escapa de las paletas
  if (ballX >= SCREEN_WIDTH - BALL_SIZE || ballX <= 0) {
    if (ballSpeedX > 0) {
      scoreA++;
      ballX = SCREEN_WIDTH / 4;
    }
    if (ballSpeedX < 0) {
      scoreB++;
      ballX = SCREEN_WIDTH / 4 * 3;
    }
    
    soundPoint();   
    //Terminar juego por anotacion de puntos.
    if (scoreA >= 6) //gana jugador A al anotar 6 puntos
    {
      display.clearDisplay(); 

      display.setTextColor(WHITE);
      centerPrint("PONG",0,3);
      centerPrint("GANA JUGADOR A",24,1);
      centerPrint("Visitanos en:",33,1);
      centerPrint("dafrelectronics.com",42,1);

      display.fillRect(0,SCREEN_HEIGHT-10,SCREEN_WIDTH,10,WHITE);
      display.setTextColor(BLACK);
      centerPrint(" Espera un momento! ",SCREEN_HEIGHT-9,1);

      display.display();
      soundPoint();
      delay(8000);
      
      resetFunc(); //reiniciar arduino automaticamente
    }
    
    if (scoreB >= 6) //gana jugador B al anotar 6 puntos.
    {
      display.clearDisplay(); 

      display.setTextColor(WHITE);
      centerPrint("PONG",0,3);
      centerPrint("GANA JUGADOR B",24,1);
      centerPrint("Visitanos en:",33,1);
      centerPrint("dafrelectronics.com",42,1);

      display.fillRect(0,SCREEN_HEIGHT-10,SCREEN_WIDTH,10,WHITE);
      display.setTextColor(BLACK);
      centerPrint(" Espera un momento! ",SCREEN_HEIGHT-9,1);

      display.display();
      soundPoint();
      delay(8000);
      
      resetFunc(); //reiniciar arduino automaticamente
    }
  }

  //regresar paletas a su ubicacion anterior
  lastPaddleLocationA = paddleLocationA;
  lastPaddleLocationB = paddleLocationB;  
}

//dibujar graficos
void draw() 
{
  display.clearDisplay(); 

  //dibujar paleta A
  display.fillRect(PADDLE_PADDING,paddleLocationA,PADDLE_WIDTH,PADDLE_HEIGHT,WHITE);

  //dibujar paleta B
  display.fillRect(SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING,paddleLocationB,PADDLE_WIDTH,PADDLE_HEIGHT,WHITE);

  //dibujar linea punteada media cancha
  for (int i=0; i<SCREEN_HEIGHT; i+=4) {
    display.drawFastVLine(SCREEN_WIDTH/2, i, 2, WHITE);
  }

  //dibujar pelota
  display.fillRect(ballX,ballY,BALL_SIZE,BALL_SIZE,WHITE);

  //imprimir puntajes

  //hacia atrás guión puntuación A. Esto es un desastre, pero funciona ...;)
  int scoreAWidth = 5 * FONT_SIZE;
  if (scoreA > 9) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 99) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 999) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 9999) scoreAWidth += 6 * FONT_SIZE;

  display.setCursor(SCREEN_WIDTH/2 - SCORE_PADDING - scoreAWidth,0);
  display.print(scoreA);

  display.setCursor(SCREEN_WIDTH/2 + SCORE_PADDING+1,0); //+1 a causa de la linea punteada 
  display.print(scoreB);

  display.display();
}  

//efecto de la pelota
void addEffect(int paddleSpeed)
{
  float oldBallSpeedY = ballSpeedY;

  //añadir efecto a la bola cuando paleta se mueve mientras rebota .
  //para cada píxel de movimiento de la paleta , añadir o quitar velocidad en EFFECT_SPEED 
  for (int effect = 0; effect < abs(paddleSpeed); effect++) {
    if (paddleSpeed > 0) {
      ballSpeedY += EFFECT_SPEED;
    } else {
      ballSpeedY -= EFFECT_SPEED;
    }
  }

  //limite de velocidad minima
  if (ballSpeedY < MIN_Y_SPEED && ballSpeedY > -MIN_Y_SPEED) {
    if (ballSpeedY > 0) ballSpeedY = MIN_Y_SPEED;
    if (ballSpeedY < 0) ballSpeedY = -MIN_Y_SPEED;
    if (ballSpeedY == 0) ballSpeedY = oldBallSpeedY;
  }

  //limite de velocidad maxima
  if (ballSpeedY > MAX_Y_SPEED) ballSpeedY = MAX_Y_SPEED;
  if (ballSpeedY < -MAX_Y_SPEED) ballSpeedY = -MAX_Y_SPEED;
}

//iniciar sonido
void soundStart() 
{
  tone(BEEPER, 250);
  delay(100);
  tone(BEEPER, 500);
  delay(100);
  tone(BEEPER, 1000);
  delay(100);
  noTone(BEEPER);
}

//sonido rebote
void soundBounce() 
{
  tone(BEEPER, 500, 50);
}

//sonido anotacion
void soundPoint() 
{
  tone(BEEPER, 150, 150);
}

//imprimir en el centro
void centerPrint(char *text, int y, int size)
{
  display.setTextSize(size);
  display.setCursor(SCREEN_WIDTH/2 - ((strlen(text))*6*size)/2,y);
  display.print(text);
}


