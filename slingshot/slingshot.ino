// Individual Project
// Arcade Slingshot
// Vincent Zhu
#include <LiquidCrystal.h>
#include <math.h>

// var for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// var for 7-segment
const int dataPin = 10;  // blue wire to 74HC595 pin 
const int latchPin = 8; // green to 74HC595 pin 
const int clockPin = 13; // yellow to 74HC595 pin 
const char common = 'c';    // common cathode
bool decPt = true;  // decimal point display flag
 

// var for joystick
const int inx = A0;
const int iny = A1;
const int inpressed = 7;
int x = 0;
int y = 0;
int initx = 0;
int inity = 0;
int notpressed = 0;

int deltax = 0;
int deltay = 0;
double rad, angle;

String X = "";
String Y = "";

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

byte matrix[][5][8] = {
    {{B10000, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B01000, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00100, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00010, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00001, B00000, B00000, B00000, B00000, B00000, B00000, B00000}},
    {{B00000, B10000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B01000, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00100, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00010, B00000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00001, B00000, B00000, B00000, B00000, B00000, B00000}},
    {{B00000, B00000, B10000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B01000, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00100, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00010, B00000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00001, B00000, B00000, B00000, B00000, B00000}},
    {{B00000, B00000, B00000, B10000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B01000, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00100, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00010, B00000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00001, B00000, B00000, B00000, B00000}},
    {{B00000, B00000, B00000, B00000, B10000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B01000, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00100, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00010, B00000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00001, B00000, B00000, B00000}},
    {{B00000, B00000, B00000, B00000, B00000, B10000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B01000, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00100, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00010, B00000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00001, B00000, B00000}},
    {{B00000, B00000, B00000, B00000, B00000, B00000, B10000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B01000, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00100, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00010, B00000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00000}},
    {{B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B01000},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00100},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00010},
     {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00001}}};

int row, col, lrow, lcol, mx, my, trow, tcol, tx, ty, drow, dcol;
int lastx, lasty, lastrow, lastcol;
int nextx, nexty, nx, ny;
float xmap, ymap, lastmap, l;
float lastv = -1;
int now, loopcount;
int score, currentscore;
boolean hit;

void setup()
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  pinMode(inx, INPUT);
  pinMode(iny, INPUT);
  pinMode(inpressed, INPUT_PULLUP);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  
  hit = true;
  score = 0;

  // wait for the press to start the game
  lcd.print("Press joystick");
  lcd.setCursor(0, 1);
  lcd.print("to start/pause!");
  
  while (digitalRead(inpressed))
  {
  }
  
  lcd.clear();
  lcd.print("Press to restart!");
  lcd.setCursor(0, 1);
  lcd.print("Good Luck!");
  delay(2000);
}

void mapping(int xx, int yy)
{ // map the input into new range
  ymap = map(xx, 512, 1023, 0, 80);
  ymap = constrain(ymap, 0, 80);
  xmap = map(yy, 0, 1023, -7, 8);
  xmap = constrain(xmap, -7, 8);
  if (xmap == 0)
  {
    l = 0;
  }
  else
  {
    l = ymap / xmap;
  }
}

void lcdposition(int lx, int ly)
{ // calculate the new location of the ball
  if (lx <= 0)
  {
    lcol = 0;
  }
  else if (lx > 0)
  {
    lcol = 1;
  }
  lrow = ly / 5;
  mx = ly % 5;
  my = abs(lx);

  if (lcol == 0)
  {
    my = 7 - my;
  }
  lcd.createChar(1, matrix[mx][my]);

  // create the byte for displaying the ball
  byte ball[8];

  // prevent the target from being cleared
  if (lrow == trow && lcol == tcol)
  {
    for (int j = 0; j < 8; j++)
    {
      ball[j] = matrix[my][mx][j] | matrix[tx][ty][j] | matrix[tx + 1][ty][j] | matrix[tx][ty + 1][j] | matrix[tx + 1][ty + 1][j];
    }
  }
  else
  {
    for (int j = 0; j < 8; j++)
    {
      ball[j] = matrix[my][mx][j];
    }
  }
  lcd.createChar(2, ball);

  if (lrow <= 2)
  {
    lrow = 0;
    lcol = 0;
    byte iball[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B10000,
    };
    lcd.createChar(2, iball);
  }
  lcd.setCursor(lrow, lcol);
  lcd.write(2);
}

void clearball()
{ // erase the last ball
  byte empty[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
  };
  if (lrow == trow && lcol == tcol)
  {
    lcd.setCursor(lrow, lcol);
    lcd.write(4);
  }
  else
  {
    lcd.createChar(3, empty);
    lcd.setCursor(lrow, lcol);
    lcd.write(3);
  }
}

void drawtarget()
{ // draw a random target
  trow = random(4, 17);
  tcol = random(0, 2);
  tx = random(0, 8);
  ty = random(0, 5);
  byte target[8];
  for (int j = 0; j < 8; j++)
  {
    target[j] = matrix[tx][ty][j] | matrix[tx + 1][ty][j] |
                matrix[tx][ty + 1][j] |
                matrix[tx + 1][ty + 1][j];
  }
  lcd.createChar(4, target);
  lcd.setCursor(trow, tcol);
  lcd.write(4);
}

void cleartarget()
{ // erace the last target
  byte empty[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
  };
  lcd.createChar(5, empty);
  lcd.setCursor(trow, tcol);
  lcd.write(5);
}

void fly(int xx, int yy, int rrow, int ccol)
{ // let the ball fly
  int ix = 6;
  int iy = 0;
  float slope;
  int dx = ccol * 8 + xx;
  int dy = rrow * 5 + yy;
  byte next[8];

  // find the next position
  slope = (float)(dx - ix) / (float)(dy - iy);
  nexty = iy;
  while (nexty < 79)
  {
    byte empty[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
    };

    // clear the last position
    if (drow == trow && dcol == tcol)
    {
      lcd.setCursor(drow, dcol);
      lcd.write(4);
    }
    else
    {
      lcd.createChar(5, empty);
      lcd.setCursor(drow, dcol);
      lcd.write(5);
    }

    nextx = (int)(slope * nexty) + ix;

    drow = nexty / 5;
    dcol = nextx / 8;

    nx = nextx % 8;
    ny = nexty % 5;

    // prevent the target from being cleared
    if (drow == trow && dcol == tcol)
    {
      for (int j = 0; j < 8; j++)
      {
        next[j] = matrix[nx][ny][j] | matrix[tx][ty][j] | matrix[tx + 1][ty][j] | matrix[tx][ty + 1][j] | matrix[tx + 1][ty + 1][j];
      }
    }
    else
    {
      for (int j = 0; j < 8; j++)
      {
        next[j] = matrix[nx][ny][j];
      }
    }

    lcd.createChar(6, next);
    lcd.setCursor(drow, dcol);
    lcd.write(6);
    nexty = nexty + 1;
    delay(50);

    // check if the target is hit
    check();
    if (hit)
    {
      break;
    }
    // cehck if reach the end point
    if (drow == rrow && dcol == ccol)
    {
      if (nx == xx && ny == yy)
      {
        break;
      }
    }
  }
}

void check()
{ // check if the target is hit
  if (drow == trow && dcol == tcol)
  {
    if ((tx <= nx) && (nx <= tx + 1) && (ty <= ny) && (ny <= ty))
    {
      Serial.println("hit!!!");
      hit = true;
      score = score + 1;
      cleartarget();
      drawtarget();
    }
  }
}

void loop()
{
  // read input
  x = analogRead(inx);                 // reading x value [range 0 -> 1023] from left to right
  y = analogRead(iny);                 // reading y value [range 0 -> 1023] from up to down
  notpressed = digitalRead(inpressed); // reading button state: 1 = not pressed, 0 = pressed

  // restart the game
  if (!notpressed && (millis() > 3000))
  {
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      cleartarget();
      drawtarget();
      score = 0;
    }
    lastDebounceTime = millis();
  }

  // record the initial position
  if (millis() < 50)
  {
    initx = x;
    inity = y;
  }

  // calculate for joystick
  deltax = x - initx;
  deltay = y - inity;
  rad = atan2(deltay, deltax);
  angle = rad * 57.3;

  // clean the startup screen
  if (loopcount == 0)
  {
    lcd.clear();
    loopcount += 1;
  }

  // draw a new target if hit
  if (hit)
  {
    cleartarget();
    drawtarget();
    hit = false;
  }

  // display score
  byte scoreBits = toBits(score);
  updateDisplay(scoreBits);

  // uncomment for a visible ball
  clearball();
  mapping(x, y);
  lcdposition(xmap, ymap);

  // check if user release the joystick
  if ((ymap < 10.00) && (lastmap - ymap > 20))
  {
    Serial.println("Release!!!!!!!!!!!!!!!!!  ");
    Serial.print(lastmap);
    Serial.print(" - ");
    Serial.println(ymap);
    Serial.println(lastx);
    Serial.println(lasty);
    Serial.println(lastrow);
    Serial.println(lastcol);
    fly(lastx, lasty, lastrow, lastcol);
  }
  lastx = my;
  lasty = mx;
  lastrow = lrow;
  lastcol = lcol;
  lastmap = ymap;
  delay(30);
}

void updateDisplay(byte eightBits) {
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
}

byte toBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B11111100;
      break;
    case 1:
      return B01100000;
      break;
    case 2:
      return B11011010;
      break;
    case 3:
      return B11110010;
      break;
    case 4:
      return B01100110;
      break;
    case 5:
      return B10110110;
      break;
    case 6:
      return B10111110;
      break;
    case 7:
      return B11100000;
      break;
    case 8:
      return B11111110;
      break;
    case 9:
      return B11110110;
      break;
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;   
  }
}
