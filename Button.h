#ifndef PUMPER_BUTTON_H
#define PUMPER_BUTTON_H

#include "Common.h"
#include "Linker.h"

class Button: public ILinkable {
public:
  Button(Buffer & buffer) {
    buffer.read(m_buttonPin);
    _setup();
  }

  virtual void store(Buffer & buffer) {
    buffer.write(m_buttonPin);
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason == Tick) {
      if (digitalRead(m_buttonPin) == 1) {
        notify(ButtonPushed, 0);
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

