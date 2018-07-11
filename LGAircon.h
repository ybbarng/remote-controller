#ifndef LG_AIRCON_H
#define LG_AIRCON_H

enum class CommandType {
  normal,
  toggle,
  timer
};

enum class Mode {
  on,
  cooling,
  dehumidifying
};

enum class FanSpeed {
  low,
  high
};

enum class Toggle {
  powerSaving,
  powerCooling,
  swing
};

class LGAircon {
public:
  void sendToAirconRaw(unsigned long command);
  void sendCommandNormal(Mode mode, unsigned int temperature, FanSpeed fanSpeed);
  void sendCommandToggle(Toggle toggle, boolean on);
  void sendCommandTimer(unsigned int minutes);
  void turnOn();
  void turnOff();
private:
  unsigned long applyChecksum(unsigned long signal);
};
#endif