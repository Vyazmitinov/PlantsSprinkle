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

const uint8_t memory[] = {
  E_Ticker,                                                     // 0
  E_HumiditySensor, HS0PowerPin, HS0AnalogPin, HSDefaultLevel, 0,  // 1
  E_HumiditySensor, HS1PowerPin, HS1AnalogPin, HSDefaultLevel, 0,  // 2
  E_Display,                                                    // 3
  E_Pump, P1PowerPin, HIGH,                                     // 4
  E_Pump, P2PowerPin, HIGH,                                     // 5
  E_Time,                                                       // 6
  E_Alarm, 7, 0, 0,                                             // 7
  E_Alarm, 9, 30, 0,                                            // 8
  E_Alarm, 17, 0, 0,                                            // 9
  E_Alarm, 23, 0, 0,                                            // 10
  E_Button, BP1Pin,                                             // 11
  E_Button, BP2Pin,                                             // 12
  E_Button, BP3Pin,                                             // 13
  E_Button, BP4Pin,                                             // 14
  E_Light, LightPowerPin, 0,                                    // 15
  E_SunAlarm, 7, 111, 222, 165, 66, 25, 42, 92, 66, 0,          // 16
  E_Link, 0, 1, Tick, 0, //    Linker::instance()->addLink(&MainTicker, &Sensors[0], Tick);
  E_Link, 0, 2, Tick, 0, //    Linker::instance()->addLink(&MainTicker, &Sensors[1], Tick);
  E_Link, 1, 3, HSValue, 0, //    Linker::instance()->addLink(&Sensors[0], &MainDisplay, HSValue, 0);
  E_Link, 2, 3, HSValue, 1, //    Linker::instance()->addLink(&Sensors[1], &MainDisplay, HSValue, 1);
  E_Link, 1, 3, HSLevelChanged, 0, //    Linker::instance()->addLink(&Sensors[0], &MainDisplay, HSLevelChanged, 0);
  E_Link, 2, 3, HSLevelChanged, 1, //    Linker::instance()->addLink(&Sensors[1], &MainDisplay, HSLevelChanged, 1);
  E_Link, 0, 4, Tick, 0, //    Linker::instance()->addLink(&MainTicker, &Pumps[0], Tick);
  E_Link, 0, 5, Tick, 0, //    Linker::instance()->addLink(&MainTicker, &Pumps[1], Tick);
  E_Link, 4, 3, PWorkStarted, 0, //    Linker::instance()->addLink(&Pumps[0], &MainDisplay, PWorkStarted, 0);
  E_Link, 5, 3, PWorkStarted, 1, //    Linker::instance()->addLink(&Pumps[1], &MainDisplay, PWorkStarted, 1);
  E_Link, 1, 4, HSDry, 0, //  Linker::instance()->addLink(&Sensors[0], &Pumps[0], HSDry);
  E_Link, 1, 4, HSWet, 0, //  Linker::instance()->addLink(&Sensors[0], &Pumps[0], HSWet);
  E_Link, 2, 5, HSDry, 0, //  Linker::instance()->addLink(&Sensors[1], &Pumps[1], HSDry);
  E_Link, 2, 5, HSWet, 0, //  Linker::instance()->addLink(&Sensors[1], &Pumps[1], HSWet);
  E_Link, 0, 6, Tick, 0, //  Linker::instance()->addLink(&MainTicker, &MainTime, Tick);
  E_Link, 6, 3, TimeUpdated, 0, //  Linker::instance()->addLink(&MainTime, &MainDisplay, TimeUpdated);
  E_Link, 6, 7, TimeUpdated, 0, //  Linker::instance()->addLink(&MainTime, &MorningStartAlarm, TimeUpdated);
  E_Link, 6, 8, TimeUpdated, 0, //  Linker::instance()->addLink(&MainTime, &MorningStopAlarm, TimeUpdated);
  E_Link, 6, 9, TimeUpdated, 0, //  Linker::instance()->addLink(&MainTime, &EveningStartAlarm, TimeUpdated);
  E_Link, 6, 10, TimeUpdated, 0, //  Linker::instance()->addLink(&MainTime, &EveningStopAlarm, TimeUpdated);
  E_Link, 11, 1, ButtonPushed, ButtonDown, //  Linker::instance()->addLink(&ButtonDown0, &Sensors[0], ButtonDown);
  E_Link, 12, 1, ButtonPushed, ButtonUp, //  Linker::instance()->addLink(&ButtonUp0, &Sensors[0], ButtonUp);
  E_Link, 13, 2, ButtonPushed, ButtonDown, //  Linker::instance()->addLink(&ButtonDown1, &Sensors[1], ButtonDown);
  E_Link, 14, 2, ButtonPushed, ButtonUp, //  Linker::instance()->addLink(&ButtonUp1, &Sensors[1], ButtonUp);
  E_Link, 0, 11, Tick, 0, //  Linker::instance()->addLink(&MainTicker, &ButtonDown0, Tick);
  E_Link, 0, 12, Tick, 0, //  Linker::instance()->addLink(&MainTicker, &ButtonUp0, Tick);
  E_Link, 0, 13, Tick, 0, //  Linker::instance()->addLink(&MainTicker, &ButtonDown1, Tick);
  E_Link, 0, 14, Tick, 0, //  Linker::instance()->addLink(&MainTicker, &ButtonUp1, Tick);
  E_Link, 6, 16, TimeUpdated, 0,
  E_Link, 16, 15, LightOn, 0,
  E_Link, 16, 15, LightOff, 0,
};

Buffer buffer(memory, sizeof(memory));

Ticker * MainTicker = NULL;

void setup() {
//  Serial.begin(9600);

  Linker * linker = Linker::instance();
  while (buffer.left()) {
    uint8_t type;
    buffer.read(type);
    switch(type) {
      case E_Ticker: {
        MainTicker = new Ticker();
        linker->addObject(MainTicker);
        break;
      }
      case E_HumiditySensor: {
        linker->addObject(new HumiditySensor(buffer));
        break;
      }
      case E_Display: {
        linker->addObject(new Display());
        break;
      }
      case E_Pump: {
        linker->addObject(new Pump(buffer));
        break;
      }
      case E_Time: {
        linker->addObject(new Time());
        break;
      }
      case E_Alarm: {
        linker->addObject(new Alarm(buffer));
        break;
      }
      case E_Button: {
        linker->addObject(new Button(buffer));
        break;
      }
      case E_Light: {
        linker->addObject(new Light(buffer));
        break;
      }
      case E_SunAlarm: {
        linker->addObject(new SunAlarm(buffer));
        break;
      }
      case E_Link: {
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

