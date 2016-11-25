#ifndef PUMPER_COMMON_H
#define PUMPER_COMMON_H

enum {
  HSDry,
  HSWet,
  HSValue,
  HSLevelChanged,
  PWorkStarted,
  PWorkStopped,
  Tick,
  ButtonPushed,
  ButtonUp,
  ButtonDown,
  TimeUpdated,
  AlarmOccured,
  MorningStarted,
  MorningStopped,
  EveningStarted,
  EveningStopped,
  LightOff,
  LightOn
};

enum ObjectType {
  E_NoObject,
  E_Link,
  E_Ticker,
  E_HumiditySensor,
  E_Display,
  E_Pump,
  E_Time,
  E_Alarm,
  E_Button,
  E_Light,
  E_SunAlarm
};

const long LoopDelay = 600; // 0.6 s

#endif // PUMPER_COMMON_H

