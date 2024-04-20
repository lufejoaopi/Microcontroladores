#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo servo1, servo2, servo3;

// Data e hora específica para ajustar o RTC manualmente
const int ano = 2024;
const int mes = 4;
const int dia = 17;
const int hora = 20;
const int minuto = 0;
const int segundo = 0;

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);  // Parar se o RTC não for encontrado
  }

  if (rtc.lostPower()) {
    Serial.println("RTC perdeu a hora. Corrigindo...");
    rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
  }

  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(12);
}

void loop() {
  DateTime now = rtc.now();

  // Checa se o minuto atual é um múltiplo de 10
  if (now.minute() % 1 == 0 && now.second() == 0) {
    moveServos();  // Move os servos se a condição for verdadeira
  }

  // Atualização do display de tempo a cada segundo
  if (millis() % 1000 < 50) {  // Simples debounce para evitar múltiplas impressões
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  }
}

void moveServos() {
  servo1.write(90);
  delay(1000);
  servo2.write(90);
  delay(1000);
  servo3.write(90);
  delay(1000);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}
