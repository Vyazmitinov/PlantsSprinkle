#ifndef PUMPER_ALARM_H
#define PUMPER_ALARM_H

#include "Object.h"
#include "Common.h"

#ifdef PS_ORDINAL_ALARM

class Alarm: public Object {
public:
  Alarm() : m_activated(false) {}

  virtual uint8_t getType() const {return kAlarm;}

  virtual uint8_t read(VirtualBuffer &buffer);
  virtual uint8_t write(VirtualBuffer & buffer);

  virtual uint8_t update(const Linker *linker, uint8_t, const void * data);

private:
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  bool m_activated;
};

#endif // PS_ORDINAL_ALARM

#endif // PUMPER_ALARM_H
