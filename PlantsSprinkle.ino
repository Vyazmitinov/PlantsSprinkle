#include <avr/sleep.h>
#include <avr/power.h>

#include "Common.h"
#include "Display.h"
#include "HumiditySensor.h"
#include "Alarm.h"
#include "Light.h"
#include "Pump.h"
#include "Button.h"
#include "Ticker.h"

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

const int HS0Power = 8;
const int HS1Power = 9;
const int HS0Analog = A3;
const int HS1Analog = A2;

const char HSDefaultLevel = 7;

const int SensorsCount = 2;
HumiditySensor Sensors[SensorsCount] = {
  {HS0Power, HS0Analog, HSDefaultLevel}, 
  {HS1Power, HS1Analog, HSDefaultLevel}
};

const int P1Power = 2;
const int P2Power = 3;

const int PumpsCount = 2;
Pump Pumps[PumpsCount] = {
  {P1Power},
  {P2Power}
};

Time MainTime;
Display MainDisplay(&MainTime);
Ticker MainTicker;

const int BP1Pin = 6;
const int BP2Pin = 7;
const int BP3Pin = 10;
const int BP4Pin = 11;

//ButtonPannel MainPannel(BP1Pin, BP2Pin, BP3Pin, BP4Pin);
Button ButtonDown0(BP1Pin, ButtonDown);
Button ButtonUp0(BP2Pin, ButtonUp);
Button ButtonDown1(BP3Pin, ButtonDown);
Button ButtonUp1(BP4Pin, ButtonUp);

Alarm MorningStartAlarm(&MainTime, 7, 0, 0, MorningStarted);
Alarm MorningStopAlarm(&MainTime, 9, 30, 0, MorningStopped);
Alarm EveningStartAlarm(&MainTime, 17, 0, 0, EveningStarted);
Alarm EveningStopAlarm(&MainTime, 23, 0, 0, EveningStopped);

const int LightPower = 13;
Light MainLight(LightPower);

void setup() {
//  Serial.begin(9600);

  MainTime.setup();
  MainDisplay.setup();
  
  for(uint8_t i = 0; i < SensorsCount; ++i) {
    Sensors[i].setup();
    Linker::instance()->addLink(&Sensors[i], &MainDisplay, HSValue, i);
    Linker::instance()->addLink(&Sensors[i], &MainDisplay, HSLevelChanged, i);
    Linker::instance()->addLink(&MainTicker, &Sensors[i], Tick);
//    Sensors[i].attach(HSValue, i, &MainDisplay);
//    Sensors[i].attach(HSLevelChanged, i, &MainDisplay);
//    MainTicker.attach(Tick, &Sensors[i]);
  }

  for(uint8_t i = 0; i < PumpsCount; ++i) {
    Pumps[i].setup();
    Linker::instance()->addLink(&Pumps[i], &MainDisplay, PWorkStarted, i);
    Linker::instance()->addLink(&MainTicker, &Pumps[i], Tick);
//    Pumps[i].attach(PWorkStarted, i, &MainDisplay);
//    MainTicker.attach(Tick, &Pumps[i]);
  }

  Linker::instance()->addLink(&Sensors[0], &Pumps[0], HSDry);
  Linker::instance()->addLink(&Sensors[1], &Pumps[1], HSDry);
  Linker::instance()->addLink(&Sensors[0], &Pumps[0], HSWet);
  Linker::instance()->addLink(&Sensors[1], &Pumps[1], HSWet);
//  Sensors[0].attach(HSDry, 0, &Pumps[0]);
//  Sensors[1].attach(HSDry, 1, &Pumps[1]);
//  Sensors[0].attach(HSWet, 0, &Pumps[0]);
//  Sensors[1].attach(HSWet, 1, &Pumps[1]);

  Linker::instance()->addLink(&MainTime, &MainDisplay, TimeUpdated);
  Linker::instance()->addLink(&MainTime, &MorningStartAlarm, TimeUpdated);
  Linker::instance()->addLink(&MainTime, &MorningStopAlarm, TimeUpdated);
  Linker::instance()->addLink(&MainTime, &EveningStartAlarm, TimeUpdated);
  Linker::instance()->addLink(&MainTime, &EveningStopAlarm, TimeUpdated);
  Linker::instance()->addLink(&MainTicker, &MainTime, Tick);
//  MainTime.attach(TimeUpdated, &MainDisplay);
//  MainTime.attach(TimeUpdated, &MorningStartAlarm);
//  MainTime.attach(TimeUpdated, &MorningStopAlarm);
//  MainTime.attach(TimeUpdated, &EveningStartAlarm);
//  MainTime.attach(TimeUpdated, &EveningStopAlarm);
//  MainTicker.attach(Tick, &MainTime);

  ButtonDown0.setup();
  ButtonUp0.setup();
  ButtonDown1.setup();
  ButtonUp1.setup();

  Linker::instance()->addLink(&ButtonDown0, &Sensors[0], ButtonDown);
  Linker::instance()->addLink(&ButtonUp0, &Sensors[0], ButtonUp);
  Linker::instance()->addLink(&ButtonDown1, &Sensors[1], ButtonDown);
  Linker::instance()->addLink(&ButtonUp1, &Sensors[1], ButtonUp);
//  ButtonDown0.attach(ButtonPushed, ButtonDown, &Sensors[0]);
//  ButtonUp0.attach(ButtonPushed, ButtonUp, &Sensors[0]);
//  ButtonDown1.attach(ButtonPushed, ButtonDown, &Sensors[1]);
//  ButtonUp1.attach(ButtonPushed, ButtonUp, &Sensors[1]);
  
  Linker::instance()->addLink(&MainTicker, &ButtonDown0, Tick);
  Linker::instance()->addLink(&MainTicker, &ButtonUp0, Tick);
  Linker::instance()->addLink(&MainTicker, &ButtonDown1, Tick);
  Linker::instance()->addLink(&MainTicker, &ButtonUp1, Tick);
//  MainTicker.attach(Tick, &ButtonDown0);
//  MainTicker.attach(Tick, &ButtonUp0);
//  MainTicker.attach(Tick, &ButtonDown1);
//  MainTicker.attach(Tick, &ButtonUp1);

  MainDisplay.update(HSLevelChanged, HSDefaultLevel, 0);
  MainDisplay.update(HSLevelChanged, HSDefaultLevel, 1);

  MainLight.setup();
  Linker::instance()->addLink(&MorningStartAlarm, &MainLight, MorningStarted);
  Linker::instance()->addLink(&MorningStopAlarm, &MainLight, MorningStopped);
  Linker::instance()->addLink(&EveningStartAlarm, &MainLight, EveningStarted);
  Linker::instance()->addLink(&EveningStopAlarm, &MainLight, EveningStopped);
//  MorningStartAlarm.attach(MorningStarted, &MainLight);
//  MorningStopAlarm.attach(MorningStopped, &MainLight);
//  EveningStartAlarm.attach(EveningStarted, &MainLight);
//  EveningStopAlarm.attach(EveningStopped, &MainLight);

  /*** Configure the timer. http://donalmorrissey.blogspot.ru/2011/11/sleeping-arduino-part-4-wake-up-via.html ***/
  TCCR1A = 0x00; /* Normal timer operation.*/
  TCNT1=0xF600; 
  TCCR1B = 0x05;
  TIMSK1=0x01;
}

void loop() {
  if(f_timer==1) {
    f_timer = 0;
    
    MainTicker.tick();
    /* Re-enter sleep mode. */
    enterSleep();
  }
}

