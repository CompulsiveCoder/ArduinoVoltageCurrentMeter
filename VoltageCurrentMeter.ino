#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define VT_PIN A0 // connect VT
#define AT_PIN A1 // connect AT

#define ARDUINO_WORK_VOLTAGE 5.0

void setup()
{
  Serial.begin(9600);

  lcd.init();

  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Starting meter");
  
  delay(500);
}

void loop() 
{
  int vt_temp = analogRead(VT_PIN);
  int at_temp = analogRead(AT_PIN);
  
  long result;
  
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  result = result / 1000;
  
  double voltage = vt_temp * (result / 1023.0) * 5;
  double current = at_temp * (result / 1023.0);
  
  Serial.print("A0: ");
  Serial.print(vt_temp);
  Serial.print(" / ");
  Serial.print("A1: ");
  Serial.print(at_temp);
  
  Serial.print(" ----- "); 
  
  Serial.print(voltage);
  Serial.print(" / ");
  Serial.println(current);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(voltage);
  lcd.print(" / ");
  lcd.println(current);
  
  delay(500);
}

