#ifndef PUMPER_TIME_H
#define PUMPER_TIME_H

#include <iarduino_RTC.h>

#include "Observer.h"
#include "Common.h"

class Time: public ILinkableObserver, public ILinkableSubject, public ISerializable {
  public:
    Time() 
      : m_time(RTC_DS3231)
    {
      _setup();
    }

    virtual void store(Buffer & buffer) {}

    virtual void update(uint8_t reason, int value, uint8_t additionalData) {
      if (reason != Tick) {
        return;
      }
      
      m_time_str = m_time.gettime("H:i:s");
      m_time.gettime();
      
      if ((m_second == m_time.seconds) && (m_minute == m_time.minutes) && (m_hour == m_time.Hours)) {
        return;
      }
      
      m_hour = m_time.Hours;
      m_minute = m_time.minutes;
      m_second = m_time.seconds;

      notify(TimeUpdated, this);
    }
    const String & getTimeStr() const { return m_time_str; }
    uint8_t hour() const { return m_hour; }
    uint8_t minute() const { return m_minute; }
    uint8_t second() const { return m_second; }
  private:
    void _setup() {
      m_time.begin();
    }

    String m_time_str;
    uint8_t m_hour;
    uint8_t m_minute;
    uint8_t m_second;
    iarduino_RTC m_time;
};

#endif // PUMPER_TIME_H

