#include <PulsePosition.h>
PulsePositionOutput pulseOutput(FALLING);
uint32_t loops = 0;
uint32_t timeCommand = micros();
volatile uint32_t timeLastPulse = micros();
volatile uint32_t timeThisPulse = micros();
String printBuffer = "";

void setup() {
  pulseOutput.begin(5);
  pulseOutput.write(1, 900);
  Serial.begin(115200);
  attachInterrupt(6, motorISR, CHANGE);
}

void loop() {
  if (Serial.available()) {
    loops=Serial.parseInt();
  }
  while (loops > 0) {
    pulseOutput.write(1, 2100);
    timeCommand = micros();
    delay(1000);
    pulseOutput.write(1, 900);
    delay(1000);
    loops--;
  }
  Serial.println('s');
  delay(100);
}

void motorISR() {
  timeThisPulse = micros();
  printBuffer = "b";
  printBuffer += timeThisPulse-timeCommand;
  printBuffer += "m";
  printBuffer += timeThisPulse-timeLastPulse;
  printBuffer += "e";
  Serial.print(printBuffer);
  timeLastPulse = timeThisPulse;
}
