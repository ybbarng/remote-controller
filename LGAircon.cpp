#include <Arduino.h>
#include <IRremote.h>
#include "LGAircon.h"

unsigned long COMMAND_HEADER = 0x8800000;
unsigned long TYPE_NORMAL = 0x00000;
unsigned long TYPE_TOGGLE = 0x10000;
unsigned long TYPE_TIMER = 0xA0000;

unsigned long MODE_ON = 0x0000;
unsigned long MODE_COOLING = 0x8000;
unsigned long MODE_DEHUMIDIFYING = 0x9000;
unsigned long FAN_SPEED_LOW = 0x00;
unsigned long FAN_SPEED_HIGH = 0x40;

unsigned long TOGGLE_POWER_SAVING_ON = 0x0040;
unsigned long TOGGLE_POWER_SAVING_OFF = 0x0050;
unsigned long TOGGLE_POWER_COOLING = 0x0080;
unsigned long TOGGLE_SWING_ON = 0x3140;
unsigned long TOGGLE_SWING_OFF = 0x3150;

IRsend irsend;

unsigned long LGAircon::applyChecksum(unsigned long signal) {
  signal = signal >> 4;
    return ((signal << 4) | ((((signal & 0xF000) >> 12) +
    ((signal & 0xF00) >> 8) +
    ((signal & 0xF0) >> 4) +
    ((signal & 0xF))) & 0xF));
}

void LGAircon::sendToAirconRaw(unsigned long command) {
  command = COMMAND_HEADER | applyChecksum(command);
  /*
  Serial.print("AIRCON COMMAND RAW: ");
  Serial.println(command, HEX);
  */
  irsend.sendLG(command, 28);
}

void LGAircon::sendCommandNormal(Mode mode, unsigned int temperature, FanSpeed fanSpeed) {
  unsigned long modeValue = 0;
  unsigned long fanSpeedValue = 0;
  switch(mode) {
    case Mode::on:
      modeValue = MODE_ON;
      break;
    case Mode::cooling:
      modeValue = MODE_COOLING;
      break;
    case Mode::dehumidifying:
      modeValue = MODE_DEHUMIDIFYING;
      break;
  }
  switch(fanSpeed) {
    case FanSpeed::low:
      fanSpeedValue = FAN_SPEED_LOW;
      break;
    case FanSpeed::high:
      fanSpeedValue = FAN_SPEED_HIGH;
      break;
  }
  sendToAirconRaw(TYPE_NORMAL | modeValue | ((temperature - 15) << 8) | fanSpeedValue);
}

void LGAircon::sendCommandToggle(Toggle toggle, boolean on) {
  unsigned long command = 0;
  switch(toggle) {
    case Toggle::powerSaving:
      if (on) {
        command = TOGGLE_POWER_SAVING_ON;
      } else {
        command = TOGGLE_POWER_SAVING_OFF;
      }
      break;
    case Toggle::powerCooling:
      command = TOGGLE_POWER_COOLING;
      break;
    case Toggle::swing:
      if (on) {
        command = TOGGLE_SWING_ON;
      } else {
        command = TOGGLE_SWING_OFF;
      }
      break;
  }
  sendToAirconRaw(TYPE_TOGGLE | command);
}

void LGAircon::sendCommandTimer(unsigned int minutes) {
  minutes = minutes > 0xFFF ? 0 : minutes;
  sendToAirconRaw(TYPE_TIMER | (minutes << 4));
}

void LGAircon::turnOn() {
  sendCommandNormal(Mode::on, 18, FanSpeed::high);
}

void LGAircon::turnOff() {
  sendToAirconRaw(0xC0051);
}