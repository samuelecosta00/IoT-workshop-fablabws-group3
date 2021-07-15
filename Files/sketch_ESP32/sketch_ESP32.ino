//chiamiamo le librerie del sensore di umidità, del Wi-Fi e quelle scaricate
#include "DHT.h";
#include <WiFi.h>;
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
//definiamo il nome e la password della rete a cui il sistema sarà collegato
#define WIFI_SSID "Sam Phone"
#define WIFI_PASSWORD "1234567890"
//definiamo l'indirizzo ip della raspberry che sarebbe l'host e la porta che è un valore standard
#define MQTT_HOST IPAddress(192, 168, 43, 111)
#define MQTT_PORT 1883
//definiamo i canali dove verranno pubblicati i valori acquisiti dal sensore
#define MQTT_PUB_TEMP "esp32/dht/temperature"
#define MQTT_PUB_HUM "esp32/dht/humidity"
//difiniamo il pin dell' ESP32 a cui è collegato il sensore e il tipo di sensore utilizzato
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//dichiariamo due variabili float per la temperatura e l'umidità
float temp;
float hum;
//dichiariamo due varibili che useremò come intervalli tra un invio di dati e l'altro
unsigned long previousMillis = 0;
const long interval = 5000;
//creaiamo un oggetto Mqtt che gestirà le richieste verso il server
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
//creiamo un void per l'iniazilizzazione della connessione Wi-Fi
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}
//creiamo un void per definire gli eventi Wi-Fi
void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
      break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0);
        xTimerStart(wifiReconnectTimer, 0);
      break;
    }
}
//creiamo dei void per le funzioni MQTT
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) { 
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print(" packetId: ");
  Serial.println(packetId);
}
//prima inizializziamo il monitor seriale, e il sensore di umidità in void setup
void setup() {
  Serial.begin(9600);
  Serial.println();
  
  dht.begin();
  //creiamo delle funzioni setup che ci permettono di riconnettere la scheda in caso di errori di connessione
  mqttReconnectTimer = xTimerCreate("mqttTimer",pdMS_TO_TICKS(2000),pdFALSE, (void*)0,reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer",pdMS_TO_TICKS(2000),pdFALSE, (void*)0,reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  //diciamo alla scheda ESP32 che quando accade un qualsiasi evento Wi-Fi deve eseguire la funzione voide WiFievent
  WiFi.onEvent(WiFiEvent);
  //la stessa cosa con gli eventi MQTT
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  //connessione al Wi-Fi
  connectToWifi();
}

void loop() {
  //prendiamo il tempo corrente
  unsigned long currentMillis = millis();
  //creiamo un if che in base al tempo passato regolamenta l'invio dei dati (ogni 10 secondi) 
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //leggiamo i valori del sensore
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    //creiamo un if che controlla la corretta lettura del sensore
    if (isnan(temp) || isnan(hum)) {
      Serial.println(F("Error: Failed to read from DHT sensor!"));
    } 
    else {
      //se la lettura è andata a buon fine comunica al MQTT prima la temperatura e poi l'umidità
      uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str()); 
      Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
      Serial.printf("Message: %.2f \n", temp);
 
      uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str()); 
      Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
      Serial.printf("Message: %.2f \n", hum);
    }
 }
}
