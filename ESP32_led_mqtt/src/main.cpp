#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define LED_PIN 2

//Setup WiFi credentials
const char* ssid = "SISWANTO";
const char* password = "Cemol231";

//Setup MQTT Broker
const char* mqtt_server = "df9d2e89ca9845ea915e61ebe04b2037.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "led_mqtt_test";
const char* mqtt_password = "hiveLolin32";

//Setup Topic
const char* topic = "led/control";
const char* pub_topic = "led/status";

WiFiClientSecure espClient;
PubSubClient client(espClient);

//CALLBACK function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Pesan masuk di sub_topic [ ");
  Serial.print(topic);
  Serial.print(" ]:");
  Serial.println(message);

  if (message == "ON") {
    digitalWrite(LED_PIN, HIGH);
    client.publish(pub_topic, "LED is ON");
    Serial.println("LED dinyalakan");
  } else if (message == "OFF") {
    digitalWrite(LED_PIN, LOW);
    client.publish(pub_topic, "LED is OFF");
    Serial.println("LED dimatikan");
  }

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
    Serial.print("Connecting MQTT... ");

    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe(topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry 5s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  setup_wifi();

  espClient.setInsecure();   // 🔥 INI KUNCI UTAMA
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // static unsigned long lastMsg = 0;
  // if (millis() - lastMsg > 5000) {
  //   lastMsg = millis();
  //   client.publish(topic, "Halo dari ESP32 🚀");
  // }
}
