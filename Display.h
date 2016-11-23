#ifndef PUMPER_DIPLAY_H
#define PUMPER_DIPLAY_H

#include "Linker.h"
#include "Common.h"
#include "Time.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

class Display: public ILinkable {
public:
  Display()
    : m_lcd(0x3f,16,2)   /* Задаем адрес и размерность дисплея. */
  {
    _setup();
  }

  virtual void store(Buffer & buffer) {}

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case HSValue: {
        m_lcd.setCursor(0, additionalData);
        m_lcd.print(value);
        break;
      }
      case PWorkStarted: {
        m_lcd.setCursor(8, 0);
        m_lcd.print(m_lastTime);
        break;
      }
      case TimeUpdated: {
        Time * time = (Time *)value;
        m_lcd.setCursor(8, 1);
        m_lastTime = time->getTimeStr();
        m_lcd.print(m_lastTime);
        break;
      }
      case HSLevelChanged: {
        m_lcd.setCursor(2, additionalData);
        m_lcd.print(value);
      }
    }
  }
private:
  void _setup() {
    m_lcd.init();
    m_lcd.noBacklight();
  }
  LiquidCrystal_I2C m_lcd;
  String m_lastTime;
};

#endif // PUMPER_DIPLAY_H
