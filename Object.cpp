#include "Object.h"

#include "Linker.h"

uint8_t Object::notify(const Linker* linker, uint8_t signal, const void *data) {
  return linker->notify(getId(), signal, data);
}
