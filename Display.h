#ifndef PUMPER_DIPLAY_H
#define PUMPER_DIPLAY_H

#include "HumiditySensor.h"
#include "Observer.h"
#include "Common.h"
#include "Time.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

class Display: public ILinkableObserver {
public:
  Display(Time * _time)
    : m_lcd(0x3f,16,2)   /* Задаем адрес и размерность дисплея. */
    , m_time(_time)
  {}

  void setup() {
    m_lcd.init();
    m_lcd.noBacklight();
  }
  
  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case HSValue: {
        m_lcd.setCursor(0, additionalData);
        m_lcd.print(value);
        break;
      }
      case PWorkStarted: {
        m_lcd.setCursor(8, 0);
        m_lcd.print(m_time->getTimeStr());
        break;
      }
      case TimeUpdated: {
        m_lcd.setCursor(8, 1);
        m_lcd.print(m_time->getTimeStr());
        break;
      }
      case HSLevelChanged: {
        m_lcd.setCursor(2, additionalData);
        m_lcd.print(value);
      }
    }
  }
private:
  Time * m_time;
  LiquidCrystal_I2C m_lcd;
};

#endif // PUMPER_DIPLAY_H
