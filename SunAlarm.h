#ifndef PS_SUN_ALARM_H
#define PS_SUN_ALARM_H

#include "Common.h"
#include "Object.h"
#include "Time.h"

#ifdef PS_SUN_ALARM

class SunAlarm: public Object {
  const uint8_t HoursBeforeSunrise = 5;
  const uint8_t HoursAfterSunset = 2;

  struct SimpleTime {
    int8_t hour;
    int8_t minute;

    SimpleTime(int8_t _hour = 0, int8_t _minute = 0)
      : hour(_hour)
      , minute(_minute)
    {}

    bool operator < (const SimpleTime &r) {
      return (hour < r.hour)
          || ((hour == r.hour) && (minute < r.minute));
    }

    const SimpleTime operator + (const SimpleTime &r) const {
      SimpleTime time(this->hour + r.hour, this->minute + r.minute);
      if (time.minute >= 60) {
        ++time.hour;
        time.minute -= 60;
      }
      if (time.hour >= 24) {
        time.hour -= 24;
      }
      return time;
    }

    const SimpleTime operator - (const SimpleTime &r) const {
      SimpleTime time(this->hour - r.hour, this->minute - r.minute);
      if (time.minute < 0) {
        --time.hour;
        time.minute += 60;
      }
      if (time.hour < 0) {
        time.hour += 24;
      }
      return time;
    }

  };

public:
  virtual uint8_t getType() const {return kSunAlarm;}

  virtual uint8_t read(VirtualBuffer &buffer);
  virtual uint8_t write(VirtualBuffer & buffer);

  virtual uint8_t update(const Linker *linker, uint8_t command, const void * data);

private:
  void _calcSunTimes(Time * time);

  int8_t m_timezone;
  float m_longitude;
  float m_latitude;
  int8_t m_state;

  SimpleTime m_sunrise;
  SimpleTime m_sunset;
  SimpleTime m_beforeSunrise;
  SimpleTime m_afterSunset;
};

#endif // PS_SUN_ALARM

#endif // PS_SUN_ALARM_H
