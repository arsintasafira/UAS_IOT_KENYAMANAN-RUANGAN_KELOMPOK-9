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

char ssid[] = "STUDENT@UPNVHotspot";
char pass[] = "iam@student";

// Pin
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define MQ135_PIN 34
#define KY037_PIN 35

#define LED_HIJAU 27
#define LED_MERAH 26

DHT dht(DHT_PIN, DHT_TYPE);
BH1750 lightMeter;
BlynkTimer timer;

BLYNK_CONNECTED() {
  Serial.println("=== BLYNK CONNECTED ===");
}

/*************************************************
 * BACA SENSOR
 *************************************************/
void bacaSensor() {

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca DHT22!");
    return;
  }

  // MQ135
  long totalUdara = 0;
  for (int i = 0; i < 10; i++) {
    totalUdara += analogRead(MQ135_PIN);
    delay(5);
  }
  int kualitasUdara = totalUdara / 10;

  // BH1750
  float cahaya = lightMeter.readLightLevel();
  if (cahaya < 0) cahaya = 0;

  // KY037
  long totalNoise = 0;
  for (int i = 0; i < 20; i++) {
    totalNoise += analogRead(KY037_PIN);
    delay(2);
  }

  int adcNoise = totalNoise / 20;

  // Mapping kasar ADC -> dB
  float kebisingan = map(adcNoise, 0, 4095, 20, 100);

  /*************************************************
   * PENILAIAN KENYAMANAN
   *************************************************/
  bool tidakNyaman = false;

  if (suhu < 22 || suhu > 25)
    tidakNyaman = true;

  if (kelembapan < 40 || kelembapan > 60)
    tidakNyaman = true;

  if (kualitasUdara > 1000)
    tidakNyaman = true;

  if (cahaya < 300 || cahaya > 500)
    tidakNyaman = true;

  if (kebisingan > 35)
    tidakNyaman = true;

  /*************************************************
   * LED
   *************************************************/
  digitalWrite(LED_MERAH, tidakNyaman);
  digitalWrite(LED_HIJAU, !tidakNyaman);

  /*************************************************
   * KIRIM KE BLYNK
   *************************************************/
  if (Blynk.connected()) {

    Blynk.virtualWrite(V0, suhu);
    Blynk.virtualWrite(V1, kelembapan);
    Blynk.virtualWrite(V2, kebisingan);
    Blynk.virtualWrite(V3, kualitasUdara);
    Blynk.virtualWrite(V4, cahaya);
    Blynk.virtualWrite(V5,
                       tidakNyaman ? "TIDAK NYAMAN" : "NYAMAN");

    Serial.println("KIRIM DATA KE BLYNK");
  }
  else {
    Serial.println("BLYNK OFFLINE");
  }

  /*************************************************
   * SERIAL MONITOR
   *************************************************/
  Serial.println("================================");

  Serial.print("Suhu          : ");
  Serial.println(suhu);

  Serial.print("Kelembapan    : ");
  Serial.println(kelembapan);

  Serial.print("KualitasUdara : ");
  Serial.println(kualitasUdara);

  Serial.print("Cahaya        : ");
  Serial.println(cahaya);

  Serial.print("Kebisingan    : ");
  Serial.println(kebisingan);

  Serial.print("Blynk Status  : ");
  Serial.println(Blynk.connected());

  Serial.println("================================");
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  dht.begin();

  // ESP32 SDA=21 SCL=22
  Wire.begin(21, 22);

  if (lightMeter.begin()) {
    Serial.println("BH1750 OK");
  } else {
    Serial.println("BH1750 GAGAL");
  }

  Serial.println("Menghubungkan WiFi...");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Blynk.config(BLYNK_AUTH_TOKEN);

  if (Blynk.connect()) {
    Serial.println("BLYNK ONLINE");
  } else {
    Serial.println("BLYNK GAGAL");
  }

  timer.setInterval(3000L, bacaSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
