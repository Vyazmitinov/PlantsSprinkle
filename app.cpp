#include <EEPROM.h>
#include <Arduino.h>
#include <LowPower.h>

#include "Common.h"
#include "Linker.h"
#include "Ticker.h"

const uint8_t HS0PowerPin = 8;
const uint8_t HS0AnalogPin = A3;
const uint8_t HSDefaultLevel = 7;

const int P0PowerPin = 2;

const int BP0Pin = 6;
const int BP1Pin = 7;

const int Relay0PowerPin = 4;
const int Light1PowerPin = 3;

enum Objects {
  kTikerObj,
  kHSObj0,
  kDisplayObj,
  kPumpObj0,
  kTimeObj,
  kButtonObj0,
  kButtonObj1,
  kRelayObj,
  kLightObj1,
  kSunAlarmObj,
  kGarlandAlarmOn,
  kGarlandAlarmOff,
};

uint8_t objectsMemory[] = {
  kTicker,          kTikerObj,
  kHumiditySensor,  kHSObj0,          HS0PowerPin, HS0AnalogPin, HSDefaultLevel, 0,
  kDisplay,         kDisplayObj,
  kRelay,           kPumpObj0,        P0PowerPin, 0,
  kTime,            kTimeObj,
  kButton,          kButtonObj0,      BP0Pin,
  kButton,          kButtonObj1,      BP1Pin,
  kRelay,           kRelayObj,        Relay0PowerPin, 0,
  kRelay,           kLightObj1,       Light1PowerPin, 0,
  kSunAlarm,        kSunAlarmObj,     7, 111, 222, 165, 66, 25, 42, 92, 66, 0,
  kAlarm,           kGarlandAlarmOn,  19, 30, 0,
  kAlarm,           kGarlandAlarmOff, 22, 30, 0,
};

uint8_t linksMemory[] = {
  kTikerObj,        kHSObj0,          kSigTick,           kCmdTick,
  kHSObj0,          kDisplayObj,      kSigHSLevelChanged, kCmdUpdateHSLevel,
  kHSObj0,          kDisplayObj,      kSigHSValue,        kCmdUpdateHSValue,
  kHSObj0,          kDisplayObj,      kSigHSDry,          kCmdTurnRelayOn,
  kHSObj0,          kPumpObj0,        kSigHSDry,          kCmdTurnRelayOn,
  kHSObj0,          kPumpObj0,        kSigHSWet,          kCmdTurnRelayOff,
  kTikerObj,        kTimeObj,         kSigTick,           kCmdTick,
  kTimeObj,         kDisplayObj,      kSigTimeUpdated,    kCmdUpdateTime,
  kButtonObj0,      kHSObj0,          kSigButtonPushed,   kCmdLevelDown,
  kButtonObj1,      kHSObj0,          kSigButtonPushed,   kCmdLevelUp,
  kTikerObj,        kButtonObj0,      kSigTick,           kCmdTick,
  kTikerObj,        kButtonObj1,      kSigTick,           kCmdTick,
  kTimeObj,         kSunAlarmObj,     kSigTimeUpdated,    kCmdUpdateTime,
  kSunAlarmObj,     kRelayObj,        kSigStartLighting,  kCmdTurnRelayOn,
  kSunAlarmObj,     kRelayObj,        kSigStopLighting,   kCmdTurnRelayOff,
  kTimeObj,         kGarlandAlarmOn,  kSigTimeUpdated,    kCmdUpdateTime,
  kTimeObj,         kGarlandAlarmOff, kSigTimeUpdated,    kCmdUpdateTime,
  kGarlandAlarmOn,  kLightObj1,       kSigAlarmOccured,   kCmdTurnRelayOn,
  kGarlandAlarmOff, kLightObj1,       kSigAlarmOccured,   kCmdTurnRelayOff
};

Buffer objectsBuffer(objectsMemory, sizeof(objectsMemory));
Buffer linksBuffer(linksMemory, sizeof(linksMemory));

Ticker * MainTicker = NULL;
ObjectStore MainStore;
Linker MainLinker(&MainStore);

void setup() {
  Serial.begin(9600);

  MainStore.read(objectsBuffer);
  MainLinker.read(linksBuffer);

  MainTicker = reinterpret_cast<Ticker*>(MainStore.getObject(kTikerObj));
}

void loop() {
  uint8_t rv = MainTicker->tick(&MainLinker);
  delay(10);
  if (rv & kNeedShortDelay) {
    LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
  } else {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }
}
