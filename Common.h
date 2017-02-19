#ifndef PUMPER_COMMON_H
#define PUMPER_COMMON_H

#define PS_RTC_TIME
#define PS_RELAY
#define PS_SUN_ALARM
#define PS_HUMIDITY_SENSOR
#define PS_ORDINAL_ALARM
#define PS_BUTTON
#define PS_DISPLAY
//#define PS_SERIALIZER

enum SignalType {
  // Ticker
  kSigTick,
  // Humidity sensor
  kSigHSDry,
  kSigHSWet,
  kSigHSValue,
  kSigHSLevelChanged,
  // Button
  kSigButtonPushed,
  // RTC time
  kSigTimeUpdated,
  // Ordinal alarm
  kSigAlarmOccured,
  // Sun alarm
  kSigStartLighting,
  kSigStopLighting,
};

enum CommandType {
  // Almost all
  kCmdTick,
  // Display
  kCmdUpdateHSValue,
  kCmdUpdateHSLevel,
  // Humidity sensor
  kCmdLevelUp,
  kCmdLevelDown,
  // Alarms and display
  kCmdUpdateTime,
  // Relay
  kCmdTurnRelayOn,
  kCmdTurnRelayOff,
  // Serializer
  kCmdSerialize
};

enum ObjectType {
  kNoObject,
  kTicker,
  kHumiditySensor,
  kDisplay,
  kTime,
  kAlarm,
  kButton,
  kRelay,
  kSunAlarm,
  kSerializer
};

enum TickEvent {
  kNeedShortDelay = 1
};

const long ShortLoopDelay = 15; // SLEEP_15MS
const long RegularLoopDelay = 1000; // SLEEP_1S

#endif // PUMPER_COMMON_H

