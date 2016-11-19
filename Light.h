#ifndef PUMPER_LIGHT_H
#define PUMPER_LIGHT_H

#include "Common.h"
#include "Observer.h"

class Light: public ILinkableObserver {
  public:
    const int ON = HIGH;
    const int OFF = LOW;
    
    Light(uint8_t _powerPin)
      : m_powerPin(_powerPin)
    {}
    void setup() {
      pinMode(m_powerPin, OUTPUT);
      digitalWrite(m_powerPin, OFF);
    }
  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
      switch (reason) {
        case MorningStarted:
        case EveningStarted: {
          digitalWrite(m_powerPin, ON);
          break;
        }
        case MorningStopped:
        case EveningStopped: {
          digitalWrite(m_powerPin, OFF);
          break;
        }
      }
    }
  private:
    uint8_t m_powerPin;
};

#endif PUMPER_LIGHT_H

