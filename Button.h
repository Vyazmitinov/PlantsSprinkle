#ifndef PUMPER_BUTTON_H
#define PUMPER_BUTTON_H

#include "Common.h"
#include "Observer.h"

class Button: public ILinkableObserver, public ILinkableSubject {
public:
  struct Data {
    uint8_t destinity;
  };
  Button(uint8_t pin, uint8_t destinity) 
    : m_buttonPin(pin)
    , m_destinity(destinity)
  {}

  void setup() {
    pinMode(m_buttonPin, INPUT);
  }
  
  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason == Tick) {
      if (digitalRead(m_buttonPin) == 1) {
        Data data{m_destinity};
        notify(ButtonPushed, &data);
      }
    }
  }

private:

  uint8_t m_buttonPin;
  uint8_t m_destinity;
};

#endif // PUMPER_BUTTON_H

