#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include <algorithm>
#include <vector>

#include "io.h"

template <int BufferSize>
class RingBufferUint16 {
  std::vector<uint16_t> buffer_;
  size_t ins_pos_;

 public:
  RingBufferUint16() : ins_pos_(0) {
  }

  inline void push(uint16_t v) {
    if (size() < BufferSize) {
      buffer_.push_back(v);
    } else {
      buffer_[ins_pos_] = v;
    }

    if (ins_pos_ == BufferSize - 1) {
      ins_pos_ = 0;
    } else {
      ins_pos_ = ins_pos_ + 1;
    }
  }

  inline size_t size() {
    return buffer_.size();
  }

  inline std::vector<uint16_t>::iterator begin() {
    return buffer_.begin();
  }

  inline std::vector<uint16_t>::iterator end() {
    return buffer_.end();
  }
};

template <int BufferSize, int DropMinNum, int DropMaxNum>
class MultiSampling : public AnalogInput {
  std::shared_ptr<AnalogInput> source_input_;
  RingBufferUint16<BufferSize> buffer_;
  std::vector<uint16_t> sorted_;

 public:
  MultiSampling(std::shared_ptr<AnalogInput> source_input)
      : source_input_(source_input), sorted_(BufferSize, 0) {
  }

  uint16_t Read() override {
    if (BufferSize == 1) {
      return source_input_->Read();
    }

    if (buffer_.size() < BufferSize) {
      // warm up in a poor way...
      while (buffer_.size() < BufferSize) {
        uint16_t latest_value = source_input_->Read();
        buffer_.push(latest_value);
        delay(1);
      }
    } else {
      uint16_t latest_value = source_input_->Read();
      buffer_.push(latest_value);
    }

    std::partial_sort_copy(
        buffer_.begin(), buffer_.end(), sorted_.begin(), sorted_.end());

    uint32_t sum = 0;
    for (size_t i = DropMinNum; i < sorted_.size() - DropMaxNum; i++) {
      sum += sorted_[i];
    }
    return sum / (sorted_.size() - DropMaxNum - DropMinNum);
  }
};

#endif
