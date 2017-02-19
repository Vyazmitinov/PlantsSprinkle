#include "Alarm.h"

#include "Time.h"

#ifdef PS_ORDINAL_ALARM

uint8_t Alarm::read(VirtualBuffer &buffer) {
  Object::read(buffer);
  buffer.read(&m_hour, sizeof(m_hour));
  buffer.read(&m_minute, sizeof(m_minute));
  buffer.read(&m_second, sizeof(m_second));
  return 4;
}

uint8_t Alarm::write(VirtualBuffer &buffer) {
  Object::write(buffer);
  buffer.write(&m_hour, sizeof(m_hour));
  buffer.write(&m_minute, sizeof(m_minute));
  buffer.write(&m_second, sizeof(m_second));
  return 4;
}

uint8_t Alarm::update(const Linker* linker, uint8_t, const void * data) {
  const Time * time = reinterpret_cast<const Time *>(data);
  if ((time->hour() == 0) && (time->minute() == 0) && (time->second() == 0)) {
    m_activated = false;
  }
  if (m_activated) {
    return 0;
  }
  if (time->hour() < m_hour) {
    return 0;
  }
  if ((time->hour() == m_hour) && (time->minute() < m_minute)) {
    return 0;
  }
  if ((time->hour() == m_hour) && (time->minute() == m_minute) && (time->second() < m_second)) {
    return 0;
  }
  m_activated = true;
  return notify(linker, kSigAlarmOccured, nullptr);
}

#endif // PS_ORDINAL_ALARM
