#ifndef SWITCH_H
#define SWITCH_H

class Switch {
 public:
  virtual ~Switch() {
  }

  virtual bool IsOn() = 0;
};

#endif
