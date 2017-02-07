#include "SunAlarm.h"

#ifdef PS_SUN_ALARM

#include "SunTime.h"

uint8_t SunAlarm::read(VirtualBuffer &buffer) {
  Object::read(buffer);
  buffer.read(&m_timezone, sizeof(m_timezone));
  buffer.read(&m_longitude, sizeof(m_longitude));
  buffer.read(&m_latitude, sizeof(m_latitude));
  buffer.read(&m_state, sizeof(m_state));
  return 5;
}

uint8_t SunAlarm::write(VirtualBuffer &buffer) {
  Object::write(buffer);
  buffer.write(&m_timezone, sizeof(m_timezone));
  buffer.write(&m_longitude, sizeof(m_longitude));
  buffer.write(&m_latitude, sizeof(m_latitude));
  buffer.write(&m_state, sizeof(m_state));
  return 5;
}

uint8_t SunAlarm::update(const Linker* linker, uint8_t , const void * data) {
  Time * time = (Time *)data;
  if (((time->hour() == 0) && (time->minute() == 0) && (time->second() == 0))
      || ((m_sunrise.hour == 0) && (m_sunrise.minute == 0)))
  {
    _calcSunTimes(time);
  }

  SimpleTime stime(time->hour(), time->minute());

  uint8_t newState = kSigStopLighting;
  if (stime < m_beforeSunrise) {
    newState = kSigStopLighting;
  } else if (stime < m_sunrise) {
    newState = kSigStartLighting;
  } else if (stime < m_sunset) {
    newState = kSigStopLighting;
  } else if (stime < m_afterSunset) {
    newState = kSigStartLighting;
  }

  if (newState != m_state) {
    m_state = newState;
    return notify(linker, m_state, nullptr);
  }
  return 0;
}

void SunAlarm::_calcSunTimes(Time *time) {
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

#endif // PS_SUN_ALARM
