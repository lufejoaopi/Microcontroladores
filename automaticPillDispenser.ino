#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
RTC_DS3231 rtc;
Servo servo1, servo2, servo3;
int buzzerPin = 2; 

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

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
    rtc.adjust(DateTime(2024, 4, 17, 24, 0, 0)); // ano, mês, dia, hora, minuto, segundo
  }

  // rtc.adjust(DateTime(2024, 4, 17, 10, 46, 0)); // ano, mês, dia, hora, minuto, segundo

  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(12);

  display.dim(false); // Ajusta o brilho do display
}

void loop() {
  DateTime now = rtc.now(); // Lê a data e hora atual

  display.clearDisplay();
  display.setTextSize(2); // Define o tamanho do texto
  display.setTextColor(WHITE); // Define a cor do texto
  display.setCursor(0,0); // Posiciona o cursor

  // Formata e exibe a hora e o minuto
  if (now.hour() < 10) {
    display.print("0");
  }
  display.print(now.hour(), DEC);
  display.print(':');
  if (now.minute() < 10) {
    display.print("0");
  }
  display.print(now.minute(), DEC);
  display.display(); // Mostra o que foi escrito no display

  delay(1000); // Espera um segundo para atualizar o display novamente

  //display.clearDisplay();
  //display.setCursor(0,0); // Posiciona o cursor
  //display.print("");
  //display.display(); 

  //delay(2000);

 
  // Checa se o minuto atual é um múltiplo de 10
  if (now.minute() % 2 == 0 && now.second() == 0) {
   
    delay(1000); 
    moveServos();  // Move os servos se a condição for verdadeira
    buzzer();
    display.clearDisplay();
    display.setCursor(0,0); // Posiciona o cursor
    display.print("Ta na hora do remédio");
    display.display(); 

    delay(5000);
  }

  // Atualização do display de tempo a cada segundo
 
}
void buzzer(){
    for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, HIGH); // Liga o buzzer
    delay(3000);                   // Mantém o buzzer ligado por 3 segundos
    digitalWrite(buzzerPin, LOW);  // Desliga o buzzer
    delay(1000);                   // Espera 1 segundo antes de repetir
  }

  // Ajuste este delay para quanto tempo você quer esperar antes de repetir todo o processo
  delay(10000);  
}

void moveServos() {
  // Ativa cada servo sequencialmente
  servo1.write(90);
  delay(1000);
  servo2.write(90);
  delay(1000);
  servo3.write(90);
  delay(1000);

  // Retorna cada servo para a posição inicial
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}
