#ifndef PUMPER_LIGHT_H
#define PUMPER_LIGHT_H

#include "Common.h"
#include "Linker.h"

class Light: public ILinkable {
public:
  const int ON = HIGH;
  const int OFF = LOW;
    
  Light(Buffer & buffer) {
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
      case kLightOn:
        m_state = ON;
        break;
      case kLightOff:
        m_state = OFF;
        break;
      default:
        return;
    }
    digitalWrite(m_powerPin, m_state);
  }
private:
  void _setup() {
    pinMode(m_powerPin, OUTPUT);
    digitalWrite(m_powerPin, OFF);
  }
  uint8_t m_powerPin;
  uint8_t m_state;
};

#endif PUMPER_LIGHT_H

