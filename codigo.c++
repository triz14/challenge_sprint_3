// Inclusão das bibliotecas
#include <WiFi.h>
#include <ArduinoJson.h>
#include "RTClib.h"
#include <PubSubClient.h>
 
// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  
 
// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "BIA_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "challenge/sprint";
const char *TOPIC_PUBLISH_DATE_HOUR = "challenge/sprint/tres";
 
// Configurações de Hardware
#define PIN_LED 15
#define PUBLISH_DELAY 2000
 
// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
RTC_DS1307 rtc;
unsigned long publishUpdate = 0;
const int TAMANHO = 200;
int pirPin = 12;  
bool pirLastState = LOW; // Estado anterior do sensor PIR
 
// Protótipos de funções
void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
 
void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
 
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}
 
void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char *)payload).substring(0, length);
 
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);
 
  if (strcmp(topic, TOPIC_SUBSCRIBE_LED) == 0) {
    int valor = atoi(msg.c_str());
 
    if (valor == 1) {
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ligado via comando MQTT");
    } else if (valor == 0) {
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED desligado via comando MQTT");
    }
  }
}
 
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);
 
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}
 
void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}
 
void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;
 
  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
 
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  pinMode(pirPin, INPUT);
  initWiFi();
  initMQTT();
 
#ifndef ESP32
  while (!Serial);
#endif
 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
 
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Ajusta o RTC com a data/hora da compilação
  }
}
 
void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();
 
  int pirState = digitalRead(pirPin);
 
  if (pirState != pirLastState && pirState == HIGH) { // Detecta mudança no estado do sensor
    publishUpdate = millis();
    DateTime time = rtc.now();
 
    String timehour = time.timestamp(DateTime::TIMESTAMP_TIME);
    String timedate = time.timestamp(DateTime::TIMESTAMP_DATE);
 
    StaticJsonDocument<TAMANHO> doc;
    doc["hora"] = timehour;
    doc["data"] = timedate;
 
    char buffer[TAMANHO];
    serializeJson(doc, buffer);
    MQTT.publish(TOPIC_PUBLISH_DATE_HOUR, buffer); // Publica as informações no MQTT
 
    Serial.println(String("Movimento detectado! Data e hora: ") + buffer);
  }
 
  pirLastState = pirState; // Atualiza o estado anterior do sensor PIR
}
 
