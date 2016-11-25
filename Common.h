#ifndef PUMPER_COMMON_H
#define PUMPER_COMMON_H

enum {
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
  kLightOn
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
  kSunAlarm
};

const long LoopDelay = 600; // 0.6 s

#endif // PUMPER_COMMON_H

