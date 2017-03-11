#define SWITCH_PIN 0
#define ESC_PIN 5
#define MOTOR_PIN 6
#define LED_PIN 13

#define DEBOUNCE_MS 100
#define ESC_FREQ 400
#define PWM_MAX 65535

uint32_t loops = 0;
uint32_t timeCommand = millis();
uint32_t timeNow = millis();
uint32_t timeLastSwitch = millis();
volatile uint32_t timeLastPulse = micros();
volatile uint32_t timeThisPulse = micros();
String printBuffer = "";

//uint16_t pulseLenOff = 105;
//uint16_t pulseLenOn = 260;
uint16_t pulseLenOff = 900;
uint16_t pulseLenOn = 2100;
uint32_t escOutVal = 0;
bool switchState = true;
bool debouncedSwitchState = true;

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ESC_PIN, OUTPUT);
  analogWriteFrequency(ESC_PIN, ESC_FREQ);
  analogWriteResolution(16);
  Serial.begin(115200);
  attachInterrupt(MOTOR_PIN, motorISR, CHANGE);
}

void loop() {
  if (Serial.available()) {
    delay(1);
    loops=Serial.parseInt();
    while (loops > 0) {
      Serial.println("tarting");
      escOut(pulseLenOn);
      timeCommand = micros();
      delay(1000);
      escOut(pulseLenOff);
      delay(1000);
      loops--;
    }
    Serial.println('s');
  }

  switchState = !digitalReadFast(SWITCH_PIN);
  if (switchState != debouncedSwitchState) {
    timeNow = millis();
    if (timeNow-timeLastSwitch > DEBOUNCE_MS) {
      debouncedSwitchState = switchState;
      timeLastSwitch = timeNow;
      if (debouncedSwitchState == 1){
        digitalWriteFast(LED_PIN, HIGH);
        escOut(pulseLenOn);
      } else {
        digitalWriteFast(LED_PIN, LOW);
        escOut(pulseLenOff);
      }
    }
  }
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

void escOut(uint32_t escOut_us) {
  escOutVal = ((escOut_us*ESC_FREQ)/1000*PWM_MAX)/1000;
  Serial.println(escOutVal);
  analogWrite(ESC_PIN, escOutVal);
}
