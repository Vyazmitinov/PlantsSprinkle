#ifndef PS_DISPLAY_H
#define PS_DISPLAY_H

#include "Object.h"
#include "Common.h"
#include "Time.h"

#ifdef PS_DISPLAY

#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

class Display: public Object {
public:
  Display()
    : m_lcd(0x3f,16,2)   /* Задаем адрес и размерность дисплея. */
  {
    _setup();
  }

  virtual uint8_t getType() const {return kDisplay;}

  virtual uint8_t update(const Linker *, uint8_t command, const void * data) {
    switch (command) {
      case kCmdUpdateHSValue: {
        m_lcd.setCursor(0, 0);
        m_lcd.print(*(reinterpret_cast<const uint8_t*>(data)));
        break;
      }
      case kCmdTurnRelayOn: {
        m_lcd.setCursor(8, 0);
        m_lcd.print(m_lastTime);
        break;
      }
      case kCmdUpdateTime: {
        const Time * time = reinterpret_cast<const Time *>(data);
        m_lcd.setCursor(8, 1);
        m_lastTime = time->getTimeStr();
        m_lcd.print(m_lastTime);
        break;
      }
      case kCmdUpdateHSLevel: {
        m_lcd.setCursor(2, 0);
        m_lcd.print(*(reinterpret_cast<const uint8_t*>(data)));
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

#endif // PS_DISPLAY

#endif // PS_DISPLAY_H
