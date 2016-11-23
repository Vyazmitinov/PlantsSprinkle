#ifndef PUMPER_PUMP_H
#define PUMPER_PUMP_H

#include "Linker.h"
#include "Common.h"

const long PumpTicks = 40000 / LoopDelay; // 40s
//const long WaitTicks = 43200000 / LoopDelay; // 12h
const long WaitTicks = 3600000 / LoopDelay; // 1h
const long MaxWorkingTime = 10L * 60L * 1000L / LoopDelay; // 10m

class Pump: public ILinkable {
public:
  const uint8_t ON = LOW;
  const uint8_t OFF = HIGH;

  Pump(Buffer & buffer)
    : m_waitTicks(0)
    , m_state(OFF)
  {
    buffer.read(m_powerPin);
    buffer.read(m_state);
    _setup();
  }

  void store(Buffer & buffer) {
    buffer.write(m_powerPin);
    buffer.write(m_state);
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
    digitalWrite(m_powerPin, m_state);
  }

  void _startWork() {
    if ((m_waitTicks > 0) || (m_state == ON)) {
      return;
    }
    m_state = ON;
    digitalWrite(m_powerPin, m_state);
    notify(PWorkStarted, 0);
  }

  void _work() {
    if (m_waitTicks > 0) {
      --m_waitTicks;
    }
  }

  void _stopWork() {
    if (m_state == OFF) {
      return;
    }
    m_state = OFF;
    digitalWrite(m_powerPin, m_state);
    notify(PWorkStopped);
    m_waitTicks = WaitTicks;
  }

  uint8_t m_powerPin;
  int m_waitTicks;
  uint8_t m_state;
};

#endif // PUMPER_PUMP_H

