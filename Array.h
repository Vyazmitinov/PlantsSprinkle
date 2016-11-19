#ifndef PUMPER_ARRAY_H
#define PUMPER_ARRAY_H

template <typename T, uint8_t N>
class array {
public:
  array()
    : _count(0)
  {}

  void push_back(const T &obj) {
    if (_count == N) {
      return;
    }
    _data[_count] = obj;
    ++_count;
  }

  const T & operator[] (uint8_t i) const {
    if (i < N) {
      return _data[i];
    }
    return _data[N - 1];
  }

  inline uint8_t size() const {
    return _count;
  }

private:
  uint8_t _count;
  T _data[N];
} __attribute__((packed));

#endif PUMPER_ARRAY_H

