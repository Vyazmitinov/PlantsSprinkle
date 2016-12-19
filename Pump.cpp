#include "Pump.h"

#include "Linker.h"

Pump::Pump(VirtualBuffer &buffer)
  : m_waitTicks(0)
  , m_state(OFF)
{
  buffer.read(&m_powerPin, sizeof(m_powerPin));
  buffer.read(&m_state, sizeof(m_state));
  _setup();
}

void Pump::store(VirtualBuffer &buffer) {
  buffer.write(&m_powerPin, sizeof(m_powerPin));
  buffer.write(&m_state, sizeof(m_state));
}

void Pump::update(uint8_t reason, int value, uint8_t additionalData) {
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

void Pump::_setup() {
  pinMode(m_powerPin, OUTPUT);
  digitalWrite(m_powerPin, m_state);
}

void Pump::_startWork() {
  if ((m_waitTicks > 0) || (m_state == ON)) {
    return;
  }
  m_state = ON;
  digitalWrite(m_powerPin, m_state);
  Linker::instance()->notify(this, kPWorkStarted);
}

void Pump::_work() {
  if (m_waitTicks > 0) {
    --m_waitTicks;
  }
}

void Pump::_stopWork() {
  if (m_state == OFF) {
    return;
  }
  m_state = OFF;
  digitalWrite(m_powerPin, m_state);
  Linker::instance()->notify(this, kPWorkStopped);
  m_waitTicks = WaitTicks;
}
