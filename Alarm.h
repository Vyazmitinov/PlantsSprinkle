#ifndef PUMPER_ALARM_H
#define PUMPER_ALARM_H

#include "Observer.h"
#include "Common.h"

class Alarm: public ILinkableObserver, public ILinkableSubject, public ISerializable {
public:
  Alarm(Buffer & buffer)
    : m_activated(false)
  {
    buffer.read(m_hour);
    buffer.read(m_minute);
    buffer.read(m_second);
  }

  virtual void store(Buffer & buffer) {
    buffer.write(m_hour);
    buffer.write(m_minute);
    buffer.write(m_second);
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason != TimeUpdated) {
      return;
    }
    Time * time = (Time *)value;
    if ((time->hour() == 0) && (time->minute() == 0) && (time->second() == 0)) {
      m_activated = false;
    }
    if (m_activated) {
      return;
    }
    if (time->hour() < m_hour) {
      return;
    }
    if ((time->hour() == m_hour) && (time->minute() < m_minute)) {
      return;
    }
    if ((time->hour() == m_hour) && (time->minute() == m_minute) && (time->second() < m_second)) {
      return;
    }
    m_activated = true;
    notify(AlarmOccured);
  }
private:
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  bool m_activated;
};

#endif // PUMPER_ALARM_H

