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

const long LoopDelay = 600; // 0.6 s

union Size {
  int size_int;
  uint8_t size_ch[2];
};

#endif // PUMPER_COMMON_H

