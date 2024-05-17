#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
RTC_DS3231 rtc;
Servo servo1, servo2;
int buzzerPin = 2;
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);  

  // Inicializa o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Infinite loop if display initialization fails
  }
  display.clearDisplay();
  
  // Inicializa o RTC DS3231
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Infinite loop if RTC is not found
  }
  
  // Verifica se o RTC perdeu energia e, se sim, define uma data e hora específicas
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // Definir data e hora especificamente
    rtc.adjust(DateTime(2024, 4, 17, 12, 0, 50)); // ano, mês, dia, hora, minuto, segundo
  }

  rtc.adjust(DateTime(2024, 4, 17, 12, 0, 50)); // ano, mês, dia, hora, minuto, segundo

  servo1.attach(9);
  servo2.attach(10);

  servo1.write(0);
  servo2.write(90);
  delay(1000);

  display.dim(false); // Ajusta o brilho do display
}

void loop() {
  DateTime now = rtc.now();

  // Verifica se a hora é igual a 24 e, se for, define a hora como 0
  if (now.hour() == 24) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
  }
  
  display.clearDisplay();
  display.setTextSize(2); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(16, 40);
  if (now.hour() < 10) {
    display.print("0"); // Adiciona um zero à esquerda se a hora for menor que 10
  }
  display.print(now.hour());
  display.print(":");
  if (now.minute() < 10) {
    display.print("0"); // Adiciona um zero à esquerda se o minuto for menor que 10
  }
  display.print(now.minute());
  display.print(":");
  if (now.second() < 10) {
    display.print("0"); // Adiciona um zero à esquerda se o segundo for menor que 10
  }
  display.println(now.second());
  display.display();
  
  delay(1000);

  // Checa se o segundo atual é igual a 0
  if (now.second() == 0) {

    digitalWrite(ledPin, HIGH);
    display.clearDisplay();
    display.setCursor(0,0); 
    display.print("Dispensando...");
    display.display();
    buzzer();
    moveServos();
    digitalWrite(ledPin, LOW);
    delay(1000);
  }
}
void buzzer(){
    for (int i = 0; i < 1; i++) {
    digitalWrite(buzzerPin, HIGH); // Liga o buzzer
    delay(1000);                   // Mantém o buzzer ligado por 3 segundos
    digitalWrite(buzzerPin, LOW);  // Desliga o buzzer
    delay(1000);                   // Espera 1 segundo antes de repetir
  }

  // Ajuste este delay para quanto tempo você quer esperar antes de repetir todo o processo
  delay(1000);  
}

void moveServos() {
  // Ativa cada servo sequencialmente
  servo1.write(90);
  delay(500);
  servo1.write(0);
  delay(500);

  servo2.write(0);
  delay(500);
  servo2.write(90);
  delay(500);
}
