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
char * HOME_SSID = "davidjohnson";
char * HOME_PASS = "ytsedrah";
const char* btn_event_topic = "dev/test/led";
const char* mqtt_server = "192.168.1.77";
const int buttonPin = 4;     
const int relay_pin =  13;     



WiFiClient espClient;
PubSubClient client(espClient);
// Instantiate a Bounce object :
Bounce debouncer = Bounce(); 

//Variables

int relayState = 0;
int buttonState = 0;
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
    digitalWrite(relay_pin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("relay_pin -> LOW");
    relayState = LOW;
    EEPROM.write(0, relayState);    // Write state to EEPROM
    EEPROM.commit();
  } else if ((char)payload[0] == '1') {
    digitalWrite(relay_pin, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("relay_pin -> HIGH");
    relayState = HIGH;
    EEPROM.write(0, relayState);    // Write state to EEPROM
    EEPROM.commit();
  } else if ((char)payload[0] == '2') {
    relayState = !relayState;
    digitalWrite(relay_pin, relayState);  // Turn the LED off by making the voltage HIGH
    Serial.print("relay_pin -> switched to ");
    Serial.println(relayState); 
    EEPROM.write(0, relayState);    // Write state to EEPROM
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
      client.publish(btn_event_topic, "E12 Button Connected");
      // ... and resubscribe
      client.subscribe(btn_event_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for(int i = 0; i<5000; i++){
        extButton();
        delay(1);
      }
    }
  }
}

void extButton() {
  debouncer.update();
   
   // Call code if Bounce fell (transition from HIGH to LOW) :
   if ( debouncer.fell() ) {
     Serial.println("Debouncer fell");
     // Toggle relay state :
     relayState = !relayState;
     digitalWrite(relay_pin,relayState);
     EEPROM.write(0, relayState);    // Write state to EEPROM
     if (relayState == 1){
      client.publish(btn_event_topic, "1");
     }
     else if (relayState == 0){
      client.publish(btn_event_topic, "0");
     }
   }
}

void startWifi(){
    int attemptCount = 0;
    WiFi.mode(WIFI_STA);
    Serial.print("Reconnecting...");
    WiFi.begin("davidjohnson", "ytsedrah");
    do{
        Serial.print(WiFi.status());       
        delay(1000);
        //Serial.print("\nConnecting to : "); Serial.print(HOME_SSID); Serial.print("\nPassword: "); Serial.print(HOME_PASS);
        
        if(attemptCount > 50){ Serial.print("\nRESETTING DUE TO CONNECTION FAILURE"); ESP.restart(); }//try 13 times then log failure and return
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
  relayState = EEPROM.read(0);  
  //Input or output?
  pinMode(relay_pin, OUTPUT);      
  pinMode(buttonPin, INPUT);
  debouncer.attach(buttonPin);   // Use the bounce2 library to debounce the built in button
  debouncer.interval(50);   
  //restore previous state of led
  digitalWrite(relay_pin,relayState);
  
  startWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  extButton();
}
