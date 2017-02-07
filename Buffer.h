#ifndef PS_BUFFER_H
#define PS_BUFFER_H

#include <stdint.h>

class VirtualBuffer {
public:
  VirtualBuffer()
    : m_pos(0)
  {}

  virtual uint8_t read(void * data, uint8_t size);
  virtual uint8_t lookup(void * data, uint8_t size);
  virtual uint8_t skip(uint8_t size);
  virtual uint8_t write(const void * data, uint8_t size);

  virtual uint16_t left() {return 0;}
  virtual uint16_t size() {return m_pos;}

protected:
  uint16_t m_pos;
};

class Buffer: public VirtualBuffer {
public:
  Buffer(uint8_t *memory, uint16_t capacity)
    : m_memory(memory)
    , m_capacity(capacity)
  {}

  virtual uint8_t lookup(void * data, uint8_t size);
  virtual uint8_t write(const void * data, uint8_t size);
  virtual uint16_t left();

private:
  uint8_t *m_memory;
  uint16_t m_capacity;
};

union Size {
  uint16_t size_uint16;
  uint8_t size_ch[2];
};

class EepromBuffer: public VirtualBuffer {
public:
  EepromBuffer(uint16_t shift);
  ~EepromBuffer();

  void saveCounters();
  virtual uint8_t lookup(void * data, uint8_t size);
  virtual uint8_t write(const void * data, uint8_t size);
  virtual uint16_t left();

private:
  unsigned long _eepromCrc();
  
  uint16_t m_capacity;
  uint16_t m_shift;
};

#endif // PS_BUFFER_H
