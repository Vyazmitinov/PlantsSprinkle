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

uint8_t Pump::update(uint8_t reason, int value, uint8_t additionalData) {
  switch (reason) {
    case kTick:
      return _work();
    case kHSDry:
      return _startWork();
    case kHSWet:
      return _stopWork();
  }
  return 0;
}

void Pump::_setup() {
  pinMode(m_powerPin, OUTPUT);
  digitalWrite(m_powerPin, m_state);
}

uint8_t Pump::_startWork() {
  if ((m_waitTicks > 0) || (m_state == ON)) {
    return 0;
  }
  m_state = ON;
  digitalWrite(m_powerPin, m_state);
  return Linker::instance()->notify(this, kPWorkStarted);
}

uint8_t Pump::_work() {
  if (m_waitTicks > 0) {
    --m_waitTicks;
  }
  return 0;
}

uint8_t Pump::_stopWork() {
  if (m_state == OFF) {
    return 0;
  }
  m_state = OFF;
  digitalWrite(m_powerPin, m_state);
  m_waitTicks = WaitTicks;
  return Linker::instance()->notify(this, kPWorkStopped);
}
