#include "Alarm.h"

#include "Time.h"

Alarm::Alarm(VirtualBuffer &buffer)
  : m_activated(false)
{
  buffer.read(&m_hour, sizeof(m_hour));
  buffer.read(&m_minute, sizeof(m_minute));
  buffer.read(&m_second, sizeof(m_second));
}

void Alarm::store(VirtualBuffer &buffer) {
  buffer.write(&m_hour, sizeof(m_hour));
  buffer.write(&m_minute, sizeof(m_minute));
  buffer.write(&m_second, sizeof(m_second));
}

void Alarm::update(uint8_t reason, int value, uint8_t additionalData) {
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
