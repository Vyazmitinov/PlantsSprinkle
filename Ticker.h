#ifndef PS_TICKER_H
#define PS_TICKER_H

#include "IObject.h"
#include "Common.h"
#include "Linker.h"

class Ticker: public IObject {
public:
  Ticker(VirtualBuffer &) {}
  virtual uint8_t getType() const {return kTicker;}
  virtual uint8_t tick() {
    return Linker::instance()->notify(this, kTick);
  }
  virtual uint8_t update(uint8_t commad, int data, uint8_t additionalData) {return 0;}
private:
};

#endif // PS_TICKER_H

