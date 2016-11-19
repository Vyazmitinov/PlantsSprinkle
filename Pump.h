#ifndef PUMPER_PUMP_H
#define PUMPER_PUMP_H

#include "Observer.h"
#include "Common.h"

const long PumpTicks = 40000 / LoopDelay; // 40s
//const long WaitTicks = 43200000 / LoopDelay; // 12h
const long WaitTicks = 3600000 / LoopDelay; // 1h
const long MaxWorkingTime = 10L * 60L * 1000L / LoopDelay; // 10m

class Pump: public ILinkableObserver, public ILinkableSubject {
public:
  const int ON = LOW;
  const int OFF = HIGH;

  Pump(Buffer & buffer)
    : m_waitTicks(0)
    , m_pumping(false)
  {
    buffer.read(m_powerPin);
    _setup();
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case Tick: {
        _work();
        break;
      }
      case HSDry: {
        _startWork();
        break;
      }
      case HSWet: {
        _stopWork();
        break;
      }
    }
  }
    
private:
  void _setup() {
    pinMode(m_powerPin, OUTPUT);
    digitalWrite(m_powerPin, OFF);
  }

  void _startWork() {
    if (m_waitTicks > 0) {
      return;
    }
    if (m_pumping == true) {
      return;
    }
    m_pumping = true;
    digitalWrite(m_powerPin, ON);
    notify(PWorkStarted, 0);
  }

  void _work() {
    if (m_waitTicks > 0) {
      --m_waitTicks;
    }
  }

  void _stopWork() {
    if (m_pumping == false) {
      return;
    }
    m_pumping = false;
    digitalWrite(m_powerPin, OFF);
    notify(PWorkStopped);
    m_waitTicks = WaitTicks;
  }

  uint8_t m_powerPin;
  int m_waitTicks;
  bool m_pumping;
};

#endif // PUMPER_PUMP_H

