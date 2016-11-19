#ifndef PUMPER_BUFFER_H
#define PUMPER_BUFFER_H

class Buffer {
public:
  Buffer(const uint8_t *memory, int size)
    : m_memory(memory)
    , m_size(size)
    , m_pos(0)
  {}

  template<typename T>
  uint8_t read(T & data) {
    uint8_t looked = lookup(data);
    m_pos += looked;
    return looked;
  }

  template<typename T>
  uint8_t lookup(T & data) {
    if (m_pos > m_size - sizeof(T)) {
      return 0;
    }
    data = *(T*)(m_memory + m_pos);
    return sizeof(T);
  }

  int left() {
    return m_size - m_pos;
  }
private:
  const uint8_t *m_memory;
  int m_size;
  int m_pos;
};

#endif PUMPER_BUFFER_H

