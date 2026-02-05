#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define ledPin 2    // On-board LED pin
#define tiltPin 35  // Analog pin for tilt sensor
#define buzzerPin 27 // Buzzer pin

//Setup Wifi
const char* ssid = "SISWANTO";
const char* password = "Cemol231";

//Setup MQTT BROKER
const char* mqtt_server = "df9d2e89ca9845ea915e61ebe04b2037.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "led_mqtt_test";
const char* mqtt_password = "hiveLolin32";

//Setup topic
const char* tilt_topic = "home/tilt_sensor";
const char* alert_topic = "home/tilt_alert";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Handle messages for tilt topic
  if (String(topic) == tilt_topic) {
    if (message == "YA") {
      digitalWrite(ledPin, LOW); 
      noTone(buzzerPin); // Deactivate buzzer
    } else if (message == "BUKAN") {
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000); // Activate buzzer at 1000 Hz
    }
  }
}

// Connect to WiFi
void setup_wifi() {
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT... ");

    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe(tilt_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry 5s");
      delay(2000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(tiltPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);  // Ensure LED is off at start
  digitalWrite(buzzerPin, LOW); // Ensure buzzer is off at start

  setup_wifi();

  espClient.setInsecure(); // Disable certificate validation for testing purposes
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  // put your main code here, to run repeatedly:
  int tiltValue = analogRead(tiltPin);

  if (tiltValue == 0) {
    digitalWrite(ledPin, HIGH);  // Turn on LED if tilted
    client.publish(alert_topic, "Tilt detected!");
    client.publish(alert_topic, "ini kamu?");
    tone(buzzerPin, 1000); // Activate buzzer at 1000 Hz
  } else {
    digitalWrite(ledPin, LOW);   // Turn off LED if not tilted
    client.publish(tilt_topic, "No tilt");
    delay(5000);  // Wait for 5 seconds before next reading
  }
  client.loop();
  delay(1000);  // Wait for half a second before next reading
}
