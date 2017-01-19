#ifndef PS_DIPLAY_H
#define PS_DIPLAY_H

#include "IObject.h"
#include "Common.h"
#include "Time.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

class Display: public IObject {
public:
  Display(VirtualBuffer &)
    : m_lcd(0x3f,16,2)   /* Задаем адрес и размерность дисплея. */
  {
    _setup();
  }

  virtual uint8_t getType() const {return kDisplay;}

  virtual uint8_t update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case kHSValue: {
        m_lcd.setCursor(0, additionalData);
        m_lcd.print(value);
        break;
      }
      case kPWorkStarted: {
        m_lcd.setCursor(8, 0);
        m_lcd.print(m_lastTime);
        break;
      }
      case kTimeUpdated: {
        Time * time = (Time *)value;
        m_lcd.setCursor(8, 1);
        m_lastTime = time->getTimeStr();
        m_lcd.print(m_lastTime);
        break;
      }
      case kHSLevelChanged: {
        m_lcd.setCursor(2, additionalData);
        m_lcd.print(value);
      }
    }
    return 0;
  }
private:
  void _setup() {
    m_lcd.init();
    m_lcd.backlight();
  }
  LiquidCrystal_I2C m_lcd;
  String m_lastTime;
};

#endif // PS_DIPLAY_H
