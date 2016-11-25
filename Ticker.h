#ifndef PUMPER_TICKER_H
#define PUMPER_TICKER_H

#include "Linker.h"

class Ticker: public ILinkable {
public:
  virtual void store(Buffer & buffer) {}
  void tick() {
    notify(kTick);
  }
  virtual void update(uint8_t commad, int data, uint8_t additionalData) {}
};

#endif // PUMPER_TICKER_H

