/* Learn to use pushbutton (button switch) with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/category/button
   Dev: Michalis Vasilakis / Date: 19/10/2014  
   UPDATED 2/6/2016 - LED to Arduino pin 3 and button to Arduino pin 4*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Bounce2.h>
#include <EEPROM.h>
#include <Arduino.h>
//Constants
char * HOME_SSID = "javssecure";
char * HOME_PASS = "JaV5L0u15ViLL3";
const char* motion_event_topic = "places/home/test/motion";
const char* mqtt_server = "107.202.1.107";
const int MOTION_PIN = 4;     
const int LED_PIN =  13;     



WiFiClient espClient;
PubSubClient client(espClient);

//Variables
byte motionState = 0;
int flag=0;
long lastMsg = 0;
char msg[50];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]\n");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(LED_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("LED_PIN -> LOW");
    motionState = LOW;
    EEPROM.write(0, motionState);    // Write state to EEPROM
    EEPROM.commit();
  } else if ((char)payload[0] == '1') {
    digitalWrite(LED_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("LED_PIN -> HIGH");
    motionState = HIGH;
    EEPROM.write(0, motionState);    // Write state to EEPROM
    EEPROM.commit();
  } else if ((char)payload[0] == '2') {
    motionState = !motionState;
    digitalWrite(LED_PIN, motionState);  // Turn the LED off by making the voltage HIGH
    Serial.print("LED_PIN -> switched to ");
    Serial.println(motionState); 
    EEPROM.write(0, motionState);    // Write state to EEPROM
    EEPROM.commit();
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

void motionStateChanged() {
    digitalWrite(LED_PIN, motionState);
    //EEPROM.write(0, motionState);    // Write state to EEPROM
    if (motionState == 1){
    client.publish(motion_event_topic, "1");
    }
    else if (motionState == 0){
    client.publish(motion_event_topic, "0");
    }
}

void startWifi(){
    int attemptCount = 0;
    WiFi.mode(WIFI_STA);
    Serial.print("Reconnecting...");
    WiFi.begin(HOME_SSID, HOME_PASS);
    do{
        Serial.print(WiFi.status());       
        delay(1000);
        //Serial.print("\nConnecting to : "); Serial.print(HOME_SSID); Serial.print("\nPassword: "); Serial.print(HOME_PASS);
        
        if(attemptCount > 30)
        {
           Serial.print("\nRESETTING DUE TO CONNECTION FAILURE");
           ESP.restart();
        }
        //try 13 times then log failure and return
        attemptCount++;
    }while (WiFi.status() != WL_CONNECTED);
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }
  //INIT AND READ MEM
  EEPROM.begin(512);
  motionState = EEPROM.read(0);  
  //Input or output?
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  
  startWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){

  if (!client.connected()) {
    reconnect();
  }
  byte pinState = digitalRead(MOTION_PIN);
  if(pinState != motionState){
    motionState = pinState;
    motionStateChanged();
  }
  client.loop();
}
