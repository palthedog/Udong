#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <cstddef>

template <class T>
class RingBuffer {
  T* buffer_;
  size_t buffer_size_;
  size_t head_;
  size_t tail_;
  size_t size_;

  void Increment(size_t& index) {
    if (index == buffer_size_ - 1) {
      index = 0;
    } else {
      index++;
    }
  }

  void Decrement(size_t& index) {
    if (index == 0) {
      index = buffer_size_ - 1;
    } else {
      index--;
    }
  }

 public:
  RingBuffer(size_t buffer_size)
      : buffer_(new T[buffer_size]),
        buffer_size_(buffer_size),
        head_(0),
        tail_(0),
        size_(0) {
  }

  ~RingBuffer() {
    delete[] buffer_;
  }

  void Clear() {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
  }

  bool PushBack(const T& value) {
    buffer_[tail_] = value;
    Increment(tail_);

    if (size_ == buffer_size_) {
      // Over wrote the head value.
      Increment(head_);
      return false;
    }
    size_++;
    return true;
  }

  bool PushFront(const T& value) {
    Decrement(head_);

    buffer_[head_] = value;
    if (size_ == buffer_size_) {
      // Over wrote the tail value.
      Decrement(tail_);
      return false;
    }
    size_++;
    return true;
  }

  bool PopBack(T& value) {
    if (size_ == 0) {
      return false;
    }

    Decrement(tail_);
    value = buffer_[tail_];
    size_--;
    return true;
  }

  bool PopFront(T& value) {
    if (size_ == 0) {
      return false;
    }

    value = buffer_[head_];
    Increment(head_);
    size_--;

    return true;
  }

  size_t Size() const {
    return size_;
  }
};

#endif
