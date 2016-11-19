#ifndef PUMPER_ALARM_H
#define PUMPER_ALARM_H

#include "Observer.h"
#include "Common.h"

class Alarm: public ILinkableObserver, public ILinkableSubject {
  public:
    Alarm(Time * _time, uint8_t _hour, uint8_t _minute, uint8_t _second, int _command)
      : m_time(_time)
      , m_hour(_hour)
      , m_minute(_minute)
      , m_second(_second)
      , m_command(_command)
      , m_activated(false)
    {}
    virtual void update(uint8_t reason, int value, uint8_t additionalData) {
      if (reason != TimeUpdated) {
        return;
      }
      if ((m_time->hour() == 0) && (m_time->minute() == 0) && (m_time->second() == 0)) {
        m_activated = false;
      }
      if (m_activated) {
        return;
      }
      if (m_time->hour() < m_hour) {
        return;
      }
      if ((m_time->hour() == m_hour) && (m_time->minute() < m_minute)) {
        return;
      }
      if ((m_time->hour() == m_hour) && (m_time->minute() == m_minute) && (m_time->second() < m_second)) {
        return;
      }
      m_activated = true;
      notify(m_command);
    }
  private:
    Time * m_time;

    uint8_t m_hour;
    uint8_t m_minute;
    uint8_t m_second;
    int m_command;
    bool m_activated;
};

#endif // PUMPER_ALARM_H

