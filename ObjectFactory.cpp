#include "ObjectFactory.h"

#include "HumiditySensor.h"
#include "Display.h"
#include "Alarm.h"
#include "Light.h"
#include "Pump.h"
#include "Button.h"
#include "Ticker.h"
#include "Buffer.h"
#include "SunAlarm.h"
#include "Serializer.h"

IObject * ObjectFactory::create(uint8_t type, VirtualBuffer & buffer) {
  switch(type) {
    case kTicker: return new Ticker(buffer);
    case kHumiditySensor: return new HumiditySensor(buffer);
    case kDisplay: return new Display(buffer);
    case kPump: return new Pump(buffer);
    case kTime: return new Time(buffer);
    case kAlarm: return new Alarm(buffer);
    case kButton: return new Button(buffer);
    case kLight: return new Light(buffer);
    case kSunAlarm: return new SunAlarm(buffer);
    case kSerializer: return new Serializer(buffer);
  }
  return 0;
}

