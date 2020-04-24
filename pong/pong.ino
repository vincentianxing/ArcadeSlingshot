
/*
   Ping Pong on Arduino-16*2 LCD
   Author: Bobby Joseph Sebastian
   Created on:19-03-2019
   Features: Players are named as player A and player B.
             Initially, both players are given 10 scores each first to lose all 10 scores will lose the game.
             Uses custom characters to display game objects to enhance the gaming experience. The angle of reflection on the ball is not the same everywhere on the striker, this reduces invertibility of the ball movement.
             The speed of the ball increases when players lose score
*/

#include <LiquidCrystal.h>           // include the library code:
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins
byte strikerA1[8], strikerA2[8];     //custom charcters for dispalying strikerA, sometimes the striker is spread across two character so two custom characters are required
byte strikerB1[8], strikerB2[8];
byte ball[8]; //character for the ball
byte temp = B00000;

//game intialization
void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  lcd.setCursor(1, 1);
  lcd.print("PING PONG GAME");
  delay(2000);
  lcd.clear();
  lcd.print("Rule: Last one");
  lcd.setCursor(0, 1);
  lcd.print("standing wins ");
  delay(3000);
  lcd.clear();
  lcd.print("Player A");
  lcd.setCursor(8, 1);
  lcd.print("Player B");
  delay(2000);
  pinMode(8, INPUT);  // inputs button for strikerA upwards
  pinMode(9, INPUT);  //for strikerA downwards
  pinMode(10, INPUT); //inputs button for strikerB upwards
  pinMode(11, INPUT); //inputs button for strikerB downwards
}

int buttonState[4];                           // to take inputs from the buttons
bool prevState[4] = {true, true, true, true}; //to store previous state ,single click on the button will trigger only one movement of the striker(no continous triggering)
int i;
int t = 40, t1;               // variables for delay adjustment
int xi = 1, yi = 1;           //initial ball movement adding components
int x = 39, y = 8;            //initial position of the ball
int ly = 0, ry = 10;          //initial position of the strikers
int scoreA = 10, scoreB = 10; //initial scores

//function to display result
void displayResult()
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SCORE:");
  lcd.setCursor(5, 1);
  lcd.print(scoreA);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(8, 1);
  lcd.print(scoreB);
  delay(1000);
  if (scoreB == 0)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("PLAYER A WON");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.print("Press RESET to ");
    lcd.setCursor(0, 1);
    lcd.print("restart the Game");
    while (true)
      ;
  }
  if (scoreA == 0)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("PLAYER B WON");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.print("Press RESET to ");
    lcd.setCursor(0, 1);
    lcd.print("restart the Game");
    while (true)
      ; // waiting for restart
  }
}

//function to display ball,strikerA and strikerB (x,y) is the ball position,yA strikerA position,yB strikerB position
void plotter(int yA, int yB, int x, int y)
{
  //clearing screen data and setting custom characters to initial state
  lcd.clear();
  int i = 0;
  //reintializing custom characters for screen output
  while (i < 8)
  {
    strikerA1[i] = B00000;
    strikerA2[i] = B00000;
    strikerB1[i] = B00000;
    strikerB2[i] = B00000;
    ball[i] = B00000;
    i++;
  }

  //creating custom characters  for the ball
  if (x % 5 < 1)
    temp = B10000;
  else if (x % 5 < 2)
    temp = B01000;
  else if (x % 5 < 3)
    temp = B00100;
  else if (x % 5 < 4)
    temp = B00010;
  else
    temp = B00001;

  ball[y % 8] = temp;

  //creating custom characters  for striker A
  i = 0;
  while (i < 4)
  {
    if (yA > 7) //sometimes strike have to be split into two character
      strikerA2[yA % 8] = B10000;
    else
      strikerA1[yA % 8] = B10000;
    i++;
    yA++;
  }

  //creating custom characters  for striker B
  i = 0;
  while (i < 4)
  {
    if (yB > 7) //sometimes strike have to be split into two character
      strikerB2[yB % 8] = B00001;
    else
      strikerB1[yB % 8] = B00001;
    i++;
    yB++;
  }

  //displaying created custom characters
  lcd.createChar(1, strikerA1); //dispalying screen data
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.createChar(2, strikerA2);
  lcd.setCursor(15, 1);
  lcd.write(byte(2));
  lcd.createChar(3, strikerB1);
  lcd.setCursor(0, 0);
  lcd.write(byte(3));
  lcd.createChar(4, strikerB2);
  lcd.setCursor(0, 1);
  lcd.write(byte(4));
  lcd.createChar(0, ball);
  lcd.setCursor(x / 5, y / 8);
  lcd.write(byte(0));
}

void loop()
{

  //for bouncing
  if ((x + xi) < 5 || (x + xi) > 74)
    xi = xi * -1;
  x = x + xi;
  if ((y + yi) < 0 || (y + yi) > 15)
    yi = yi * -1;
  y = y + yi;
  plotter(ry, ly, x, y);
  // for data inputs
  i = 0;

  // taking inputs
  while (i < 4)
  {
    buttonState[i] = digitalRead(8 + i);
    if (buttonState[i] == HIGH && prevState[i] == true)
    {
      if (i == 0)
      {
        if (ly < 12)
          ly++;
      }
      else if (i == 1)
      {
        if (ly > 0)
          ly--;
      }
      else if (i == 2)
      {
        if (ry < 12)
          ry++;
      }
      else if (i == 3)
      {
        if (ry > 0)
          ry--;
      }
      prevState[i] = false;
    }
    else if (prevState[i] == false && buttonState[i] == LOW)
    {
      prevState[i] = true;
    }
    i++;
  }

  //for collision ditection on strikerA side
  if (x == 5)
  {
    if (ly - 1 > y || y > ly + 4)
    {
      scoreA--;
      displayResult();
      x = 39;
      y = 8;    //setting ball position
      xi *= -1; //reversing ball movement
      yi *= -1;
    }
    else
    { //changing ball movement according to point of contact on the strikerA (To avoid inevitability)
      if (ly - 1 == y || ly + 4 == y)
      {
        xi = 1;
        yi = 1;
      }
      else if (ly == y || ly + 3 == y)
      {
        xi = 1;
        yi = 2;
      }
      else
      {
        xi = 2;
        yi = 1;
      }
    }
  }

  //for collision ditection on strikerB side
  else if (x == 74)
  {
    if (ry - 1 > y || y > ry + 4)
    {
      scoreB--;
      displayResult();
      x = 39;
      y = 8;    //setting ball position
      xi *= -1; //reversing ball movement
      yi *= -1;
    }
    else
    { //changing ball movement according to point of contact on the strikerB (To avoid inevitability)
      if (ry - 1 == y || ry + 4 == y)
      {
        xi = 1;
        yi = 1;
      }
      else if (ry == y || ry + 3 == y)
      {
        xi = 1;
        yi = 2;
      }
      else
      {
        xi = 2;
        yi = 1;
      }
    }
  }

  delay(t + (scoreA + scoreB) * 3); //as players lose scores speed of the ball increases base delay is 100ms  and it could decrease upto 40ms ( another method is that to millis(),so after time passes speed of the ball increases)
}
