#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "soc/adc_channel.h"

#define POTENSIO 32   // Analog pin connected to the potentiometer 

//Setup WiFi credentials
const char* ssid = "SISWANTO";
const char* password = "Cemol231";  
//Setup MQTT Broker
const char* mqtt_server = "df9d2e89ca9845ea915e61ebe04b2037.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "led_mqtt_test";
const char* mqtt_password = "hiveLolin32";
//Setup Topic
const char* topic = "potensiometer/readings";
const char* pub_topic = "potensiometer/status";

WiFiClientSecure espClient;
PubSubClient client(espClient); 

//callback function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
  //subscribe pesan
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Pesan masuk di sub_topic [ ");
  Serial.print(topic);
  Serial.print(" ]:");
  Serial.println(message);

  //publish pesan

  int potValue = analogRead(POTENSIO);

  //Potensiometer
  Serial.print("Potensiometer Value: ");
  Serial.println(potValue); 
  float voltage = potValue * (3.3 / 4095.0); // Convert ADC value to voltage (ESP32 ADC is 12-bit)

  //Publish the voltage value to MQTT topic
  // char voltageStr[50];
  // dtostrf(voltage, 5, 2, voltageStr); // Convert voltage to string
  // client.publish(pub_topic, voltageStr);
  // Serial.print("Ini Voltagenya: ");
  // Serial.println(voltageStr);
}

//WiFi and MQTT setup function
void setup_wifi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, "ESP32 connected");
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(POTENSIO, INPUT);
  analogSetPinAttenuation(POTENSIO, ADC_11db);

  setup_wifi();

  espClient.setInsecure(); // Disable SSL certificate verification (for testing only)
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

int readPotAverage(int pin) {
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  return sum / 20;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // // Membaca potensiometer
  // int potValue = analogRead(POTENSIO); // 0 - 4095
  // float potVoltage = potValue * (3.3 / 4095.0);

  // // Convert ke string untuk MQTT
  // char msg[50];
  // snprintf(msg, 50, "%.2f", potVoltage);

  // // Publish ke MQTT
  // if (client.publish(pub_topic, msg)) {
  //   Serial.print("Published: ");
  //   Serial.println(msg);
  // } else {
  //   Serial.println("Publish gagal");
  // }

int potValue = readPotAverage(POTENSIO);
float potVoltage = potValue * (3.3 / 4095.0);

char msg[20];
snprintf(msg, 20, "%.2f", potVoltage);

static float lastValue = 0;
if (abs(potVoltage - lastValue) > 0.05) {
  lastValue = potVoltage;
  client.publish(pub_topic, msg);
}

  delay(2000); // Publish tiap 2 detik
}
