#include <avr/sleep.h>
#include <avr/power.h>

#include "HumiditySensor.h"
#include "Common.h"
#include "Display.h"
#include "Alarm.h"
#include "Light.h"
#include "Pump.h"
#include "Button.h"
#include "Ticker.h"
#include "Buffer.h"
#include "SunAlarm.h"

volatile int f_timer=0;

ISR(TIMER1_OVF_vect) {
   if(f_timer == 0) {
     f_timer = 1;
   }
}

void enterSleep(void) {
  set_sleep_mode(SLEEP_MODE_IDLE);

  TCNT1=0xDC00; 

  sleep_enable();

  /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}

const uint8_t HS0PowerPin = 8;
const uint8_t HS1PowerPin = 9;
const uint8_t HS0AnalogPin = A3;
const uint8_t HS1AnalogPin = A2;
const uint8_t HSDefaultLevel = 7;

const int P1PowerPin = 2;
const int P2PowerPin = 3;

const int BP1Pin = 6;
const int BP2Pin = 7;
const int BP3Pin = 10;
const int BP4Pin = 11;

const int LightPowerPin = 13;

enum Objects {
  kTikerObj,
  kHSObj0,
  kHSObj1,
  kDisplayObj,
  kPumpObj0,
  kPumpObj1,
  kTimeObj,
  kButtonObj0,
  kButtonObj1,
  kButtonObj2,
  kButtonObj3,
  kLightObj,
  kSunAlarmObj
};

const uint8_t memory[] = {
  kTicker,
  kHumiditySensor, HS0PowerPin, HS0AnalogPin, HSDefaultLevel, 0,
  kHumiditySensor, HS1PowerPin, HS1AnalogPin, HSDefaultLevel, 0,
  kDisplay,
  kPump, P1PowerPin, HIGH,
  kPump, P2PowerPin, HIGH,
  kTime,
  kButton, BP1Pin,
  kButton, BP2Pin,
  kButton, BP3Pin,
  kButton, BP4Pin,
  kLight, LightPowerPin, 0,
  kSunAlarm, 7, 111, 222, 165, 66, 25, 42, 92, 66, 0,
  
  kLink, kTikerObj,     kHSObj0,      kTick,           0,
  kLink, kTikerObj,     kHSObj1,      kTick,           0,
  kLink, kHSObj0,       kDisplayObj,  kHSValue,        0,
  kLink, kHSObj1,       kDisplayObj,  kHSValue,        1,
  kLink, kHSObj0,       kDisplayObj,  kHSLevelChanged, 0,
  kLink, kHSObj1,       kDisplayObj,  kHSLevelChanged, 1,
  kLink, kTikerObj,     kPumpObj0,    kTick,           0,
  kLink, kTikerObj,     kPumpObj1,    kTick,           0,
  kLink, kPumpObj0,     kDisplayObj,  kPWorkStarted,   0,
  kLink, kPumpObj1,     kDisplayObj,  kPWorkStarted,   1,
  kLink, kHSObj0,       kPumpObj0,    kHSDry,          0,
  kLink, kHSObj0,       kPumpObj0,    kHSWet,          0,
  kLink, kHSObj1,       kPumpObj1,    kHSDry,          0,
  kLink, kHSObj1,       kPumpObj1,    kHSWet,          0,
  kLink, kTikerObj,     kTimeObj,     kTick,           0,
  kLink, kTimeObj,      kDisplayObj,  kTimeUpdated,    0,
  kLink, kButtonObj0,   kHSObj0,      kButtonPushed,   kButtonDown,
  kLink, kButtonObj1,   kHSObj0,      kButtonPushed,   kButtonUp,
  kLink, kButtonObj2,   kHSObj1,      kButtonPushed,   kButtonDown,
  kLink, kButtonObj3,   kHSObj1,      kButtonPushed,   kButtonUp,
  kLink, kTikerObj,     kButtonObj0,  kTick,           0,
  kLink, kTikerObj,     kButtonObj1,  kTick,           0,
  kLink, kTikerObj,     kButtonObj2,  kTick,           0,
  kLink, kTikerObj,     kButtonObj3,  kTick,           0,
  kLink, kTimeObj,      kSunAlarmObj, kTimeUpdated,    0,
  kLink, kSunAlarmObj,  kLightObj,    kLightOn,        0,
  kLink, kSunAlarmObj,  kLightObj,    kLightOff,       0,
};

Buffer buffer(memory, sizeof(memory));

Ticker * MainTicker = NULL;

void setup() {
  Serial.begin(9600);

  Linker * linker = Linker::instance();
  while (buffer.left()) {
    uint8_t type;
    buffer.read(type);
    switch(type) {
      case kTicker: {
        MainTicker = new Ticker();
        linker->addObject(MainTicker);
        break;
      }
      case kHumiditySensor: {
        linker->addObject(new HumiditySensor(buffer));
        break;
      }
      case kDisplay: {
        linker->addObject(new Display());
        break;
      }
      case kPump: {
        linker->addObject(new Pump(buffer));
        break;
      }
      case kTime: {
        linker->addObject(new Time());
        break;
      }
      case kAlarm: {
        linker->addObject(new Alarm(buffer));
        break;
      }
      case kButton: {
        linker->addObject(new Button(buffer));
        break;
      }
      case kLight: {
        linker->addObject(new Light(buffer));
        break;
      }
      case kSunAlarm: {
        linker->addObject(new SunAlarm(buffer));
        break;
      }
      case kLink: {
        uint8_t senderId;
        uint8_t receiverId;
        uint8_t command;
        uint8_t additionalData;

        buffer.read(senderId);
        buffer.read(receiverId);
        buffer.read(command);
        buffer.read(additionalData);

        linker->addLink(senderId, receiverId, command, additionalData);
        break;
      }
    }
  }

  /*** Configure the timer. http://donalmorrissey.blogspot.ru/2011/11/sleeping-arduino-part-4-wake-up-via.html ***/
  TCCR1A = 0x00; /* Normal timer operation.*/
  TCNT1=0xF600; 
  TCCR1B = 0x05;
  TIMSK1=0x01;
}

void loop() {
  if(f_timer==1) {
    f_timer = 0;
    
    MainTicker->tick();
    /* Re-enter sleep mode. */
    enterSleep();
  }
}

