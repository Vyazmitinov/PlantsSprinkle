ARDUINO_DIR  = /usr/share/arduino

TARGET       = main
ARDUINO_LIBS = iarduino_RTC EEPROM LiquidCrystal_I2C Wire

USER_LIB_PATH = /home/sergey/Arduino/libraries

MCU          = atmega328p
F_CPU        = 16000000
CPPFLAGS    += -std=c++11

# Avrdude code
ARDUINO_PORT = /dev/ttyACM0
AVRDUDE_ARD_PROGRAMMER = arduino
AVRDUDE_ARD_BAUDRATE = 115200

include /usr/share/arduino/Arduino.mk
