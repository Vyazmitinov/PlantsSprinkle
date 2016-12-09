#ifndef PUMPER_SERIALIZER_H
#define PUMPER_SERIALIZER_H

#include "Common.h"
#include "Linker.h"
#include <EEPROM.h>

class Serializer: public IObject {
  const int TicksToSave = 30;
public:
  Serializer(VirtualBuffer &) 
    : m_need_to_save(false)
    , m_first_run(true)
  {}

  uint8_t getType() { return kSerializer; }

  virtual void update(uint8_t reason, int value, uint8_t additionalData) {
    switch (reason) {
      case kNeedToSerialize:
        Serial.println("needs to save");
        m_need_to_save = true;
        m_ticks_to_save = TicksToSave;
        break;

      case kTick: {
        if ((m_need_to_save) && (--m_ticks_to_save == 0)) {
          m_need_to_save = false;
          //_save();
        }
        _checkSerial();
      }
    }
  }
  private:
    void _save() {
      Serial.print("Saving\n");
      VirtualBuffer vbuf;
      Linker::instance()->store(vbuf);
      
      uint8_t * buf = new uint8_t[vbuf.size()];
      Buffer buffer(buf, vbuf.size());
      Linker::instance()->store(buffer);

      Size size;
      size.size_int = buffer.size();
      EEPROM.write(0, size.size_ch[0]);
      EEPROM.write(1, size.size_ch[1]);
      for (int i = 0 ; i < buffer.size() ; i++) {
        EEPROM.write(i + 2, buf[i]);
      }

      delete[] buf;
      
      Serial.print("Saving done\n");
    }
    uint8_t _readln(char * mem, uint8_t len) {
      uint8_t pos = 0;
      while (Serial.available() != 0) {
        if (len == pos - 1) {
          while (Serial.available() != 0) {
            Serial.read();
          }
          break;
        }
        mem[pos] = Serial.read();
        ++pos;
      }
      mem[pos] = 0;
      return pos;
    }
    void _printState() {
      Serial.print("Total objects size: "); 

      VirtualBuffer vbuf;
      Linker::instance()->store(vbuf);
      Serial.println(vbuf.size());
      
      uint8_t * memory = new uint8_t[vbuf.size()];
      if (memory == 0) {
        Serial.println("Can't allocate enought memory");
        return;
      }

      Buffer buffer(memory, vbuf.size());
      Linker::instance()->store(buffer);

      Serial.print("Serializer: ");
      Serial.println(buffer.size());
      for (int i = 0; i < buffer.size(); ++i) {
        Serial.print("0x");
        if (memory[i] <= 0xF) {
          Serial.print("0");
        }
        Serial.print(memory[i], HEX);
          
        if (i % 16 == 15) {
          Serial.print("\n");
        } else {
          Serial.print(" ");
        }
      }
      Serial.print("\n");
      delete[] memory;
    }
    void _checkSerial() {
      if (Serial.available() == 0) {
        return;
      }
      char str[16];
      uint8_t len = _readln(str, sizeof(str));
      Serial.println(str);
      if (strcmp(str, "serialize") == 0) {
        _printState();
      }
      if (strcmp(str, "save") == 0) {
        _save();
      }
    }

    bool m_need_to_save;
    bool m_first_run;
    int m_ticks_to_save;
};

#endif PUMPER_SERIALIZER_H

