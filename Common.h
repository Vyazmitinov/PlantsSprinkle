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
  MorningStarted,
  MorningStopped,
  EveningStarted,
  EveningStopped
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
  E_Light
};

const long LoopDelay = 600; // 0.6 s

#endif // PUMPER_COMMON_H

