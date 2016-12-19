#ifndef PS_PUMP_H
#define PS_PUMP_H

#include "IObject.h"
#include "Common.h"
#include <Arduino.h>

const long PumpTicks = 40000 / LoopDelay; // 40s
//const long WaitTicks = 43200000 / LoopDelay; // 12h
const long WaitTicks = 3600000 / LoopDelay; // 1h
const long MaxWorkingTime = 10L * 60L * 1000L / LoopDelay; // 10m

class Pump: public IObject {
public:
  const uint8_t ON = HIGH;
  const uint8_t OFF = LOW;

  Pump(VirtualBuffer & buffer);

  virtual uint8_t getType() const {return kPump;}
  virtual void store(VirtualBuffer & buffer);
  virtual void update(uint8_t reason, int value, uint8_t additionalData);

private:
  void _setup();
  void _startWork();
  void _work();
  void _stopWork();

  uint8_t m_powerPin;
  long m_waitTicks;
  uint8_t m_state;
};

#endif // PS_PUMP_H

