#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define AWS_IOT_SUBSCRIBE_TOPIC1 "Parking_System"

// Pins
#define RED_LED 32
#define GREEN_LED 14
#define BUZZER 13
#define SERVO_PIN 25
#define IR_SENSOR 33
#define BUTTON_PIN 26
#define TRIG_PIN 18
#define ECHO_PIN 19

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address may vary

bool gateOpen = false;

// ---- Distance Reading ----
long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // cm
}

// ---- Gate Control ----
void openGate() {
  Serial.println("Gate opening...");

  lcd.clear();
  lcd.print("Gate: OPEN");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);

  gateServo.write(0); // Open gate
  delay(2000); // Keep open & display "Gate: OPEN"

  gateServo.write(90); // Close gate
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  lcd.clear();
  lcd.print("Gate: CLOSED");
  delay(2000); // Show "Gate: CLOSED"

  Serial.println("Gate closed.");
}

// ---- MQTT Message Handler ----
void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* msg = doc["message"];
  if (msg && strcmp(msg, "open") == 0) {
    openGate();
  }
}

void setup() {    
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(RED_LED, HIGH);  // Start: gate closed
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  gateServo.attach(SERVO_PIN);
  gateServo.write(90); // Closed position

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // AWS Certs
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // MQTT Setup
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IoT");
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC1);
  Serial.println("AWS IoT Connected!");
  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  client.loop();

  static unsigned long lastUltrasonicTriggerTime = 0;
  static unsigned long lastDisplayUpdate = 0;

  long distance = readUltrasonicDistance();
  bool isCarParked  = digitalRead(IR_SENSOR) == LOW;
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  unsigned long currentTime = millis();

  // BUTTON should always work, no delay
  if (buttonPressed) {
    openGate();
  }

  // Ultrasonic triggers only every 5 seconds to prevent spamming
  if (distance > 2 && distance < 10 && (currentTime - lastUltrasonicTriggerTime > 5000)) {
    openGate();
    lastUltrasonicTriggerTime = currentTime;
  }

  // Realtime LCD update every 1s
  if (currentTime - lastDisplayUpdate > 1000) {
    lcd.clear();
    lcd.print("Available: ");
    lcd.print(isCarParked ? "0" : "1");
    lastDisplayUpdate = currentTime;
  }

  delay(100);
}
