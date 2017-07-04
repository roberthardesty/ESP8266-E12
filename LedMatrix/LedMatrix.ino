#include <cp437font.h>
#include <LedMatrix.h>

#define NUMBER_OF_DEVICES 4
#define CS_PIN 2
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

void setup() {
  Serial.begin(115200); // For debugging output
  ledMatrix.init();
  ledMatrix.setIntensity(6);
  ledMatrix.setCharWidth(7);// range is 0-15
  ledMatrix.setRotation(true);
  ledMatrix.setText("DIRTY WORD! DIRTY WORD! DIRTY WORD! TGIF!");
}

void loop() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays
  delay(50);
}
