#ifndef PS_TIME_H
#define PS_TIME_H

#include <iarduino_RTC.h>

#include "IObject.h"
#include "Common.h"
#include "Linker.h"

class Time: public IObject {
public:
  Time(VirtualBuffer &)
    : m_time(RTC_DS3231)
  {
    _setup();
  }

  virtual uint8_t getType() const {return kTime;}

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason != kTick) {
      return;
    }

    m_time.gettime();

    if ((m_second == m_time.seconds) && (m_minute == m_time.minutes) && (m_hour == m_time.Hours)) {
      return;
    }

    m_time_str = m_time.gettime((char*)"H:i:s");

    m_hour = m_time.Hours;
    m_minute = m_time.minutes;
    m_second = m_time.seconds;

    m_year = m_time.year;
    m_month = m_time.month;
    m_day = m_time.day;

    Linker::instance()->notify(this, kTimeUpdated, reinterpret_cast<int>(this));
  }
  const String & getTimeStr() const { return m_time_str; }

  inline uint8_t hour() const { return m_hour; }
  inline uint8_t minute() const { return m_minute; }
  inline uint8_t second() const { return m_second; }

  inline uint8_t year() const { return m_year; }
  inline uint8_t day() const { return m_day; }
  inline uint8_t month() const { return m_month; }
private:
  void _setup() {
    m_time.begin();
  }

  String m_time_str;
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  uint8_t m_year;
  uint8_t m_month;
  uint8_t m_day;
  iarduino_RTC m_time;
};

#endif // PS_TIME_H

