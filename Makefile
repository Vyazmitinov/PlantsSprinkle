ARDUINO_DIR  = /usr/share/arduino

TARGET       = main
ARDUINO_LIBS = iarduino_RTC EEPROM LiquidCrystal_I2C Wire Low-Power-master

USER_LIB_PATH = /home/sergey/Arduino/libraries

#BOARD_TAG = pro328
BOARD_TAG = uno
CPPFLAGS    += -std=c++11

# Avrdude code
#ARDUINO_PORT = /dev/ttyUSB0
ARDUINO_PORT = /dev/ttyACM0
AVRDUDE_ARD_PROGRAMMER = arduino
#AVRDUDE_ARD_BAUDRATE = 57600
AVRDUDE_ARD_BAUDRATE = 115200

include /usr/share/arduino/Arduino.mk
