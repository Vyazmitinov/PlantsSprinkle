#ifndef PS_OBJECT_H
#define PS_OBJECT_H

#include <stdint.h>

#include "Buffer.h"

class Linker;

class IReceiver {
public:
  virtual uint8_t update(const Linker* linker, uint8_t command, const void * data) = 0;
};

class ISender {
  virtual uint8_t getId() const = 0;
  virtual uint8_t notify(const Linker* linker, uint8_t signal, const void * data = nullptr) = 0;
};

class Object: public IReceiver, public ISender {
public:
  virtual uint8_t getType() const = 0;
  inline virtual uint8_t getId() const {return m_id;}

  inline virtual uint8_t write(VirtualBuffer &buffer) {
    return buffer.write(&m_id, sizeof(m_id));
  }
  inline virtual uint8_t read(VirtualBuffer &buffer) {
    return buffer.read(&m_id, sizeof(m_id));
  }

  virtual uint8_t update(const Linker*, uint8_t, const void *) {
    return 0;
  }
  virtual uint8_t notify(const Linker* linker, uint8_t signal, const void * data = nullptr) final;

private:
  uint8_t m_id;
};

#endif // PS_OBJECT_H
