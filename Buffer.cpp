#include "Buffer.h"

#include <EEPROM.h>
#include <Arduino.h>
#include <string.h>

uint8_t VirtualBuffer::read(void *data, uint8_t size) {
  uint8_t looked = lookup(data, size);
  m_pos += looked;
  return looked;
}

uint8_t VirtualBuffer::lookup(void *data, uint8_t size) {
  return size;
}

uint8_t VirtualBuffer::skip(uint8_t size) {
  m_pos += size;
  return size;
}

uint8_t VirtualBuffer::write(const void *data, uint8_t size) {
  m_pos += size;
  return size;
}

uint8_t Buffer::lookup(void *data, uint8_t size) {
  if (m_pos > m_capacity - size) {
    return 0;
  }
  memcpy(data, m_memory + m_pos, size);
  return size;
}

uint8_t Buffer::write(const void *data, uint8_t size) {
  if ((m_capacity - m_pos) < size) {
    return 0;
  }
  memcpy(m_memory + m_pos, data, size);
  m_pos += size;
  return size;
}

int Buffer::left() {
  return m_capacity - m_pos;
}

static inline void EepromUpdate(int pos, uint8_t val) {
  if (EEPROM.read(pos) != val) {
    EEPROM.write(pos, val);
  }
}

EepromBuffer::EepromBuffer() {
  Size size;
  size.size_ch[0] = EEPROM.read(0);
  size.size_ch[1] = EEPROM.read(1);
  m_capacity = size.size_int;
}

EepromBuffer::~EepromBuffer() {
  saveCounters();
}

void EepromBuffer::saveCounters() {
  Size size;
  size.size_int = m_pos;
  EepromUpdate(0, size.size_ch[0]);
  EepromUpdate(1, size.size_ch[1]);
}

uint8_t EepromBuffer::lookup(void *data, uint8_t size) {
  if (m_pos > m_capacity - size) {
    return 0;
  }
  int pos = m_pos + 2;
  uint8_t * p_src = reinterpret_cast<uint8_t *>(data);
  for (int i = 0 ; i < size; i++, pos++, p_src++) {
    *p_src = EEPROM.read(pos);
  }
  return size;
}

uint8_t EepromBuffer::write(const void *data, uint8_t size) {
  int pos = m_pos + 2;
  if (pos + size > E2END) {
    return 0;
  }
  const uint8_t * p_src = reinterpret_cast<const uint8_t *>(data);
  for (int i = 0; i < size; i++, pos++, p_src++) {
    EepromUpdate(pos, *p_src);
  }
  m_pos += size;
  if (m_pos > m_capacity) {
    m_capacity = m_pos;
  }
  return size;
}

int EepromBuffer::left() {
  return m_capacity - m_pos;
}

unsigned long EepromBuffer::_eepromCrc() {
  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;

  for (int index = 0; index < m_capacity; ++index) {
    crc = crc_table[(crc ^ EEPROM.read(index)) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM.read(index) >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }

  return crc;
}
