#ifndef SWITCH_H
#define SWITCH_H

class Switch {
 public:
  virtual ~Switch() {
  }
};

class DigitalSwitch : public Switch {
 public:
  virtual ~DigitalSwitch() override {
  }
};

#endif
