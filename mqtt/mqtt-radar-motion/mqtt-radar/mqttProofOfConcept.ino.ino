/* Learn to use pushbutton (button switch) with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/category/button
   Dev: Michalis Vasilakis / Date: 19/10/2014  
   UPDATED 2/6/2016 - LED to Arduino pin 3 and button to Arduino pin 4*/


//Constants
const int buttonPin = 4;     
const int ledPin =  13;     

//Variables
int buttonState = 0;
int flag=0;

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }
  //Input or output?
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT_PULLUP);   
  //digitalWrite(ledPin, LOW);
}

void loop(){
  //Read button state (pressed or not pressed?)
  buttonState = digitalRead(buttonPin);

  //If button pressed...
  if (buttonState == HIGH) { 
    Serial.println("Button Pressed!");
    //...ones, turn led on!
    if ( flag == 0){
      digitalWrite(ledPin, HIGH);
      flag=1; //change flag variable
    }
    //...twice, turn led off!
    else if ( flag == 1){
      digitalWrite(ledPin, LOW);
      flag=0; //change flag variable again 
    }    
  }
  delay(200); //Small delay

}
