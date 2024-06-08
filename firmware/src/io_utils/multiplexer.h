#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <array>
#include <functional>
#include <memory>

#include "io.h"

// TC4051B
class Multiplexer8 {
  class MInput;

  DigitalOutput *selectorOutA_, *selectorOutB_, *selectorOutC_;
  AnalogInput* comIn_;

  std::vector<std::unique_ptr<MInput>> inputs_;

 public:
  Multiplexer8(
      DigitalOutput* selectorOutA,
      DigitalOutput* selectorOutB,
      DigitalOutput* selectorOutC,
      AnalogInput* comIn)
      : selectorOutA_(selectorOutA),
        selectorOutB_(selectorOutB),
        selectorOutC_(selectorOutC),
        comIn_(comIn) {
  }

  AnalogInput* GetInput(uint8_t channel) {
    if (channel >= 8) {
      Serial.printf("ERROR: Invalid channel has been selected: %d", channel);
      return nullptr;
    }
    if (inputs_.size() <= channel) {
      inputs_.resize(channel + 1);
    }
    if (!inputs_[channel]) {
      inputs_[channel] = std::move(std::make_unique<MInput>(this, channel));
    }
    return (AnalogInput*)inputs_[channel].get();
  }

 private:
  void SelectChannel(uint8_t channel) {
    if (channel >= 8) {
      Serial.printf("ERROR: Invalid channel has been selected: %d", channel);
      return;
    }

    selectorOutA_->Write(((channel >> 0) & 1) ? HIGH : LOW);
    selectorOutB_->Write(((channel >> 1) & 1) ? HIGH : LOW);
    selectorOutC_->Write(((channel >> 2) & 1) ? HIGH : LOW);

    // TODO: Consider adding delayMicroseconds(1) here if needed.
    // TC4051B would take 0.5 micro seconds to update output pin in worst case.
  }

  AnalogInput* ComInput() const {
    return comIn_;
  }

  class MInput : public AnalogInput {
    Multiplexer8* parent_;
    uint8_t channel_;

   public:
    MInput(Multiplexer8* parent, uint8_t channel)
        : parent_(parent), channel_(channel) {
    }

    uint16_t Read() override {
      parent_->SelectChannel(channel_);
      return parent_->ComInput()->Read();
    }
  };
};

#endif
