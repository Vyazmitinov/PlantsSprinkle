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

uint8_t HumiditySensor::update(uint8_t reason, int value, uint8_t additionalData) {
  if (reason == kTick) {
    return _tick();
  } else if (reason == kButtonPushed) {
    if (additionalData == kButtonUp) {
      _levelUp();
    } else if (additionalData == kButtonDown) {
      _levelDown();
    }
    notify(kNeedToSerialize);
  }
  return 0;
}

void HumiditySensor::_setup() {
  pinMode(m_powerPin, OUTPUT);
  pinMode(m_analogPin, INPUT);
  digitalWrite(m_powerPin, LOW);
}

uint8_t HumiditySensor::_processData(int data) {
  uint8_t rv = 0;
  if (data > NoConnectionLevel) {
    data = 0;
  }
  m_measurements.add(data/LevelDevider);
  int avg = m_measurements.average();
  if (_isWet(avg)) {
    rv |= notify(kHSWet, avg);
  } else if (_isDry(avg)) {
    rv |= notify(kHSDry, avg);
  }
  rv |= notify(kHSValue, avg);
  return rv;
}

uint8_t HumiditySensor::_tick() {
  uint8_t rv = 0;
  if (m_firstRun) {
    m_firstRun = false;
    rv = notify(kHSLevelChanged, m_level);
  }
  if (m_checkDelay == 0) {
    rv = kNeedShortDelay;
    digitalWrite(m_powerPin, HIGH);
  } else if (m_checkDelay == 1) {
    rv = _processData(analogRead(m_analogPin));
    digitalWrite(m_powerPin, LOW);
  }
  ++m_checkDelay;
  if (((m_checkDelay == HSCheckPumpingDelay) && _isDry(m_measurements.average()))
      || (m_checkDelay == HSCheckDelay))
  {
    m_checkDelay = 0;
  }
  return rv;
}

uint8_t HumiditySensor::_levelDown() {
  if (m_level > 0) {
    --m_level;
  }
  return notify(kHSLevelChanged, m_level);
}

uint8_t HumiditySensor::_levelUp() {
  if (m_level < MaxLevel) {
    ++m_level;
  }
  return notify(kHSLevelChanged, m_level);
}

uint8_t HumiditySensor::notify(uint8_t command, int data) const {
  return Linker::instance()->notify(this, command, data);
}
