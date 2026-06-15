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

  // Koneksi WiFi
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

  /*************************************************
   * DHT22 - SUHU & KELEMBAPAN
   *************************************************/
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  /*************************************************
   * MQ135 - KUALITAS UDARA
   *************************************************/
  float totalUdara = 0;
  for (int i = 0; i < 5; i++) {
    totalUdara += analogRead(MQ135_PIN);
    delay(10);
  }
  int kualitasUdara = totalUdara / 5;

  /*************************************************
   * BH1750 - INTENSITAS CAHAYA
   *************************************************/
  float totalCahaya = 0;
  for (int i = 0; i < 5; i++) {
    totalCahaya += lightMeter.readLightLevel();
    delay(50);
  }
  float cahaya = totalCahaya / 5;

  /*************************************************
   * KY037 - KEBISINGAN (PERKIRAAN dB)
   *************************************************/
  float totalBising = 0;
  for (int i = 0; i < 5; i++) {
    totalBising += analogRead(KY037_PIN);
    delay(10);
  }

  int adcNoise = totalBising / 5;

  // Mapping kasar ADC -> dB
  float kebisingan = map(adcNoise, 0, 4095, 20, 100);

  /*************************************************
   * PENENTUAN KENYAMANAN
   *************************************************/
  bool tidakNyaman = false;

  // Suhu nyaman 22–25°C
  if (!isnan(suhu) && !isnan(kelembapan)) {

    if (suhu < 22 || suhu > 25)
      tidakNyaman = true;

    // Kelembapan nyaman 40–60%
    if (kelembapan < 40 || kelembapan > 60)
      tidakNyaman = true;
  }

  // Kualitas udara
  if (kualitasUdara >= 1000)
    tidakNyaman = true;

  // Cahaya nyaman 300–500 lux
  if (cahaya < 300 || cahaya > 500)
    tidakNyaman = true;

  // Kebisingan maksimal 35 dB
  if (kebisingan > 35)
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
   * STATUS LED
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

  /*************************************************
   * SERIAL MONITOR
   *************************************************/
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println(" %");

  Serial.print("Kualitas Udara (MQ135): ");
  Serial.println(kualitasUdara);

  Serial.print("Cahaya (BH1750): ");
  Serial.print(cahaya);
  Serial.println(" lux");

  Serial.print("Kebisingan: ");
  Serial.print(kebisingan);
  Serial.println(" dB");

  Serial.println("--------------------");

  delay(2000);
}