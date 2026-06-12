/*
Catatan :
Pengembangan dan implementasi progress kedua project ini telah dilakukan pada 10 Juni 2025.
Repository GitHub baru diperbarui dan diunggah pada tanggal saat ini sebagai bagian dari proses dokumentasi project.
*/

#include <Wire.h>
#include <DHT.h>
#include <BH1750.h>

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

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  dht.begin();

  Wire.begin();
  lightMeter.begin();

  Serial.println("Semua sensor siap!");
}

void loop() {
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

  // Cek kondisi nyaman/tidak
  bool tidakNyaman = false;

  // Suhu
  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("DHT22 gagal dibaca!");
  } else {

    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C → ");

    if (suhu >= 22 && suhu <= 28) {
      Serial.println("NYAMAN");
    } else {
      Serial.println("TIDAK NYAMAN");
      tidakNyaman = true;
    }

    Serial.print("Kelembapan: ");
    Serial.print(kelembapan);
    Serial.print(" % → ");

    if (kelembapan >= 40 && kelembapan <= 60) {
      Serial.println("NYAMAN");
    } else {
      Serial.println("TIDAK NYAMAN");
      tidakNyaman = true;
    }
  }

  // Kualitas Udara
  Serial.print("Kualitas Udara (MQ135): ");
  Serial.print(kualitasUdara);
  Serial.print(" → ");

  if (kualitasUdara < 1000) {
    Serial.println("BAIK");
  } else if (kualitasUdara <= 2000) {
    Serial.println("SEDANG");
    tidakNyaman = true;
  } else {
    Serial.println("BURUK");
    tidakNyaman = true;
  }

  // Cahaya
  Serial.print("Cahaya (BH1750): ");
  Serial.print(cahaya);
  Serial.print(" lx → ");

  if (cahaya >= 300 && cahaya <= 500) {
    Serial.println("NYAMAN");
  } else {
    Serial.println("TIDAK NYAMAN");
    tidakNyaman = true;
  }

  // Kebisingan
  Serial.print("Kebisingan (KY037): ");
  Serial.print(kebisingan);
  Serial.print(" → ");

  if (kebisingan <= 55) {
    Serial.println("NYAMAN");
  } else {
      Serial.println("TIDAK NYAMAN");
      tidakNyaman = true;
  }

  // Kontrol LED
  if (tidakNyaman) {
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_HIJAU, LOW);
    Serial.println("STATUS: TIDAK NYAMAN 🔴");
  } else {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);
    Serial.println("STATUS: NYAMAN 🟢");
  }

  Serial.println("--------------------");
  delay(2000);
}
