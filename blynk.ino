#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Ganti dengan WiFi dan Blynk token kamu
char auth[] = "ISI_DENGAN_BLYNK_AUTH_TOKEN";
char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

// DHT setup
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// HC-SR04
#define TRIG_PIN D5
#define ECHO_PIN D6

// Blynk virtual pins
#define VPIN_SUHU V0
#define VPIN_JARAK V1

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  Blynk.run();

  float suhu = dht.readTemperature();
  long jarak = getDistance();

  if (!isnan(suhu)) {
    Serial.print("Suhu: "); Serial.println(suhu);
    Blynk.virtualWrite(VPIN_SUHU, suhu);

    if (suhu > 30) {
      Blynk.logEvent("suhu_panas", "Suhu di atas 30°C!");
    }
  }

  Serial.print("Jarak: "); Serial.print(jarak); Serial.println(" cm");
  Blynk.virtualWrite(VPIN_JARAK, jarak);

  if (jarak < 10) {
    Blynk.logEvent("jarak_dekat", "Jarak lebih kecil dari 10 cm!");
  }

  delay(5000); // jeda 5 detik
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}
