#include "ObjectStore.h"

#include "HumiditySensor.h"
#include "Display.h"
#include "Alarm.h"
#include "Relay.h"
#include "Button.h"
#include "Ticker.h"
#include "Buffer.h"
#include "SunAlarm.h"
#include "Serializer.h"
#include "Linker.h"

Object * ObjectFactory::create(uint8_t type) {
  switch(type) {
    case kTicker: return new Ticker();
#ifdef PS_HUMIDITY_SENSOR
    case kHumiditySensor: return new HumiditySensor();
#endif //PS_HUMIDITY_SENSOR
#ifdef PS_DISPLAY
    case kDisplay: return new Display();
#endif // PS_DISPLAY
#ifdef PS_RTC_TIME
    case kTime: return new Time();
#endif // PS_RTC_TIME
#ifdef PS_ORDINAL_ALARM
    case kAlarm: return new Alarm();
#endif // PS_ORDINAL_ALARM
#ifdef PS_BUTTON
    case kButton: return new Button();
#endif // PS_BUTTON
#ifdef PS_RELAY
    case kRelay: return new Relay();
#endif // PS_RELAY
#ifdef PS_SUN_ALARM
    case kSunAlarm: return new SunAlarm();
#endif // PS_SUN_ALARM
#ifdef PS_SERIALIZER
    case kSerializer: return new Serializer();
#endif // PS_SERIALIZER
  }
  return nullptr;
}

void ObjectStore::read(VirtualBuffer & buffer) {
  uint8_t type;
  while (buffer.left()) {
    buffer.read(&type, sizeof(type));
    Object * obj = ObjectFactory::create(type);
    if (obj == nullptr) {
      break;
    }
    obj->read(buffer);
    m_objects.push_back(obj);
  }
}

void ObjectStore::write(VirtualBuffer & buffer) const {
  uint8_t type;
  for (uint8_t i = 0; i < m_objects.size(); ++i) {
    type = m_objects[i]->getType();
    buffer.write(&type, sizeof(type));
    m_objects[i]->write(buffer);
  }
}
