#include <Wire.h>
#include <LCD_I2C.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LCD_I2C lcd(0x27, 16, 2);  // LCD 16x2

// Motoare
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;
int motorSpeedPin1 = 9;
int motorSpeedPin2 = 10;

enum State {STOPPED, FORWARD, REVERSE};
State currentState = STOPPED;

// LCD switching
unsigned long lastSwitchTime = 0;
bool showPergolaStatus = true;
String pergolaStatus = "Oprita";

// Praguri logica inteligenta
const int humidityThreshold = 60; // %
const int temperatureThreshold = 15; // grade C
const int rainSensorThreshold = 700; // analog

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motorSpeedPin1, OUTPUT);
  pinMode(motorSpeedPin2, OUTPUT);
  pinMode(11, OUTPUT);  // LED verde
  pinMode(12, OUTPUT);  // LED rosu


  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Stare pergola:");
  lcd.setCursor(0, 1);
  lcd.print("Oprita");
}

void loop() {
  // Citiri senzori
  int rainValue = analogRead(A3);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Pl: "); Serial.print(rainValue);
  Serial.print(" | Temp: "); Serial.print(temp);
  Serial.print(" | Umi: "); Serial.println(hum);

  // Verificare conditii de extindere
  bool ploua = rainValue < rainSensorThreshold;
  bool umiditateMare = (!isnan(hum) && hum > humidityThreshold);
  bool temperaturaMica = (!isnan(temp) && temp < temperatureThreshold);
  bool conditieExtindere = ploua || umiditateMare || temperaturaMica;

  // LOGICA DECIZIE MOTOR
  if (conditieExtindere && pergolaStatus != "Extinsa") {
    currentState = FORWARD;

    if (!ploua && (umiditateMare || temperaturaMica)) {
      // Afisare predicție
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Predictie ploaie");
      lcd.setCursor(0, 1);
      lcd.print("Extindere...");
      delay(1500);  // scurt mesaj informativ
    } else {
      // Afisare animata normala
      animateLCD("Extindere");
    }

    driveMotors(FORWARD);
    delay(700); // timp extindere
    stopMotors();
    pergolaStatus = "Extinsa";
    digitalWrite(11, LOW);   // verde off
    digitalWrite(12, HIGH);  // rosu on
  } else if (!conditieExtindere && pergolaStatus != "Retrasa") {
    currentState = REVERSE;
    animateLCD("Retragere");
    driveMotors(REVERSE);
    delay(900); // timp retragere
    stopMotors();
    pergolaStatus = "Retrasa";
    digitalWrite(11, HIGH);   // verde off
    digitalWrite(12, LOW);  // rosu on
  }

  updateLCD(temp, hum);
  delay(100);
}

void driveMotors(State direction) {
  analogWrite(motorSpeedPin1, 50);  
  analogWrite(motorSpeedPin2, 50);

  if (direction == FORWARD) {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
  } else if (direction == REVERSE) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  }
}

void stopMotors() {
  analogWrite(motorSpeedPin1, 0);
  analogWrite(motorSpeedPin2, 0);
}

void animateLCD(String actiune) {
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(0, 1);
    lcd.print(actiune + ".  ");
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print(actiune + ".. ");
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print(actiune + "...");
    delay(200);
  }
}

void updateLCD(float temp, float hum) {
  unsigned long now = millis();
  if (now - lastSwitchTime > 3000) {
    lastSwitchTime = now;
    showPergolaStatus = !showPergolaStatus;

    lcd.clear();
    if (showPergolaStatus) {
      lcd.setCursor(0, 0);
      lcd.print("Stare pergola:");
      lcd.setCursor(0, 1);
      lcd.print(pergolaStatus + "        ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      if (isnan(temp)) lcd.print("Err");
      else lcd.print(temp, 1), lcd.print(" C");

      lcd.setCursor(0, 1);
      lcd.print("Umi:  ");
      if (isnan(hum)) lcd.print("Err");
      else lcd.print((int)hum), lcd.print(" %");
    }
  }
}
