#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <array>
#include <functional>
#include <memory>

#include "io.h"

// TC4051B
class Multiplexer8 {
  class MInput;

  std::shared_ptr<DigitalOutput> selector_out_a_, selector_out_b_,
      selector_out_c_;
  std::shared_ptr<AnalogInput> com_in_;

  std::vector<std::shared_ptr<MInput>> inputs_;

 public:
  Multiplexer8(
      std::shared_ptr<DigitalOutput> selectorOutA,
      std::shared_ptr<DigitalOutput> selectorOutB,
      std::shared_ptr<DigitalOutput> selectorOutC,
      std::shared_ptr<AnalogInput> comIn)
      : selector_out_a_(selectorOutA),
        selector_out_b_(selectorOutB),
        selector_out_c_(selectorOutC),
        com_in_(comIn),
        inputs_(8) {
  }

  std::shared_ptr<AnalogInput> GetInput(uint8_t channel) {
    if (channel >= 8) {
      Serial.printf("ERROR: Invalid channel has been selected: %d", channel);
      return nullptr;
    }
    if (!inputs_[channel]) {
      inputs_[channel] = std::make_shared<MInput>(this, channel);
    }
    return inputs_[channel];
  }

 private:
  void SelectChannel(uint8_t channel) {
    if (channel >= 8) {
      Serial.printf("ERROR: Invalid channel has been selected: %d", channel);
      return;
    }

    selector_out_a_->Write(((channel >> 0) & 1) ? HIGH : LOW);
    selector_out_b_->Write(((channel >> 1) & 1) ? HIGH : LOW);
    selector_out_c_->Write(((channel >> 2) & 1) ? HIGH : LOW);

    // CD4051BM96 would take 0.75 micro seconds to START updating output pin in
    // worst case.
    // Additionally it takes much longer time to stabilize the output voltage.
    // As far as I tested on CD4051BM96, 8 micro seconds is not enough but 16 us
    // is.
    sleep_us(16);
  }

  std::shared_ptr<AnalogInput> ComInput() const {
    return com_in_;
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
