#ifndef LINKER_H
#define LINKER_H

#include "ObjectFactory.h"
#include "Array.h"
#include "Buffer.h"

class Linker {
  struct Link {
    uint8_t senderId;
    uint8_t receiverId;
    uint8_t additionalData;
  } __attribute__((packed));

public:
  static Linker * instance() {
    static Linker linker;
    return & linker;
  }

  void store(VirtualBuffer & buffer) const {
    uint8_t type;
    for (uint8_t i = 0; i < m_objects.size(); ++i) {
      type = m_objects[i]->getType();
      buffer.write(&type, sizeof(type));
      m_objects[i]->store(buffer);
    }
    type = kLink;
    for (uint8_t i = 0; i < m_links.size(); ++i) {
      buffer.write(&type, sizeof(type));
      buffer.write(&m_links[i].senderId, sizeof(m_links[i].senderId));
      buffer.write(&m_links[i].receiverId, sizeof(m_links[i].receiverId));
      buffer.write(&m_links[i].additionalData, sizeof(m_links[i].additionalData));
    }
  }

  void load(VirtualBuffer & buffer) {
    while (buffer.left()) {
      uint8_t type;
      buffer.read(&type, sizeof(type));
      if (type == kLink) {
        uint8_t senderId;
        uint8_t receiverId;
        uint8_t additionalData;
        buffer.read(&senderId, sizeof(senderId));
        buffer.read(&receiverId, sizeof(receiverId));
        buffer.read(&additionalData, sizeof(additionalData));
        addLink(senderId, receiverId, additionalData);
      } else {
        IObject * obj = ObjectFactory::create(type, buffer);
        if (obj != 0) {
          addObject(obj);
        }
      }
    }
  }

  void addObject(IObject * object) {
    m_objects.push_back(object);
  }

  IObject * getObject(unsigned pos) {
    return m_objects[pos];
  }

  void addLink(uint8_t senderId, uint8_t receiverId, uint8_t additionalData = 0) {
    m_links.push_back(Link{senderId, receiverId, additionalData});
  }

  void notify(const IObject * sender, uint8_t command, int data = 0) const {
    uint8_t senderId = 0;
    if (!_findSenderId(sender, senderId)) {
      return;
    }

    for (uint8_t i = 0; i < m_links.size(); ++i) {
      const Link & link = m_links[i];
      if (link.senderId == senderId) {
        IObject * observer = m_objects[link.receiverId];
        observer->update(command, data, link.additionalData);
      }
    }
  }

private:
  Linker() {}
  bool _findSenderId(IObject * sender, uint8_t & senderId) {
    for (uint8_t i = 0; i < m_objects.size(); ++i) {
      if (sender == m_objects[i]) {
        senderId = i;
        return true;
      }
    }
    return false;
  }

  array<Link, 42> m_links;
  array<IObject *, 16> m_objects;
};

#endif LINKER_H
