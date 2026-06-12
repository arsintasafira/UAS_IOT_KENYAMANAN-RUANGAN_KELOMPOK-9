#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define DHTPIN 4
#define DHTTYPE DHT22

#define MQ135_PIN 34
#define KY037_PIN 35

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal");
    while (true);
  }

  display.clearDisplay();

  Serial.println("Semua sensor siap!");
}

void loop() {

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  int kualitasUdara = analogRead(MQ135_PIN);
  int kebisingan = analogRead(KY037_PIN);

  String statusUdara;

  if (kualitasUdara < 1000)
    statusUdara = "Baik";
  else if (kualitasUdara < 2000)
    statusUdara = "Sedang";
  else
    statusUdara = "Buruk";

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Suhu: ");
  display.print(suhu, 1);
  display.println(" C");

  display.setCursor(0, 16);
  display.print("Kelembapan: ");
  display.print(kelembapan, 0);
  display.println("%");

  display.setCursor(0, 32);
  display.print("Udara: ");
  display.println(statusUdara);

  display.setCursor(0, 48);
  display.print("Kebisingan: ");
  display.println(kebisingan);

  display.display();


  Serial.println("====================");

  Serial.print("Suhu : ");
  Serial.print(suhu);
  Serial.println(" C");

  Serial.print("Kelembapan : ");
  Serial.print(kelembapan);
  Serial.println(" %");

  Serial.print("Udara : ");
  Serial.print(kualitasUdara);
  Serial.print(" -> ");
  Serial.println(statusUdara);

  Serial.print("Kebisingan : ");
  Serial.println(kebisingan);

  delay(1000);
}