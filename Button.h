#ifndef PS_BUTTON_H
#define PS_BUTTON_H

#include "Object.h"
#include "Common.h"

#ifdef PS_BUTTON

class Button: public Object {
public:
  virtual uint8_t getType() const {return kButton;}

  virtual uint8_t read(VirtualBuffer & buffer) {
    Object::read(buffer);
    buffer.read(&m_buttonPin, sizeof(m_buttonPin));
    _setup();
    return 2;
  }

  virtual uint8_t write(VirtualBuffer & buffer) {
    Object::write(buffer);
    buffer.write(&m_buttonPin, sizeof(m_buttonPin));
    return 2;
  }

  virtual uint8_t update(const Linker* linker, uint8_t, const void *) {
    if (digitalRead(m_buttonPin) == 1) {
      return notify(linker, kSigButtonPushed, nullptr);
    }
    return 0;
  }

private:
  void _setup() {
    pinMode(m_buttonPin, INPUT);
  }

  uint8_t m_buttonPin;
};

#endif // PS_BUTTON

#endif // PS_BUTTON_H

