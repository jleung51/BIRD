/*
 *
 * Authors: Jeffrey Leung, Charles Li, Mina Li, Paul Pinto
 * Last edited: 2016-03-19
 *
 * This Arduino sketch detects the activation of pressure pads by the presence
 * of an item and sends the result using Bluetooth communication to the
 * car system.
 *
 */

#include <SoftwareSerial.h>

const unsigned int pinPressureEssential = A0;
const unsigned int pinPressureGeneral = A1;
const unsigned int pinCapacitive = A2;

const unsigned int pinPressureEssentialPower = 7;
const unsigned int pinPressureGeneralPower = 6;
const unsigned int pinCapacitivePower = 5;

const unsigned int pinPowerLED = 13;
const unsigned int pinEssentialLED = 12;
const unsigned int pinGeneralLED = 11;

// Calibrated to the individual sensors
const unsigned int thresholdPressureEssential = 700;
const unsigned int thresholdPressureGeneral = 700;
const unsigned int thresholdCapacitive = 200;

// States
bool powerOn = true;
bool powerToggled = false;

static unsigned long minuteCounter;
unsigned long delay_hours;
unsigned long delay_minutes;

// Serial connections
SoftwareSerial serialCarSystem(2, 3);  // Pins 2, 3

// This method returns whether the given input pin reads greater than
// the given threshold.
bool SensorGreaterThan(const unsigned int pin, const unsigned int threshold) {
  return analogRead(pin) > threshold;
}

// This method counts down by a single minute when a minute has passed.
void MinuteSet() {
  if(minuteCounter < millis()) {

    if(delay_minutes == 0 && delay_hours > 0) {
      --delay_hours;
      delay_minutes = 59;
    }
    else if(delay_minutes > 0) {
      --delay_minutes;
    }

    minuteCounter = millis()+(60*1000);
  }
}

// This method sets a new delay time.
void SetDelayTime(const unsigned long hours, const unsigned long minutes) {
  delay_hours = hours;
  delay_minutes = minutes;
}

void setup() {
  Serial.begin(9600);
  serialCarSystem.begin(9600);

  // Pin outputs
  pinMode(pinPressureEssential, INPUT);
  pinMode(pinPressureGeneral, INPUT);

  pinMode(pinPressureEssentialPower, OUTPUT);
  pinMode(pinPressureGeneralPower, OUTPUT);
  pinMode(pinCapacitivePower, OUTPUT);
  digitalWrite(pinPressureEssentialPower, HIGH);
  digitalWrite(pinPressureGeneralPower, HIGH);
  digitalWrite(pinCapacitivePower, HIGH);

  pinMode(pinPowerLED, OUTPUT);
  pinMode(pinEssentialLED, OUTPUT);
  pinMode(pinGeneralLED, OUTPUT);
  digitalWrite(pinPowerLED, HIGH);
  digitalWrite(pinEssentialLED, LOW);
  digitalWrite(pinGeneralLED, LOW);

  minuteCounter = millis()+(60*1000);
}

void loop(){
  