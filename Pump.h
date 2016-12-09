#ifndef PUMPER_PUMP_H
#define PUMPER_PUMP_H

#include "IObject.h"
#include "Common.h"

const long PumpTicks = 40000 / LoopDelay; // 40s
//const long WaitTicks = 43200000 / LoopDelay; // 12h
const long WaitTicks = 3600000 / LoopDelay; // 1h
const long MaxWorkingTime = 10L * 60L * 1000L / LoopDelay; // 10m

class Pump: public IObject {
public:
  const uint8_t ON = HIGH;
  const uint8_t OFF = LOW;

  Pump(VirtualBuffer & buffer)
    : m_waitTicks(0)
    , m_state(OFF)
  {
    buffer.read(&m_powerPin, sizeof(m_powerPin));
    buffer.read(&m_state, sizeof(m_state));
    _setup();
  }

  uint8_t getType() {return kPump;}

  void store(VirtualBuffer & buffer) {
    buffer.write(&m_powerPin, sizeof(m_powerPin));
    buffer.write(&m_state, sizeof(m_state));
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case kTick: {
        _work();
        break;
      }
      case kHSDry: {
        _startWork();
        break;
      }
      case kHSWet: {
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
    Linker::instance()->notify(this, kPWorkStarted);
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
    Linker::instance()->notify(this, kPWorkStopped);
    m_waitTicks = WaitTicks;
  }

  uint8_t m_powerPin;
  int m_waitTicks;
  uint8_t m_state;
};

#endif // PUMPER_PUMP_H

