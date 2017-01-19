#ifndef PS_BUTTON_H
#define PS_BUTTON_H

#include "IObject.h"
#include "Common.h"

class Button: public IObject {
public:
  Button(VirtualBuffer & buffer) {
    buffer.read(&m_buttonPin, sizeof(m_buttonPin));
    _setup();
  }

  virtual uint8_t getType() const {return kButton;}

  virtual void store(VirtualBuffer & buffer) {
    buffer.write(&m_buttonPin, sizeof(m_buttonPin));
  }

  virtual uint8_t update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason == kTick) {
      if (digitalRead(m_buttonPin) == 1) {
        return Linker::instance()->notify(this, kButtonPushed);
      }
    }
    return 0;
  }

private:
  void _setup() {
    pinMode(m_buttonPin, INPUT);
  }

  uint8_t m_buttonPin;
};

#endif // PS_BUTTON_H

