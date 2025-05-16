#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Ganti dengan data kamu
char auth[] = "BLYNK_AUTH_TOKEN_KAMU";
char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

// DHT sensor setup
#define DHTPIN D2
#define DHTTYPE DHT11  // Ganti ke DHT22 jika pakai DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ-2 pin (analog)
#define MQ2_PIN A0

// Virtual pins di Blynk
#define VPIN_SUHU V0
#define VPIN_GAS  V1

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}

void loop() {
  Blynk.run();

  float suhu = dht.readTemperature();
  int gasValue = analogRead(MQ2_PIN); // nilai dari 0-1023

  // Kirim ke Blynk
  Blynk.virtualWrite(VPIN_SUHU, suhu);
  Blynk.virtualWrite(VPIN_GAS, gasValue);

  // Debug
  Serial.print("Suhu: "); Serial.print(suhu); Serial.print(" C | ");
  Serial.print("Gas: "); Serial.println(gasValue);

  // Kirim notifikasi jika suhu tinggi
  if (suhu > 30) {
    Blynk.logEvent("suhu_panas", "Suhu melebihi 30°C!");
  }

  // Kirim notifikasi jika gas tinggi
  if (gasValue > 400) {
    Blynk.logEvent("gas_bocor", "Terdeteksi gas berbahaya!");
  }

  delay(5000); // Jeda 5 detik
}
