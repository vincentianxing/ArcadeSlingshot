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

int row, col, lrow, lcol, mx, my;
float xmap, ymap, l;

void setup()
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  pinMode(inx, INPUT);
  pinMode(iny, INPUT);
  pinMode(inpressed, INPUT_PULLUP);
  Serial.begin(9600);

  // lcd.print("X: ");
  // lcd.setCursor(0, 1);
  // lcd.print("Y: ");
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

void lcdposition(int lx, int ly)
{
  if (lx <= 0){ //TODO
    lcol = 0;
  } 
  else if (lx > 0) {
    lcol = 1;
  }
  lrow = ly / 5;
  mx = ly % 5;
  my = abs(lx);
  lcd.createChar(1, matrix[mx][my]);
  byte ball [8];
  for (int j = 0; j < 8; j++){
    ball[j] = matrix[mx][my][j] | matrix[mx+1][my][j] | matrix[mx][my+1][j] | matrix[mx+1][my+1][j];
  }
  lcd.createChar(2, ball);
  lcd.setCursor(lrow, lcol);
  lcd.write(2);
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

  lcd.clear();

  mapping(x, y);
  lcdposition(xmap, ymap);

  // lcd.setCursor(4,0);
  // lcd.print(xmap);
  // lcd.setCursor(4,1);
  // lcd.print(ymap);

//  lcd.createChar(1, matrix[7][4]);
//  lcd.setCursor(0,0);
//  lcd.write(byte(1));
  

  Serial.print("X: ");
  Serial.println(mx);
  Serial.print("Y: ");
  Serial.println(my);
  // Serial.print("Not pressed: ");
  // Serial.println(notpressed);
  // Serial.print("Angle: ");
  // Serial.println(angle);
  // Serial.print("xmap: ");
  // Serial.println(xmap);
  // Serial.print("ymap: ");
  // Serial.println(ymap);
  // Serial.print("l: ");
  // Serial.println(l);
  delay(200);
}
