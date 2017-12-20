#define SWITCH_PIN 7
#define ESC_PIN_A 5
#define ESC_PIN_B 6
#define MOTOR_PIN 8
#define LED_PIN 13

#define DEBOUNCE_MS 100
#define ESC_FREQ 400
#define PWM_MAX 65535

uint32_t loops = 0;
uint32_t timeCommand = millis();
uint32_t timeNow = millis();
volatile uint32_t timeLastPulse = micros();
volatile uint32_t timeThisPulse = micros();
String printBuffer = "";

//uint16_t pulseLenOff = 105;
//uint16_t pulseLenOn = 260;
uint16_t pulseLenOff = 900;
uint16_t pulseLenIdle = 1100;
uint16_t pulseLenOn = 2000;
uint32_t escOutVal = 0;
uint32_t onTime = 100;
uint32_t idleTime = 30000;
bool switchPressed = false; // true = pressed
uint16_t motorState = 0; // 3=switch down, 2=switch up, still on, 1=idling, 0=off
uint32_t switchLastPressed = 0;

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ESC_PIN_A, OUTPUT);
  pinMode(ESC_PIN_B, OUTPUT);
  analogWriteFrequency(ESC_PIN_A, ESC_FREQ);
  analogWriteFrequency(ESC_PIN_B, ESC_FREQ);
  analogWriteResolution(16);
  Serial.begin(115200);
  attachInterrupt(MOTOR_PIN, motorISR, CHANGE);
}

void loop() {
  if (Serial.available()) {
    delay(1);
    loops = Serial.parseInt();
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

  switchPressed = !digitalReadFast(SWITCH_PIN);
  if (switchPressed == true) {
    motorState = 3;
  } else switch (motorState) {
      case 3:
        switchLastPressed = millis();
        motorState = 2;
        break;
      case 2:
        if (millis() - switchLastPressed > onTime) {
          escOut(pulseLenIdle);
          motorState = 1; // start idling
          digitalWriteFast(LED_PIN, HIGH);
        }
      case 1:
        if (millis() - switchLastPressed > idleTime) {
          escOut(pulseLenOff);
          motorState = 0; // stop idling
          digitalWriteFast(LED_PIN, LOW);
        }
    }
}

void motorISR() {
  timeThisPulse = micros();
  printBuffer = "b";
  printBuffer += timeThisPulse - timeCommand;
  printBuffer += "m";
  printBuffer += timeThisPulse - timeLastPulse;
  printBuffer += "e";
  Serial.print(printBuffer);
  timeLastPulse = timeThisPulse;
}

void escOut(uint32_t escOut_us) {
  escOutVal = ((escOut_us * ESC_FREQ) / 1000 * PWM_MAX) / 1000;
  Serial.println(escOutVal);
  analogWrite(ESC_PIN_A, escOutVal);
  analogWrite(ESC_PIN_B, escOutVal);
}
