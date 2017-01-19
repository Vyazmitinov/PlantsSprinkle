#ifndef PS_SUN_ALARM
#define PS_SUN_ALARM

#include "IObject.h"
#include "Common.h"
#include "Time.h"

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
  SunAlarm(VirtualBuffer & buffer);

  virtual uint8_t getType() const {return kSunAlarm;}
  virtual void store(VirtualBuffer & buffer);
  virtual uint8_t update(uint8_t reason, int value, uint8_t additionalData);

private:
  void _calcSunTimes(Time * time);
  uint8_t notify(uint8_t command, int data = 0) const;

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

