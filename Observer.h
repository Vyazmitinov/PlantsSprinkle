#ifndef PUMPER_OBSERVER_H
#define PUMPER_OBSERVER_H

#include "Array.h"

class ILinkableObserver {
public:
  virtual void update(uint8_t commad, int data, uint8_t additionalData) = 0;
};

class ILinkableSubject {
public:
  void notify(uint8_t command, int data = 0) const;
};

class Linker {
  struct Link {
    ILinkableSubject * sender;
    ILinkableObserver * receiver;
    uint8_t command;
    uint8_t additionalData;
  } __attribute__((packed));

public:
  static Linker * instance() {
    static Linker linker;
    return & linker;
  }

  void addLink(ILinkableSubject * sender, ILinkableObserver * receiver, uint8_t command, uint8_t additionalData = 0) {
    _links.push_back(Link{sender, receiver, command, additionalData});
  }

  void notify(const ILinkableSubject * sender, uint8_t command, int data) const {
    for (uint8_t i = 0; i < _links.size(); ++i) {
      const Link & link = _links[i];
      if ((link.sender == sender) && (link.command == command)) {
        link.receiver->update(command, data, link.additionalData);
      }
    }
  }

private:
  Linker() {}

  array<Link, 64> _links;
};

void ILinkableSubject::notify(uint8_t command, int data) const {
  Linker::instance()->notify(this, command, data);
}

#endif PUMPER_OBSERVER_H
