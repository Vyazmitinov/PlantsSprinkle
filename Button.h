#ifndef PUMPER_BUTTON_H
#define PUMPER_BUTTON_H

#include "IObject.h"
#include "Common.h"

class Button: public IObject {
public:
  uint8_t getType() {return kButton;}

  Button(VirtualBuffer & buffer) {
    buffer.read(&m_buttonPin, sizeof(m_buttonPin));
    _setup();
  }

  virtual void store(VirtualBuffer & buffer) {
    buffer.write(&m_buttonPin, sizeof(m_buttonPin));
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason == kTick) {
      if (digitalRead(m_buttonPin) == 1) {
        Linker::instance()->notify(this, kButtonPushed);
      }
    }
  }

private:
  void _setup() {
    pinMode(m_buttonPin, INPUT);
  }

  uint8_t m_buttonPin;
};

#endif // PUMPER_BUTTON_H

