#ifndef BOARD_H
#define BOARD_H

#include <cstddef>
#include <memory>
#include <vector>

#include "../switch/analog_switch.h"

class Board {
 public:
  Board() = default;
  virtual ~Board() = default;

  virtual std::vector<std::shared_ptr<AnalogInput>>& GetAnalogInputs() = 0;
  virtual std::vector<std::shared_ptr<DigitalInput>>& GetDigitalInputs() = 0;
  virtual AnalogOutput& GetPowerLed() = 0;
};

#endif
