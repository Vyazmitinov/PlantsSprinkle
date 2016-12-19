#ifndef PS_TICKER_H
#define PS_TICKER_H

#include "IObject.h"
#include "Common.h"
#include "Linker.h"

class Ticker: public IObject {
public:
  Ticker(VirtualBuffer &) {}
  virtual uint8_t getType() const {return kTicker;}
  virtual void tick() {
    Linker::instance()->notify(this, kTick);
  }
  virtual void update(uint8_t commad, int data, uint8_t additionalData) {}
private:
};

#endif // PS_TICKER_H

