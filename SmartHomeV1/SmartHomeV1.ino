#include<DHT.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "7d42e638-dae4-431a-a12a-c25ad70768de"
#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
const char ssid[] = "Note 9";
const char pass[] = "12345678";

ERaTimer timer;
#define gas1 33
#define gas2 32
#define gas3 35
#define tmp1 4
#define tmp2 15
DHT dht1(tmp1, DHT11);
DHT dht2(tmp2, DHT11);
int gas1val, gas2val, gas3val;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(gas1, INPUT);
  pinMode(gas2, INPUT);
  pinMode(gas3, INPUT);
  dht1.begin();
  dht2.begin();
  ERa.begin(ssid, pass);
  timer.setInterval(1000L, timerEvent);
}
void GasVal() {
  gas1val = analogRead(gas1);
  gas2val = analogRead(gas2);
  gas3val = analogRead(gas3);
  gas1val = map(gas1val, 0, 4095, 0, 100);
  gas2val = map(gas2val, 0, 4095, 0, 100);
  gas3val = map(gas3val, 0, 4095, 0, 100);
}

// bool Gas(int gas1val, int gas2val, int gas3val) {
//   if(gas1val > 60 || gas2val > 60 || gas3val > 60) {
//     digitalWrite(buzzer, HIGH);
//   }
//   else {
//     sms = false;
//     digitalWrite(buzzer, LOW);
//   }
// }
int t1;
int t2;
void TmpVal() {
  t1 = dht1.readTemperature();
  t2 = dht2.readTemperature();
}

// bool Tmp(int t1, int t2) {
//   if(t1 > 45 || t2 > 45) {
//     digitalWrite(buzzer, HIGH);
//   }
//   else {
//     sms = false;
//     servo2.attach(0);
//     digitalWrite(buzzer, LOW);
//   }
// }

void timerEvent() {
    ERa.virtualWrite(V0, gas1val);
    ERa.virtualWrite(V1, gas2val);
    ERa.virtualWrite(V2, gas3val);
    ERa.virtualWrite(V3, t1);
    ERa.virtualWrite(V4, t2);
    ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}
void loop() {
  GasVal();
  TmpVal();
  ERa.run();
  timer.run();

  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.setCursor(5, 0);
  lcd.print(gas1val);
  lcd.setCursor(8, 0);
  lcd.print(gas2val);
  lcd.setCursor(11, 0);
  lcd.print(gas3val);
  lcd.setCursor(0, 1);
  lcd.print("Tmp:");
  lcd.setCursor(5, 1);
  lcd.print(t1);
  lcd.setCursor(8, 1);
  lcd.print(t2);

}