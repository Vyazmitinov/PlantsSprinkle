#ifndef PUMPER_TICKER_H
#define PUMPER_TICKER_H

#include "Observer.h"

class Ticker: public ILinkableSubject, public ISerializable {
public:
  virtual void store(Buffer & buffer) {}
  void tick() {
    notify(Tick);
  }
};

#endif // PUMPER_TICKER_H

