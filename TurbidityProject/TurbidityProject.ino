#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // 0x27: I2C address, 20: columns, 4: rows
 
const int analogInPin = A0;
float inputVoltage;
int samples = 600;
float ntu;  // Nephelometric Turbidity Units

void setup() {
  Serial.begin(9600);
  pinMode(analogInPin, INPUT);
  
  pinMode(2, OUTPUT); // GREEN
  pinMode(3, OUTPUT); // YELLOW
  pinMode(4, OUTPUT); // RED
  
  lcd.init();         // Initializes the LCD
  lcd.backlight();    // Activates backlight     
  lcd.home();  
}

void loop() {
    inputVoltage = 0;

    // Averages 600 samples
    for(int i = 0; i < samples; i++) {
        inputVoltage += (float)analogRead(analogInPin) * (5.0 / 1023.0);
    }
    
    inputVoltage = inputVoltage/samples;     
    inputVoltage = decRound(inputVoltage, 2);

    // Calculates NTU value
    if(inputVoltage < 2.5){
      ntu = 3000;
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
    } else if(inputVoltage >= 2.5 && inputVoltage < 3.8) {
      ntu = -1120.4 * square(inputVoltage) + 5742.3 * inputVoltage - 4352.9;
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
    } else {
      ntu = -1120.4 * square(inputVoltage) + 5742.3 * inputVoltage - 4352.9;
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW); 
    }

    // For debugging purposes
    Serial.print("Voltage: ");
    Serial.println(inputVoltage);
    Serial.print("NTU: ");
    Serial.println(ntu);
    Serial.print("\n");

    // Prints Voltage to LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(inputVoltage);
    lcd.print(" V");

    // Prints NTU to LCD
    lcd.setCursor(0,1);
    lcd.print(ntu);
    lcd.print(" NTU");
    delay(10);
}

float decRound(float input, int decPlace) {
  float multiplier = powf(10.0f, decPlace);
  input = roundf(input * multiplier) / multiplier;
  
  return input;
}
