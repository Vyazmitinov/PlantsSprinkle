#ifndef PUMPER_ALARM_H
#define PUMPER_ALARM_H

#include "IObject.h"
#include "Common.h"

class Alarm: public IObject {
public:
  Alarm(VirtualBuffer & buffer);

  virtual uint8_t getType() const {return kAlarm;}
  virtual void store(VirtualBuffer & buffer);
  virtual uint8_t update(uint8_t reason, int value, uint8_t additionalData);

private:
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  bool m_activated;
};

#endif // PUMPER_ALARM_H

