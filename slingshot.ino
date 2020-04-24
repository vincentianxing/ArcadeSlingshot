// Individual Project
// Arcade Slingshot
// Vincent Zhu
#include <LiquidCrystal.h>
#include <math.h>

// var for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// var for joystick
const int inx = A0;
const int iny = A1;
const int inpressed = 7;
int x = 0;
int y = 0;
int notpressed = 0;

int deltax = 0;
int deltay = 0;
double rad, angle;

String X = "";
String Y = "";

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

int row, col;
float xmap, ymap, l;

void setup()
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  pinMode(inx, INPUT);
  pinMode(iny, INPUT);
  pinMode(inpressed, INPUT_PULLUP);
  Serial.begin(9600);

  lcd.print("X: ");
  lcd.setCursor(0, 1);
  lcd.print("Y: ");
}

void mapping(int xx, int yy)
{
  ymap = map(xx, 512, 1023, 0, 79);
  ymap = constrain(ymap, 0, 79);
  xmap = map(yy, 0, 1023, -7, 7);
  xmap = constrain(xmap, -7, 7);
  if (xmap == 0)
  {
    l = 0;
  }
  else
  {
    l = ymap / xmap;
  }
}

void loop()
{
  x = analogRead(inx);                 // reading x value [range 0 -> 1023] from left to right
  y = analogRead(iny);                 // reading y value [range 0 -> 1023] from up to down
  notpressed = digitalRead(inpressed); // reading button state: 1 = not pressed, 0 = pressed

  deltax = x - 513;
  deltay = y - 510;
  rad = atan2(deltay, deltax);
  angle = rad * 57.3;

  mapping(x, y);

  lcd.setCursor(4,0);
  lcd.print(xmap);
  lcd.setCursor(4,1);
  lcd.print(ymap);

  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Not pressed: ");
  Serial.println(notpressed);
  Serial.print("Angle: ");
  Serial.println(angle);
  Serial.print("xmap: ");
  Serial.println(xmap);
  Serial.print("ymap: ");
  Serial.println(ymap);
  Serial.print("l: ");
  Serial.println(l);
  delay(200);

  if ((x >= 500) && (x <= 600) && (y >= 500) && (y <= 600))
  {
    //middle
  }
  else
  {
    //move
  }

  if ((angle > -22.5) && (angle <= 22.5))
  {
    //up
  }
  else if ((angle > 22.5) && (angle <= 67.5))
  {
    //up-right
  }
  else if ((angle > 67.5) && (angle <= 112.5))
  {
    //right
  }
  else if ((angle > 112.5) && (angle <= 157.5))
  {
    //down-right
  }
  else if ((angle > 157.5) && (angle <= 180))
  {
    //down
  }
  else if ((angle > -180) && (angle <= -157.5))
  {
    //down
  }
  else if ((angle > -157.5) && (angle <= -112.5))
  {
    //down-left
  }
  else if ((angle > -112.5) && (angle <= -67.5))
  {
    //left
  }
  else if ((angle > -67.5) && (angle <= -22.5))
  {
    //up-left
  }
}
