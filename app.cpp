#include <EEPROM.h>
#include <Arduino.h>
#include <LowPower.h>

#include "Common.h"
#include "Linker.h"
#include "Ticker.h"

const uint8_t HS0PowerPin = 8;
const uint8_t HS1PowerPin = 9;
const uint8_t HS0AnalogPin = A3;
const uint8_t HS1AnalogPin = A2;
const uint8_t HSDefaultLevel = 7;

const int P0PowerPin = 2;

const int BP0Pin = 6;
const int BP1Pin = 7;
const int BP2Pin = 10;
const int BP3Pin = 11;

const int Light0PowerPin = 4;
const int Light1PowerPin = 3;

enum Objects {
  kTikerObj,
  kHSObj0,
  kHSObj1,
  kDisplayObj,
  kPumpObj0,
  kTimeObj,
  kButtonObj0,
  kButtonObj1,
  kButtonObj2,
  kButtonObj3,
  kLightObj0,
  kLightObj1,
  kSunAlarmObj,
  kSerializerObj,
  kGarlandAlarmOn,
  kGarlandAlarmOff,
};

uint8_t memory[] = {
  kTicker,
  kHumiditySensor, HS0PowerPin, HS0AnalogPin, HSDefaultLevel, 0,
  kHumiditySensor, HS1PowerPin, HS1AnalogPin, HSDefaultLevel, 0,
  kDisplay,
  kPump, P0PowerPin, HIGH,
  kTime,
  kButton, BP0Pin,
  kButton, BP1Pin,
  kButton, BP2Pin,
  kButton, BP3Pin,
  kLight, Light0PowerPin, 0,
  kLight, Light1PowerPin, 0,
  kSunAlarm, 7, 111, 222, 165, 66, 25, 42, 92, 66, 0,
  kSerializer,
  kAlarm, 19, 30, 0,
  kAlarm, 22, 30, 0,
  
  kLink, kTikerObj,     kHSObj0,        0,
  kLink, kTikerObj,     kHSObj1,        0,
  kLink, kHSObj0,       kDisplayObj,    0,
  kLink, kHSObj1,       kDisplayObj,    1,
  kLink, kTikerObj,     kPumpObj0,      0,
  kLink, kPumpObj0,     kDisplayObj,    0,
  kLink, kHSObj0,       kPumpObj0,      0,
  kLink, kTikerObj,     kTimeObj,       0,
  kLink, kTimeObj,      kDisplayObj,    0,
  kLink, kButtonObj0,   kHSObj0,        kButtonDown,
  kLink, kButtonObj1,   kHSObj0,        kButtonUp,
  kLink, kButtonObj2,   kHSObj1,        kButtonDown,
  kLink, kButtonObj3,   kHSObj1,        kButtonUp,
  kLink, kTikerObj,     kButtonObj0,    0,
  kLink, kTikerObj,     kButtonObj1,    0,
  kLink, kTikerObj,     kButtonObj2,    0,
  kLink, kTikerObj,     kButtonObj3,    0,
  kLink, kTimeObj,      kSunAlarmObj,   0,
  kLink, kSunAlarmObj,  kLightObj0,     0,
  kLink, kTikerObj,     kSerializerObj, 0,
  kLink, kHSObj0,       kSerializerObj, 0,
  kLink, kHSObj1,       kSerializerObj, 0,
  kLink, kTimeObj,      kGarlandAlarmOn,  0,
  kLink, kTimeObj,      kGarlandAlarmOff, 0,
  kLink, kGarlandAlarmOn,  kLightObj1,  kLightOn,
  kLink, kGarlandAlarmOff, kLightObj1,  kLightOff,
};

//uint8_t memory[] = {
//  kTicker,
//  kHumiditySensor, HS0PowerPin, HS0AnalogPin, HSDefaultLevel, 0,
//  kPump, P0PowerPin, HIGH,

//  kLink, kTikerObj,     kHSObj0,        0,
//  kLink, kTikerObj,     kPumpObj0,      0,
//  kLink, kHSObj0,       kPumpObj0,      0,
//};

Buffer buffer(memory, sizeof(memory));

//const uint8_t memory[] = {
//  kTicker,
//  kSerializer,
//  
//  kLink, kTikerObj,     kSerializerObj, 0,
//};
//
//Buffer buffer(memory, sizeof(memory));

Ticker * MainTicker = NULL;

void setup() {
  Serial.begin(9600);

  //  EepromBuffer ebuf;
  //  Linker::instance()->load(ebuf);
  Linker::instance()->load(buffer);

  MainTicker = reinterpret_cast<Ticker*>(Linker::instance()->getObject(kTikerObj));
}

void loop() {
  uint8_t rv = MainTicker->tick();
  if (rv | kNeedShortDelay) {
    LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
  } else {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }
}
