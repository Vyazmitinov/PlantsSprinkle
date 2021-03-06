#ifndef PS_TIME_H
#define PS_TIME_H

#include <iarduino_RTC.h>

#include "Common.h"
#include "Object.h"
#include "Linker.h"

#ifdef PS_RTC_TIME

class Time: public Object {
public:
  Time()
    : m_time(RTC_DS3231)
  {
    _setup();
  }

  virtual uint8_t getType() const {return kTime;}

  uint8_t update(const Linker* linker, uint8_t, const void *) {
    m_time.gettime();

    if ((m_second == m_time.seconds) && (m_minute == m_time.minutes) && (m_hour == m_time.Hours)) {
      return 0;
    }

    m_time_str = m_time.gettime((char*)"H:i:s");

    m_hour = m_time.Hours;
    m_minute = m_time.minutes;
    m_second = m_time.seconds;

    m_year = m_time.year;
    m_month = m_time.month;
    m_day = m_time.day;

    return notify(linker, kSigTimeUpdated, this);
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

#endif // PS_RTC_TIME

#endif // PS_TIME_H

