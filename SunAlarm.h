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
    uint8_t hour;
    uint8_t minute;

    SimpleTime(uint8_t _hour = 0, uint8_t _minute = 0)
      : hour(_hour)
      , minute(_minute)
    {}

    bool operator < (const SimpleTime &r) {
      return (hour < r.hour)
          || ((hour == r.hour) && (minute < r.minute));
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
