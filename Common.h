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

const long LoopDelay = 600; // 0.6 s

#endif // PUMPER_COMMON_H

