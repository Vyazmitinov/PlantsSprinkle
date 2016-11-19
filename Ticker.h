#ifndef PUMPER_TICKER_H
#define PUMPER_TICKER_H

#include "Observer.h"

class Ticker: public ILinkableSubject {
public:
  void tick() {
    notify(Tick);
  }
};

#endif // PUMPER_TICKER_H

