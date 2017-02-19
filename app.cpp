#include <EEPROM.h>
#include <Arduino.h>
//#include <LowPower.h>

#include "Common.h"
#include "Linker.h"
#include "Ticker.h"

const uint8_t HSPowerPin = 8;
const uint8_t HSAnalogPin = A3;
const uint8_t HSDefaultLevel = 7;

const int PumpPowerPin = 2;

const int Button0Pin = 6;
const int Button1Pin = 7;

const int PlantLightPowerPin = 4;
const int GralandLightPowerPin = 3;

enum Objects {
  kTikerObj,
  kHSObj0,
  kDisplayObj,
  kPumpObj,
  kTimeObj,
  kButtonObj0,
  kButtonObj1,
  kPlantLightObj,
  kGralandLightObj,
  kSunAlarmObj,
  kGarlandAlarmOn,
  kGarlandAlarmOff,
};

uint8_t objectsMemory[] = {
  kTicker,          kTikerObj,
  kHumiditySensor,  kHSObj0,          HSPowerPin, HSAnalogPin, HSDefaultLevel, 0,
  kDisplay,         kDisplayObj,
  kRelay,           kPumpObj,         PumpPowerPin, 0,
  kTime,            kTimeObj,
  kButton,          kButtonObj0,      Button0Pin,
  kButton,          kButtonObj1,      Button1Pin,
  kRelay,           kPlantLightObj,   PlantLightPowerPin, 0,
  kRelay,           kGralandLightObj, GralandLightPowerPin, 0,
  kSunAlarm,        kSunAlarmObj,     7, 111, 222, 165, 66, 25, 42, 92, 66, 0,
  kAlarm,           kGarlandAlarmOn,  19, 30, 0,
  kAlarm,           kGarlandAlarmOff, 22, 30, 0,
};

uint8_t linksMemory[] = {
  kTikerObj,        kHSObj0,          kSigTick,           kCmdTick,
  kHSObj0,          kDisplayObj,      kSigHSLevelChanged, kCmdUpdateHSLevel,
  kHSObj0,          kDisplayObj,      kSigHSValue,        kCmdUpdateHSValue,
  kHSObj0,          kDisplayObj,      kSigHSDry,          kCmdTurnRelayOn,
  kHSObj0,          kPumpObj,         kSigHSDry,          kCmdTurnRelayOn,
  kHSObj0,          kPumpObj,         kSigHSWet,          kCmdTurnRelayOff,
  kTikerObj,        kTimeObj,         kSigTick,           kCmdTick,
  kTimeObj,         kDisplayObj,      kSigTimeUpdated,    kCmdUpdateTime,
  kButtonObj0,      kHSObj0,          kSigButtonPushed,   kCmdLevelDown,
  kButtonObj1,      kHSObj0,          kSigButtonPushed,   kCmdLevelUp,
  kTikerObj,        kButtonObj0,      kSigTick,           kCmdTick,
  kTikerObj,        kButtonObj1,      kSigTick,           kCmdTick,
  kTimeObj,         kSunAlarmObj,     kSigTimeUpdated,    kCmdUpdateTime,
  kSunAlarmObj,     kPlantLightObj,   kSigStartLighting,  kCmdTurnRelayOn,
  kSunAlarmObj,     kPlantLightObj,   kSigStopLighting,   kCmdTurnRelayOff,
  kTimeObj,         kGarlandAlarmOn,  kSigTimeUpdated,    kCmdUpdateTime,
  kTimeObj,         kGarlandAlarmOff, kSigTimeUpdated,    kCmdUpdateTime,
  kGarlandAlarmOn,  kGralandLightObj, kSigAlarmOccured,   kCmdTurnRelayOn,
  kGarlandAlarmOff, kGralandLightObj, kSigAlarmOccured,   kCmdTurnRelayOff
};

Buffer objectsBuffer(objectsMemory, sizeof(objectsMemory));
Buffer linksBuffer(linksMemory, sizeof(linksMemory));

Ticker * MainTicker = NULL;
ObjectStore MainStore;
Linker MainLinker(&MainStore);

void setup() {
  MainStore.read(objectsBuffer);
  MainLinker.read(linksBuffer);
  MainTicker = reinterpret_cast<Ticker*>(MainStore.getObject(kTikerObj));
}

void loop() {
  uint8_t rv = MainTicker->tick(&MainLinker);
  if (rv & kNeedShortDelay) {
    delay(15);
//    LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
  } else {
    delay(1000);
//    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }
}
