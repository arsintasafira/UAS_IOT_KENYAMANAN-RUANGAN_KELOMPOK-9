/*************************************************
 * BLYNK
 *************************************************/
#define BLYNK_TEMPLATE_ID "TMPL6rU94W-u8"
#define BLYNK_TEMPLATE_NAME "SENSOR"
#define BLYNK_AUTH_TOKEN "GER1uBUWeGYMHjCHk3fGLNz4EYPzAvbB"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <DHT.h>
#include <BH1750.h>

char ssid[] = "WIFIATAS";
char pass[] = "Hilux2kd";

// Pin definitions
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define MQ135_PIN 34
#define KY037_PIN 35
#define LED_HIJAU 27
#define LED_MERAH 26

// Sensor objects
DHT dht(DHT_PIN, DHT_TYPE);
BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  dht.begin();
  Wire.begin();
  lightMeter.begin();

  // Konek WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting WiFi");
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect();
  } else {
    Serial.println("\nWiFi GAGAL! Jalan tanpa Blynk.");
  }

  Serial.println("Semua sensor siap!");
}

void loop() {

  if (Blynk.connected()) {
    Blynk.run();
  }

  // DHT22 - Suhu & Kelembapan
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  // MQ-135 - Kualitas Udara (rata-rata 5x)
  float totalUdara = 0;
  for(int i = 0; i < 5; i++) {
    totalUdara += analogRead(MQ135_PIN);
    delay(10);
  }
  int kualitasUdara = totalUdara / 5;

  // BH1750 - Cahaya (rata-rata 5x)
  float totalCahaya = 0;
  for(int i = 0; i < 5; i++) {
    totalCahaya += lightMeter.readLightLevel();
    delay(50);
  }
  float cahaya = totalCahaya / 5;

  // KY-037 - Kebisingan (rata-rata 5x)
  float totalBising = 0;
  for(int i = 0; i < 5; i++) {
    totalBising += analogRead(KY037_PIN);
    delay(10);
  }
  int kebisingan = totalBising / 5;

  bool tidakNyaman = false;

  // Cek Suhu & Kelembapan
  if (!isnan(suhu) && !isnan(kelembapan)) {
    if (suhu < 22 || suhu > 28)
      tidakNyaman = true;
    if (kelembapan < 40 || kelembapan > 60)
      tidakNyaman = true;
  }

  // Cek Kualitas Udara
  if (kualitasUdara >= 1000)
    tidakNyaman = true;

  // Cek Cahaya
  if (cahaya < 300 || cahaya > 500)
    tidakNyaman = true;

  // Cek Kebisingan
  if (kebisingan > 55)
    tidakNyaman = true;

  /*************************************************
   * KIRIM DATA KE BLYNK
   *************************************************/
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, suhu);
    Blynk.virtualWrite(V1, kelembapan);
    Blynk.virtualWrite(V2, kebisingan);
    Blynk.virtualWrite(V3, kualitasUdara);
    Blynk.virtualWrite(V4, cahaya);
    Blynk.virtualWrite(V5, tidakNyaman ? "TIDAK NYAMAN" : "NYAMAN");
  }

  /*************************************************
   * STATUS LED & SERIAL
   *************************************************/
  if (tidakNyaman) {
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_HIJAU, LOW);
    Serial.println("STATUS: TIDAK NYAMAN 🔴");
  } else {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);
    Serial.println("STATUS: NYAMAN 🟢");
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println(" %");

  Serial.print("Kualitas Udara (MQ135): ");
  Serial.print(kualitasUdara);
  if (kualitasUdara < 1000) Serial.println(" → BAIK");
  else if (kualitasUdara <= 2000) Serial.println(" → SEDANG");
  else Serial.println(" → BURUK");

  Serial.print("Cahaya (BH1750): ");
  Serial.print(cahaya);
  Serial.println(" lx");

  Serial.print("Kebisingan (KY037): ");
  Serial.println(kebisingan);

  Serial.println("--------------------");

  delay(2000);
}