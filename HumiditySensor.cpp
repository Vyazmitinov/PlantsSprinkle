#include "HumiditySensor.h"

#include "Arduino.h"
#include "Linker.h"

HumiditySensor::Measuremens::Measuremens()
  : m_curr(0)
  , m_overfill(false)
{
  for (uint8_t i = 0; i < MeasuremensCount; ++i) {
    m_measurements[i] = 0;
  }
}

void HumiditySensor::Measuremens::add(uint8_t measurement) {
  m_measurements[m_curr] = measurement;
  ++m_curr;
  if (m_curr == MeasuremensCount) {
    m_curr = 0;
    m_overfill = true;
  }
}

uint8_t HumiditySensor::Measuremens::average() const {
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

HumiditySensor::HumiditySensor(VirtualBuffer &buffer)
  : m_checkDelay(0)
  , m_firstRun(true)
{
  buffer.read(&m_powerPin, sizeof(m_powerPin));
  buffer.read(&m_analogPin, sizeof(m_analogPin));
  buffer.read(&m_level, sizeof(m_level));
  buffer.skip(sizeof(uint8_t));
  _setup();
}

void HumiditySensor::store(VirtualBuffer &buffer) {
  buffer.write(&m_powerPin, sizeof(m_powerPin));
  buffer.write(&m_analogPin, sizeof(m_analogPin));
  buffer.write(&m_level, sizeof(m_level));
  uint8_t avg = m_measurements.average();
  buffer.write(&avg, sizeof(avg));
}

void HumiditySensor::update(uint8_t reason, int value, uint8_t additionalData) {
  if (reason == kTick) {
    _tick();
  } else if (reason == kButtonPushed) {
    if (additionalData == kButtonUp) {
      _levelUp();
    } else if (additionalData == kButtonDown) {
      _levelDown();
    }
    notify(kNeedToSerialize);
  }
}

void HumiditySensor::_setup() {
  pinMode(m_powerPin, OUTPUT);
  pinMode(m_analogPin, INPUT);
  digitalWrite(m_powerPin, LOW);
}

void HumiditySensor::_processData(int data) {
  if (data > NoConnectionLevel) {
    data = 0;
  }
  m_measurements.add(data/LevelDevider);
  int avg = m_measurements.average();
  if (_isWet(avg)) {
    notify(kHSWet, avg);
  } else if (_isDry(avg)) {
    notify(kHSDry, avg);
  }
  notify(kHSValue, avg);
}

void HumiditySensor::_tick() {
  if (m_firstRun) {
    m_firstRun = false;
    notify(kHSLevelChanged, m_level);
  }
  if (m_checkDelay == 0) {
    digitalWrite(m_powerPin, HIGH);
  } else if (m_checkDelay == 1) {
    _processData(analogRead(m_analogPin));
    digitalWrite(m_powerPin, LOW);
  }
  ++m_checkDelay;
  if (((m_checkDelay == HSCheckPumpingDelay) && _isDry(m_measurements.average()))
      || (m_checkDelay == HSCheckDelay))
  {
    m_checkDelay = 0;
  }
  return;
}

void HumiditySensor::_levelDown() {
  if (m_level > 0) {
    --m_level;
  }
  notify(kHSLevelChanged, m_level);
}

void HumiditySensor::_levelUp() {
  if (m_level < MaxLevel) {
    ++m_level;
  }
  notify(kHSLevelChanged, m_level);
}

void HumiditySensor::notify(uint8_t command, int data) const {
  Linker::instance()->notify(this, command, data);
}
