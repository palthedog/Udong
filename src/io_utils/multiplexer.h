#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <array>
#include <functional>
#include <memory>

#include "io.h"

// TC4051B
class Multiplexer8 {
  class MInput;

  Output *selectorOutA_, *selectorOutB_, *selectorOutC_;
  Input* comIn_;

  std::vector<std::unique_ptr<MInput>> inputs_;

 public:
  Multiplexer8(
      Output* selectorOutA, Output* selectorOutB, Output* selectorOutC,
      Input* comIn)
      : selectorOutA_(selectorOutA),
        selectorOutB_(selectorOutB),
        selectorOutC_(selectorOutC),
        comIn_(comIn) {
  }

  Output* GetOutput(uint8_t channel) {
    if (channel >= 8) {
      return nullptr;
    }
    // if (inputs_)
  }

 private:
  void SelectChannel(uint8_t channel) {
    if (channel >= 8) {
      Serial.printf("ERROR: Invalid channel has been selected: %d", channel);
      return;
    }

    selectorOutA_->DigitalWrite((channel >> 0) & 1);
    selectorOutB_->DigitalWrite((channel >> 1) & 1);
    selectorOutC_->DigitalWrite((channel >> 2) & 1);

    // TODO: Consider adding delayMicroseconds(1) here if needed.
    // TC4051B would take 0.5 micro seconds to update output pin in worst case.
  }

  Input* ComInput() const {
    return comIn_;
  }

  class MInput : Input {
    Multiplexer8* parent_;
    uint8_t channel_;

   public:
    MInput(Multiplexer8* parent, uint8_t channel)
        : parent_(parent), channel_(channel) {
    }

    int AnalogRead() {
      parent_->SelectChannel(channel_);
      return parent_->ComInput()->AnalogRead();
    }

    PinStatus DigitalRead() {
      parent_->SelectChannel(channel_);
      return parent_->ComInput()->DigitalRead();
    }
  };
};

#endif
