#ifndef PUMPER_TICKER_H
#define PUMPER_TICKER_H

#include "IObject.h"

class Ticker: public IObject {
public:
  Ticker(VirtualBuffer &) {}
  uint8_t getType() {return kTicker;}
  void tick() {
    Linker::instance()->notify(this, kTick);
  }
  virtual void update(uint8_t commad, int data, uint8_t additionalData) {}
private:
};

#endif // PUMPER_TICKER_H

