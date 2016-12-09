#ifndef PUMPER_ALARM_H
#define PUMPER_ALARM_H

#include "IObject.h"
#include "Common.h"

class Alarm: public IObject {
public:
  Alarm(VirtualBuffer & buffer)
    : m_activated(false)
  {
    buffer.read(&m_hour, sizeof(m_hour));
    buffer.read(&m_minute, sizeof(m_minute));
    buffer.read(&m_second, sizeof(m_second));
  }

  uint8_t getType() {return kAlarm;}

  virtual void store(VirtualBuffer & buffer) {
    buffer.write(&m_hour, sizeof(m_hour));
    buffer.write(&m_minute, sizeof(m_minute));
    buffer.write(&m_second, sizeof(m_second));
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason != kTimeUpdated) {
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
    Linker::instance()->notify(this, kAlarmOccured);
  }
private:
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  bool m_activated;
};

#endif // PUMPER_ALARM_H

