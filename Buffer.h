#ifndef PUMPER_BUFFER_H
#define PUMPER_BUFFER_H

#include <string.h>

class VirtualBuffer {
public:
  VirtualBuffer()
    : m_pos(0)
  {}

  virtual uint8_t read(void * data, uint8_t size) {
    uint8_t looked = lookup(data, size);
    m_pos += looked;
    return looked;
  }

  virtual uint8_t lookup(void * data, uint8_t size) {
    return size;
  }

  virtual uint8_t skip(uint8_t size) {
    m_pos += size;
    return size;
  }

  virtual uint8_t write(void * data, uint8_t size) {
    m_pos += size;
    return size;
  }

  virtual int left() {return 0;}
  virtual int size() {return m_pos;}

protected:
  int m_pos;
};

class Buffer: public VirtualBuffer {
public:
  Buffer(const uint8_t *memory, int capacity)
    : m_memory(memory)
    , m_capacity(capacity)
  {}

  virtual uint8_t lookup(void * data, uint8_t size) {
    if (m_pos > m_capacity - size) {
      return 0;
    }
    memcpy(data, m_memory + m_pos, size);
    return size;
  }

  virtual uint8_t write(void * data, uint8_t size) {
    if ((m_capacity - m_pos) < size) {
      return 0;
    }
    memcpy(m_memory + m_pos, data, size);
    m_pos += size;
    return size;
  }

  virtual int left() {
    return m_capacity - m_pos;
  }

  virtual int size() {
    return m_pos;
  }
private:
  const uint8_t *m_memory;
  int m_capacity;
};

#endif PUMPER_BUFFER_H

