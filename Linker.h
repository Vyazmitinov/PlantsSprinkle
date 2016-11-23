#ifndef PUMPER_OBSERVER_H
#define PUMPER_OBSERVER_H

#include "Array.h"
#include "Buffer.h"

class ILinkable: public ISerializable {
public:
  virtual void update(uint8_t commad, int data, uint8_t additionalData) = 0;
  void notify(uint8_t command, int data = 0) const;
};

class Linker {
  struct Link {
    uint8_t senderId;
    uint8_t receiverId;
    uint8_t command;
    uint8_t additionalData;
  } __attribute__((packed));

public:
  static Linker * instance() {
    static Linker linker;
    return & linker;
  }

  void store(Buffer & buffer) {
    for (uint8_t i = 0; i < m_objects.size(); ++i) {
      m_objects[i]->store(buffer);
    }
    for (uint8_t i = 0; i < m_links.size(); ++i) {
      buffer.write(m_links[i].senderId);
      buffer.write(m_links[i].receiverId);
      buffer.write(m_links[i].command);
      buffer.write(m_links[i].additionalData);
    }
  }

  void addObject(ISerializable * object) {
    m_objects.push_back(object);
  }

  void addLink(uint8_t senderId, uint8_t receiverId, uint8_t command, uint8_t additionalData = 0) {
    m_links.push_back(Link{senderId, receiverId, command, additionalData});
  }

  void notify(const ILinkable * sender, uint8_t command, int data) const {
    uint8_t senderId = 0;
    if (!_findSenderId(sender, senderId)) {
      return;
    }

    for (uint8_t i = 0; i < m_links.size(); ++i) {
      const Link & link = m_links[i];
      if ((link.senderId == senderId) && (link.command == command)) {
        ILinkable * observer = m_objects[link.receiverId];
        observer->update(command, data, link.additionalData);
      }
    }
  }

private:
  Linker() {}
  bool _findSenderId(ILinkable * sender, uint8_t & senderId) {
    for (uint8_t i = 0; i < m_objects.size(); ++i) {
      if (sender == m_objects[i]) {
        senderId = i;
        return true;
      }
    }
    return false;
  }

  array<Link, 64> m_links;
  array<ILinkable *, 32> m_objects;
};

void ILinkable::notify(uint8_t command, int data) const {
  Linker::instance()->notify(this, command, data);
}

#endif PUMPER_OBSERVER_H
