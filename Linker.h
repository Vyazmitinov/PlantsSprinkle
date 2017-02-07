#ifndef PS_LINKER_H
#define PS_LINKER_H

#include "ObjectStore.h"
#include "Array.h"
#include "Buffer.h"

class Linker {
  struct Link {
    Link(uint8_t _senderId = 0, uint8_t _receiverId = 0, uint8_t _signal = 0, uint8_t _command = 0)
        : senderId(_senderId), receiverId(_receiverId), signal(_signal), command(_command)
    {}
    uint8_t senderId;
    uint8_t receiverId;
    uint8_t signal;
    uint8_t command;
  } __attribute__((packed));

public:
  Linker(ObjectStore * _store) : m_store(_store) {}

  const ObjectStore * getStore() const {return m_store;}

  void read(VirtualBuffer & buffer) {
    while (buffer.left()) {
      uint8_t senderId;
      uint8_t receiverId;
      uint8_t signal;
      uint8_t command;
      buffer.read(&senderId, sizeof(senderId));
      buffer.read(&receiverId, sizeof(receiverId));
      buffer.read(&signal, sizeof(signal));
      buffer.read(&command, sizeof(command));
      m_links.push_back(Link(senderId, receiverId, signal, command));
    }
  }

  void write(VirtualBuffer & buffer) const {
    for (uint8_t i = 0; i < m_links.size(); ++i) {
      buffer.write(&m_links[i].senderId, sizeof(m_links[i].senderId));
      buffer.write(&m_links[i].receiverId, sizeof(m_links[i].receiverId));
      buffer.write(&m_links[i].signal, sizeof(m_links[i].signal));
      buffer.write(&m_links[i].command, sizeof(m_links[i].command));
    }
  }

  uint8_t notify(uint8_t senderId, uint8_t signal, const void * data = nullptr) const {
    uint8_t rv = 0;
    for (uint8_t i = 0; i < m_links.size(); ++i) {
      const Link & link = m_links[i];
      if ((senderId == link.senderId) && (signal == link.signal)) {
        IReceiver * observer = m_store->getObject(link.receiverId);
        if (observer != nullptr) {
          rv |= observer->update(this, link.command, data);
        }
      }
    }
    return rv;
  }

private:
  Linker() :m_store(nullptr) {}

  array<Link, 32> m_links;
  ObjectStore * m_store;
};

#endif // PS_LINKER_H
