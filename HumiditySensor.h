#ifndef PS_HUMIDITY_SENSOR_H
#define PS_HUMIDITY_SENSOR_H

#include "Common.h"
#include "IObject.h"

const long HSCheckPumpingDelay = 10000 / LoopDelay; // 10s
const long HSCheckDelay = 30000 / LoopDelay; // 30s

const int MeasuremensCount = 3;

class HumiditySensor: public IObject {
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
  HumiditySensor(VirtualBuffer & buffer);

  virtual uint8_t getType() const {return kHumiditySensor;}
  virtual void store(VirtualBuffer & buffer);
  virtual void update(uint8_t reason, int value, uint8_t additionalData);

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

  void _processData(int data);
  void _tick();
  void _levelDown();
  void _levelUp();
  void notify(uint8_t command, int data = 0) const;

  uint8_t m_powerPin;
  uint8_t m_analogPin;
  uint8_t m_level;
  Measuremens m_measurements;
  long m_checkDelay;
  bool m_firstRun;
};

#endif // PS_HUMIDITY_SENSOR_H

