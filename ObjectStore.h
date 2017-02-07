#ifndef OBJECT_STORE_H
#define OBJECT_STORE_H

#include "Object.h"
#include "Array.h"

class ObjectFactory {
public:
  static Object * create(uint8_t type);
};


class ObjectStore {
public:
  void read(VirtualBuffer & buffer);
  void write(VirtualBuffer & buffer) const;

  Object * getObject(unsigned pos) {
    if (pos < m_objects.size()) {
      return m_objects[pos];
    }
    return nullptr;
  }

private:
  array<Object *, 16> m_objects;
};

#endif // OBJECT_STORE_H

