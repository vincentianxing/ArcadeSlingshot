#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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

void mapping(int x, int y)
{
  //  if ((x >= 500) && (x <= 600) && (y >= 500) && (y <= 600)) {
  //    return;
  //  }
  //  if (x <= 512) {
  //    return;
  //  }
  ymap = map(x, 512, 1023, 0, 79);
  xmap = map(y, 0, 1023, -7, 7);
  ymap = constrain(ymap, 0, 79);
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

void setup()
{
  Serial.begin(9600);
  lcd.createChar(1, matrix[0][0]);
  lcd.createChar(B01000, matrix[0][1]);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.write(1);
}

void loop()
{
  row = 0;
  col = 0;
  for (int i = 0; i < 16; i++)
  {
    row++;
    //    lcd.setCursor(row, col);
    //    lcd.write(1);
    //    delay(500);
  }
  mapping(768, 256);

  Serial.print("xmap: ");
  Serial.println(xmap);
  Serial.print("ymap: ");
  Serial.println(ymap);
  Serial.print("l: ");
  Serial.println(l);
  delay(1000);
}
