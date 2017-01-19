#ifndef IOBJECT_H
#define IOBJECT_H

#include <stdint.h>

#include "Buffer.h"

class IObject {
public:
  virtual uint8_t update(uint8_t commad, int data, uint8_t additionalData) = 0;
  virtual uint8_t getType() const = 0;
  virtual void store(VirtualBuffer & buffer) {}
};

#endif // IOBJECT_H

