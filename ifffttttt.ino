#include <ESP8266WiFi.h>
#include "DHT.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi credentials
#define WIFI_SSID       "NAMA_WIFI"
#define WIFI_PASSWORD   "PASSWORD_WIFI"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "USERNAME_ADAFRUIT"
#define AIO_KEY         "AIO_KEY_KAMU"

// DHT setup
#define DHTPIN D2
#define DHTTYPE DHT11 // atau DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ-2
#define MQ2_PIN A0

// WiFi client
WiFiClient client;

// MQTT client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds
Adafruit_MQTT_Publish suhuFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/suhu");
Adafruit_MQTT_Publish gasFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas");

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Connect to Adafruit IO
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) {
    connectMQTT();
  }

  mqtt.processPackets(10000);
  mqtt.ping();

  float suhu = dht.readTemperature();
  int gasValue = analogRead(MQ2_PIN);

  if (!isnan(suhu)) {
    Serial.print("Suhu: "); Serial.println(suhu);
    suhuFeed.publish(suhu);
  } else {
    Serial.println("Gagal membaca suhu");
  }

  Serial.print("Gas: "); Serial.println(gasValue);
  gasFeed.publish(gasValue);

  delay(5000); // Kirim setiap 5 detik
}

void connectMQTT() {
  while (mqtt.connect() != 0) {
    Serial.println("MQTT gagal, retry...");
    delay(5000);
  }
  Serial.println("MQTT Connected!");
}
