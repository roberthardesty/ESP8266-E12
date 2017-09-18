/*  ESP8266-12E LED-MATRIX CONTROLLER

- uses mqtt for communication
- scrolls messages in QueueArray if there are any
- Tries different Wifi Networks 

TODO:
  DNOE - mqtt callback needs to check the QueueArray
  - if QueueArray is full, reply back to a topic to indicate.
  - if LEDMatrix is idle publish that status
  DONE - if there is room, enqueue the new string.
  DONE - last string in queue should scroll 3 times then shutoff
  DONE - remove delay and instead do millis check
  DONE- STRETCH: change LEDMessage to LEDMessage to control scroll cycles via json

*/
#include <cp437font.h>
#include <LedMatrix.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <QueueArray.h>
#include <LEDMessage.h>
//Constants
#define NUMBER_OF_DEVICES 4
#define CS_PIN 2
#define LED_INTENSITY 4
#define CHAR_WIDTH 7

char * WORK_SSID = "javssecure";
char * WORK_PASS = "JaV5L0u15ViLL3";
char * HOME_SSID = "davidjohnson";
char * HOME_PASS = "ytsedrah";
const char* motion_event_topic = "devices/ledmatrix/message";
const char* HOME_MQTT_ADDRESS = "107.202.1.107";
const char* DEV_MQTT_ADDRESS = "192.168.1.67";


LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
LEDMessage ledMessage;
WiFiClient espClient;
PubSubClient client(espClient);
QueueArray <String> _messageQueue;

//Variables
byte _wifiNetwork = 0;
unsigned long _loopMillis;
unsigned long _loopCount;
int _scrollSpeed = 20;
int _sameScrollCount = 0;
bool _isScrollingSame = false;
char msg[50];

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]\n");
  char jsonPayload[length];
  for (int i = 0; i < length; i++) {
    jsonPayload[i] = (char)payload[i];
  }
  String jsonString = String(jsonPayload);
  bool isLoaded = ledMessage.load(jsonString);
  if(!isLoaded)
    client.publish("", "Invalid Json");
  else if(isLoaded && _messageQueue.count() > 10)
    client.publish("","No Room Sorry");
  else
  {
    _messageQueue.enqueue(ledMessage.messageString)
    _isScrollingSame = false;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(motion_event_topic, "E12 Button Connected");
      //client.subscribe(motion_event_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for(int i = 0; i<5000; i++){
        //motionStateChanged();
        delay(1);
      }
    }
  }
}

void startWifi(){
    int attemptCount = 0;
    WiFi.mode(WIFI_STA);
    Serial.print("Reconnecting...");

    if(_wifiNetwork == 0)
    {
      WiFi.begin(HOME_SSID, HOME_PASS);
      _wifiNetwork = 1;
    }
    else if(_wifiNetwork == 1)
    {
      Wifi.begin(WORK_SSID, WORK_PASS);
      _wifiNetwork = 0;
    }

    do{
        Serial.print(WiFi.status());       
        delay(1000);
        //Serial.print("\nConnecting to : "); Serial.print(HOME_SSID); Serial.print("\nPassword: "); Serial.print(HOME_PASS);
        
        if(attemptCount > 15)
        {
           Serial.print("\nRESETTING DUE TO CONNECTION FAILURE");
           EEPROM.write(0, _wifiNetwork);
           ESP.restart();
        }
        //try 13 times then log failure and return
        attemptCount++;
    }while (WiFi.status() != WL_CONNECTED);
}

void ledInit(){
  ledMatrix.init();
  ledMatrix.setIntensity(LED_INTENSITY);
  ledMatrix.setCharWidth(CHAR_WIDTH);// range is 0-15
  ledMatrix.setRotation(true);
  ledMatrix.setText("Hello!");
}

bool requestNewText(){
  if(_messageQueue.isEmpty()) return false;
  String message = _messageQueue.dequeue()
  ledMatrix.setNextText(message);
  return true;
}

void setup() 
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) 
  {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }
  //INIT AND READ MEM
  EEPROM.begin(512);
  _wifiNetwork = EEPROM.read(0);  

  startWifi();
  
  client.setServer(HOME_MQTT_ADDRESS, 1883);
  client.setCallback(callback);
  
  ledInit();
  ledMatrix.setNextText("Loading: Dictionary, Thesaurus, English Grammer Model");
}

void loop()
{

  client.loop();
  unsigned long currentMillis = millis();

  /*
  Things that need to happen a lot--------------------------------/\
  ----------------------------------------------------------------||
  ----------------------------------------------------------------||
  things that can happen at a controlled speed--------------------\/
  Now includes shutdown if same thing has been scrolled repeatedly
  */
  if(currentMillis - _loopMillis < _scrollSpeed || _isScrollingSame) return;

  _loopMillis = currentMillis;

  if (!client.connected())
  {
    //reconnect();
  }

  ledMatrix.clear();
  //If both these are true then we are ready for a new word but the QueueArray is empty
  if(ledMatrix.scrollTextLeft() && !requestNewText())
  {
    _isScrollingSame = _sameScrollCount > 3;
    _sameScrollCount++;
  }
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays

  //reset things for when we get a new message
  if(_isScrollingSame)
  {
    ledMatrix.clear();
    ledMatrix.commit();
    _sameScrollCount = 0;
    //publish idle status
  }

}
