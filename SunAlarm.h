#ifndef PUMPER_SUN_ALARM
#define PUMPER_SUN_ALARM

#include "IObject.h"
#include "Common.h"
#include "SunTime.h"

class SunAlarm: public IObject {
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
  SunAlarm(VirtualBuffer & buffer) {
    buffer.read(&m_timezone, sizeof(m_timezone));
    buffer.read(&m_longitude, sizeof(m_longitude));
    buffer.read(&m_latitude, sizeof(m_latitude));
    buffer.read(&m_state, sizeof(m_state));
  }

  uint8_t getType() {return kSunAlarm;}

  virtual void store(VirtualBuffer & buffer) {
    buffer.write(&m_timezone, sizeof(m_timezone));
    buffer.write(&m_longitude, sizeof(m_longitude));
    buffer.write(&m_latitude, sizeof(m_latitude));
    buffer.write(&m_state, sizeof(m_state));
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason != kTimeUpdated) {
      return;
    }
    Time * time = (Time *)value;
    if (((time->hour() == 0) && (time->minute() == 0) && (time->second() == 0))
      || ((m_sunrise.hour == 0) && (m_sunrise.minute == 0)))
    {
      _calcSunTimes(time);
    }

    SimpleTime stime{time->hour(), time->minute()};

    uint8_t newState = kLightOff;
    if (stime < m_beforeSunrise) {
      newState = kLightOff;
    } else if (stime < m_sunrise) {
      newState = kLightOn;
    } else if (stime < m_sunset) {
      newState = kLightOff;
    } else if (stime < m_afterSunset) {
      newState = kLightOn;
    }

    if (newState != m_state) {
      m_state = newState;
      notify(m_state);
    }
  }
private:
  void _calcSunTimes(Time * time) {
    float sunriseTime = getSunTime(time->day(), time->month(), time->year(), true, m_timezone, m_longitude, m_latitude);
    float sunsetTime = getSunTime(time->day(), time->month(), time->year(), false, m_timezone, m_longitude, m_latitude);

    m_sunrise.hour = sunriseTime;
    m_sunrise.minute = getMinutes(sunriseTime);
    m_sunset.hour = sunsetTime;
    m_sunset.minute = getMinutes(sunsetTime);

    m_beforeSunrise.hour = sunriseTime - HoursBeforeSunrise;
    m_beforeSunrise.minute = getMinutes(sunriseTime);
    m_afterSunset.hour = sunsetTime + HoursAfterSunset;
    m_afterSunset.minute = getMinutes(sunsetTime);
  }
  
  void notify(uint8_t command, int data = 0) const {
    Linker::instance()->notify(this, command, data);
  }

  int8_t m_timezone;
  float m_longitude;
  float m_latitude;
  int8_t m_state;

  SimpleTime m_sunrise;
  SimpleTime m_sunset;
  SimpleTime m_beforeSunrise;
  SimpleTime m_afterSunset;
};

#endif // PUMPER_SUN_ALARM

