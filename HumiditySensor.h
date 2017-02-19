#ifndef PS_HUMIDITY_SENSOR_H
#define PS_HUMIDITY_SENSOR_H

#include "Common.h"
#include "Object.h"

#ifdef PS_HUMIDITY_SENSOR

const long HSCheckPumpingDelay = 2000 / RegularLoopDelay; // 2s
const long HSCheckDelay = 30000 / RegularLoopDelay; // 30s

const int MeasuremensCount = 3;

class HumiditySensor: public Object {
  class Measuremens {
  public:
    Measuremens();
    void add(uint8_t measurement);
    uint8_t average() const;
  private:
    uint8_t m_measurements[MeasuremensCount];
    uint8_t m_curr;
    bool m_overfill;
  };

public:
  HumiditySensor()
    : m_checkDelay(0)
    , m_firstRun(true)
  {
    _setup();
  }

  virtual uint8_t getType() const {return kHumiditySensor;}

  virtual uint8_t read(VirtualBuffer &buffer);
  virtual uint8_t write(VirtualBuffer & buffer);

  virtual uint8_t update(const Linker* linker, uint8_t command, const void *);

private:
  static const int LevelDevider = 100;
  static const int MaxLevel = 9;
  static const int NoConnectionLevel = 950;

  void _setup();

  inline bool _isDry(uint8_t avg) const {
    return avg >= m_level;
  }
  inline bool _isWet(uint8_t avg) const {
    return avg < m_level - 1;
  }

  uint8_t _processData(const Linker *linker, int data);
  uint8_t _tick(const Linker *linker);
  void _levelDown(const Linker *linker);
  void _levelUp(const Linker* linker);

  uint8_t m_powerPin;
  uint8_t m_analogPin;
  uint8_t m_level;
  Measuremens m_measurements;
  long m_checkDelay;
  bool m_firstRun;
};

#endif // PS_HUMIDITY_SENSOR

#endif // PS_HUMIDITY_SENSOR_H

