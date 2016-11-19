#ifndef PUMPER_HUMIDITY_SENSOR_H
#define PUMPER_HUMIDITY_SENSOR_H

#include "Common.h"
#include "Observer.h"
#include "Buffer.h"

const long HSCheckDelay = 30000 / LoopDelay; // 30s

const int MeasuremensCount = 3;

class HumiditySensor: public ILinkableObserver, public ILinkableSubject {
  class Measuremens {
  public:
    Measuremens()
      : m_curr(0)
      , m_overfill(false)
    {
      for (uint8_t i = 0; i < MeasuremensCount; ++i) {
        m_measurements[i] = 0;
      }
    }
    void add(uint8_t measurement) {
      m_measurements[m_curr] = measurement;
      ++m_curr;
      if (m_curr == MeasuremensCount) {
        m_curr = 0;
        m_overfill = true;
      }
    }
    uint8_t average() const {
      uint8_t sum = 0;
      uint8_t count = m_overfill ? MeasuremensCount : m_curr;
      if (count == 0) {
        return 0;
      }
      for (uint8_t i = 0; i < count; ++i) {
        sum += m_measurements[i];
      }
      return sum / count;
    }
  private:
    uint8_t m_measurements[MeasuremensCount];
    uint8_t m_curr;
    bool m_overfill;
  };
public:
  HumiditySensor(Buffer & buffer)
    : m_checkDelay(0)
  {
    buffer.read(m_powerPin);
    buffer.read(m_analogPin);
    buffer.read(m_level);
    _setup();
  }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    if (reason == Tick) {
      tick();
    } else if (reason == ButtonPushed) {
      if (additionalData == ButtonUp) {
        levelUp();
      } else if (additionalData == ButtonDown) {
        levelDown();
      }
    }
  }
private:
  void _setup() {
    pinMode(m_powerPin, OUTPUT);
    pinMode(m_analogPin, INPUT);
    digitalWrite(m_powerPin, LOW);
  }

  const int LevelDevider = 100;
  const int MaxLevel = 9;
  const int NoConnectionLevel = 950;
  void processData(int data) {
    if (data > NoConnectionLevel) {
      data = 0;
    }
    m_measurements.add(data/LevelDevider);
    int avg = m_measurements.average();
    if (avg < m_level - 1) {
      notify(HSWet, avg);
    } else if (avg >= m_level) {
      notify(HSDry, avg);
    }
    notify(HSValue, avg);
  }
  void tick() {
    if (m_checkDelay == 0) {
      digitalWrite(m_powerPin, HIGH);
    } else if (m_checkDelay == 1) {
      processData(analogRead(m_analogPin));
      digitalWrite(m_powerPin, LOW);
    }
    if (++m_checkDelay == HSCheckDelay) {
      m_checkDelay = 0;
    }
    return;
  }
  void levelDown() {
    if (m_level > 0) {
      --m_level;
    }
    notify(HSLevelChanged, m_level);
  }
  void levelUp() {
    if (m_level < MaxLevel) {
      ++m_level;
    }
    notify(HSLevelChanged, m_level);
  }

  uint8_t m_powerPin;
  uint8_t m_analogPin;
  uint8_t m_level;
  Measuremens m_measurements;
  long m_checkDelay;
};

#endif PUMPER_HUMIDITY_SENSOR_H

