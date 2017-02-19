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

uint8_t HumiditySensor::read(VirtualBuffer &buffer) {
  Object::read(buffer);
  buffer.read(&m_powerPin, sizeof(m_powerPin));
  buffer.read(&m_analogPin, sizeof(m_analogPin));
  buffer.read(&m_level, sizeof(m_level));
  buffer.skip(sizeof(uint8_t));
  return 5;
}

uint8_t HumiditySensor::write(VirtualBuffer &buffer) {
  Object::write(buffer);
  buffer.write(&m_powerPin, sizeof(m_powerPin));
  buffer.write(&m_analogPin, sizeof(m_analogPin));
  buffer.write(&m_level, sizeof(m_level));
  uint8_t avg = m_measurements.average();
  buffer.write(&avg, sizeof(avg));
  return 5;
}


uint8_t HumiditySensor::update(const Linker* linker, uint8_t command, const void *) {
  if (command == kCmdTick) {
    return _tick(linker);
  }
  if (command == kCmdLevelUp) {
    _levelUp(linker);
  } else if (command == kCmdLevelDown) {
    _levelDown(linker);
  } else {
    return 0;
  }
  return notify(linker, kSigHSLevelChanged, &m_level);
}

void HumiditySensor::_setup() {
  pinMode(m_powerPin, OUTPUT);
  pinMode(m_analogPin, INPUT);
  digitalWrite(m_powerPin, LOW);
}

uint8_t HumiditySensor::_processData(const Linker* linker, int data) {
  uint8_t rv = 0;
  if (data > NoConnectionLevel) {
    data = 0;
  }
  m_measurements.add(data/LevelDevider);
  uint8_t avg = m_measurements.average();
  if (_isWet(avg)) {
    rv |= notify(linker, kSigHSWet, &avg);
  } else if (_isDry(avg)) {
    rv |= notify(linker, kSigHSDry, &avg);
  }
  rv |= notify(linker, kSigHSValue, &avg);
  return rv;
}

uint8_t HumiditySensor::_tick(const Linker* linker) {
  uint8_t rv = 0;
  if (m_firstRun) {
    m_firstRun = false;
    rv = notify(linker, kSigHSLevelChanged, &m_level);
  }
  if (m_checkDelay == 0) {
    rv = kNeedShortDelay;
    digitalWrite(m_powerPin, HIGH);
  } else if (m_checkDelay == 1) {
    rv = _processData(linker, analogRead(m_analogPin));
    digitalWrite(m_powerPin, LOW);
  }
  ++m_checkDelay;
  if (((m_checkDelay == HSCheckPumpingDelay) && !_isWet(m_measurements.average()))
      || (m_checkDelay == HSCheckDelay))
  {
    m_checkDelay = 0;
  }
  return rv;
}

void HumiditySensor::_levelDown(const Linker* linker) {
  if (m_level > 0) {
    --m_level;
  }
}

void HumiditySensor::_levelUp(const Linker *linker) {
  if (m_level < MaxLevel) {
    ++m_level;
  }
}
