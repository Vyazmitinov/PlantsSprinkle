#ifndef PS_RELAY_H
#define PS_RELAY_H

#include "Common.h"
#include "Object.h"

#ifdef PS_RELAY

class Relay: public Object {
public:
  const int ON = HIGH;
  const int OFF = LOW;

  uint8_t getType() const {return kRelay;}

  virtual uint8_t read(VirtualBuffer & buffer) {
    Object::read(buffer);
    buffer.read(&m_powerPin, sizeof(m_powerPin));
    buffer.read(&m_state, sizeof(m_state));
    _setup();
    return 3;
  }

  virtual uint8_t write(VirtualBuffer & buffer) {
    Object::write(buffer);
    buffer.write(&m_powerPin, sizeof(m_powerPin));
    buffer.write(&m_state, sizeof(m_state));
    return 3;
  }

  virtual uint8_t update(const Linker*, uint8_t command, const void *) {
    switch (command) {
    case kCmdTurnRelayOn:
      m_state = ON;
      break;
    case kCmdTurnRelayOff:
      m_state = OFF;
      break;
    default:
      return 0;
    }
    digitalWrite(m_powerPin, m_state);
    return 0;
  }
private:
  void _setup() {
    pinMode(m_powerPin, OUTPUT);
    digitalWrite(m_powerPin, OFF);
  }
  uint8_t m_powerPin;
  uint8_t m_state;
};

#endif // PS_RELAY

#endif // PS_RELAY_H
