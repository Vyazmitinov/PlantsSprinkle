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

  virtual int left() {return 0;}
  virtual int size() {return m_pos;}

protected:
  int m_pos;
};

class Buffer: public VirtualBuffer {
public:
  Buffer(uint8_t *memory, int capacity)
    : m_memory(memory)
    , m_capacity(capacity)
  {}

  virtual uint8_t lookup(void * data, uint8_t size);
  virtual uint8_t write(const void * data, uint8_t size);
  virtual int left();

private:
  uint8_t *m_memory;
  int m_capacity;
};

union Size {
  int size_int;
  uint8_t size_ch[2];
};

class EepromBuffer: public VirtualBuffer {
public:
  EepromBuffer();
  ~EepromBuffer();

  void saveCounters();
  virtual uint8_t lookup(void * data, uint8_t size);
  virtual uint8_t write(const void * data, uint8_t size);
  virtual int left();

private:
  unsigned long _eepromCrc();
  
  int m_capacity;
};

#endif // PS_BUFFER_H

