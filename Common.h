#ifndef PUMPER_COMMON_H
#define PUMPER_COMMON_H

enum CommandType {
  kHSDry,
  kHSWet,
  kHSValue,
  kHSLevelChanged,
  kPWorkStarted,
  kPWorkStopped,
  kTick,
  kButtonPushed,
  kButtonUp,
  kButtonDown,
  kTimeUpdated,
  kAlarmOccured,
  kLightOff,
  kLightOn,
  kNeedToSerialize
};

enum ObjectType {
  kNoObject,
  kLink,
  kTicker,
  kHumiditySensor,
  kDisplay,
  kPump,
  kTime,
  kAlarm,
  kButton,
  kLight,
  kSunAlarm,
  kSerializer
};

enum TickEvent {
  kNeedShortDelay = 1
};

const long ShortLoopDelay = 15; // SLEEP_15MS
const long RegularLoopDelay = 1000; // SLEEP_1S

#endif // PUMPER_COMMON_H

