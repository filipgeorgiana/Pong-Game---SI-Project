#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define BALL_R 4
#define BAR_HEIGHT 15
#define STEP 2
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int xBall = display.width() / 2;  //x coordinate of the ball
int yBall = display.height() / 2; //y coordinate of the ball  -: at the beginning, the ball is in the center of the LCD
int dx = 1;  //movement on the x axis 
int dy = 1;  //movement on the y axis 
int xCoord=display.width()-1; //pozitia pe axa x a barii playerului 2

//bar player1

int up_Player1 = display.height() / 4;  //small nr -> up; the upper end of the bar
int bottom_Player1 = display.height() / 4 + BAR_HEIGHT; //big nr:->down; the bottom end of the bar

//bar player 2
int up_Player2 = display.height() / 4;  //small nr -> up; the upper end of the bar
int bottom_Player2 = display.height() / 4 + BAR_HEIGHT; //big nr:->down; the bottom end of the bar

int count_fails_Player1 = 0; //nr of fails  Player1
int count_fails_Player2 = 0; //nr of fails Playe2

//buttons player 1
const int buttonPin_up_Player1 = 10; //pin button 1
int buttonState_up_Player1 = 0;

const int buttonPin_down_Player1 = 11; //pin button 2
int buttonState_down_Player1 = 0;


//buttons player 2
const int buttonPin_up_Player2 = 12; //pin button 1
int buttonState_up_Player2 = 0;

const int buttonPin_down_Player2 = 13; //pin button 2
int buttonState_down_Player2 = 0;

void setup() {
  //set the data rate in bits per second (baud)
  Serial.begin(9600);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  pinMode(buttonPin_up_Player1, INPUT); //setup for buttons player 1
  pinMode(buttonPin_down_Player1, INPUT);

  pinMode(buttonPin_up_Player2, INPUT); //setup for buttons player 2
  pinMode(buttonPin_down_Player2, INPUT);


}

void gameOver(String s){
   while (1)
        {
          display.display(); // show splashscreen
          delay(2000);
          display.clearDisplay();
          display.setCursor(10, display.height() / 2 - 10); //to write in the middle of the screen
          display.print(s); // s -> which player won ???
          display.println(" won");
          display.setCursor(display.width() / 2 - 10, display.height() / 2);  
          xBall = display.width() / 2;
          yBall = display.height() / 2;
          display.fillCircle(0, 0, BALL_R,  WHITE); //erase the ball

          display.drawLine(0, up_Player1, 0, bottom_Player1, WHITE); //erase the bars
          display.drawLine(xCoord, up_Player1, xCoord, bottom_Player1, WHITE);


        }
  
}

void loop() {

  
  display.fillCircle(xBall, yBall, BALL_R, WHITE);  //draw the white ball, basically we delete the previous position of the ball
  xBall += dx; //we move the ball
  yBall += dy;
  delay(50);
  display.fillCircle(xBall, yBall, BALL_R, BLACK); //draw the black ball at the new position
  
  display.drawLine(0, up_Player1, 0, bottom_Player1, BLACK); //draw the bar for PLAYER 1
  display.drawLine(xCoord, up_Player2, xCoord, bottom_Player2, BLACK); //draw the bar for PLAYER 2
  
  display.display();

  
  //if the ball reaches the horizontal borders of the lcd, it keeps moving
  
  if (xBall >= display.width() - BALL_R || xBall <= 0) {
    dx = -dx;
  }
  if (yBall >= display.height() - BALL_R || yBall <= 0) {
    dy = -dy;
  }
  

 /*
  delay(50);
   Serial.print(xBall);
    Serial.print(",");
    Serial.println(yBall);
 */
  

 //bar movements depending on which is pushed by PLAYER 1
 
  buttonState_up_Player1 = digitalRead(buttonPin_up_Player1);
  if (buttonState_up_Player1 == HIGH) {
    //  Serial.println("apasat bara sus");
    display.drawLine(0, up_Player1, 0, bottom_Player1, WHITE);
    if (up_Player1 - STEP > 0) {
      up_Player1 -= STEP;
      bottom_Player1 -= STEP;
      display.drawLine(0, up_Player1, 0, bottom_Player1, BLACK);
      display.display();
    }
  }

  buttonState_down_Player1 = digitalRead(buttonPin_down_Player1);
  if (buttonState_down_Player1 == HIGH) {
    //   Serial.println("apasat bara jos");
    display.drawLine(0, up_Player1, 0, bottom_Player1, WHITE);
    if (bottom_Player1 + STEP < display.height() ) {
      up_Player1 += STEP;
      bottom_Player1 += STEP;
      display.drawLine(0, up_Player1, 0, bottom_Player1, BLACK);
      display.display();
    }
  }

  

 //bar movements depending on which is pushed by PLAYER 1
 
  buttonState_up_Player2 = digitalRead(buttonPin_up_Player2);
  if (buttonState_up_Player2 == HIGH) {
    //  Serial.println("apasat bara sus");
    display.drawLine(xCoord, up_Player2, xCoord, bottom_Player2, WHITE);
    if (up_Player2 - STEP > 0) {
      up_Player2 -= STEP;
      bottom_Player2 -= STEP;
      display.drawLine(xCoord, up_Player2, xCoord, bottom_Player2, BLACK);
      display.display();
    }
  }

  buttonState_down_Player2 = digitalRead(buttonPin_down_Player2);
  if (buttonState_down_Player2 == HIGH) {
    //   Serial.println("apasat bara jos");
    display.drawLine(xCoord, up_Player2, xCoord, bottom_Player2, WHITE);
    if (bottom_Player2 + STEP < display.height() ) {
      up_Player2 += STEP;
      bottom_Player2 += STEP;
      display.drawLine(xCoord, up_Player2, xCoord, bottom_Player2, BLACK);
      display.display();
    }
  }


  if (xBall == 0)
  {
    if (up_Player1 < yBall && bottom_Player1 > yBall) //if the player 1 hits the ball, it keeps moving
    {
      dx = -dx;
      dy = -dy;
    }
    else  //if not, he has 2 more chance to win
    { count_fails_Player1++;
      display.fillCircle(xBall, yBall, BALL_R,  WHITE);
      xBall = display.width() / 2;
      yBall = display.height() / 2;
      if (count_fails_Player1 == 3 && count_fails_Player1>count_fails_Player2)
       gameOver("Player2");
        

    }

  }

  
  if (xBall == (xCoord - BALL_R))
  {
    if (up_Player2 < yBall && bottom_Player2 > yBall)  //if the player 2 hits the ball, it keeps moving
    {
      dx = -dx;
      dy = -dy;
    }
    else  ///if not, he has 2 more chance to win
    { count_fails_Player2++;
      display.fillCircle(xBall, yBall, BALL_R,  WHITE);
      xBall = display.width() / 2;
      yBall = display.height() / 2;
      if (count_fails_Player2 == 3 && count_fails_Player2>count_fails_Player1)
       gameOver("Player 1");

    }

  }
    
}
