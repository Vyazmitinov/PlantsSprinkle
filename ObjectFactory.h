#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "IObject.h"

class ObjectFactory {
public:
  static IObject * create(uint8_t type, VirtualBuffer & buffer);
};

#endif // OBJECT_FACTORY_H

