#ifndef PS_TICKER_H
#define PS_TICKER_H

#include "Object.h"
#include "Common.h"
#include "Linker.h"

class Ticker: public Object {
public:
  virtual uint8_t getType() const {return kTicker;}
  uint8_t tick(const Linker* linker) {
    return linker->notify(getId(), kSigTick);
  }
private:
};

#endif // PS_TICKER_H

