#ifndef TRIGGER_H
#define TRIGGER_H

class Trigger {
 public:
  virtual ~Trigger() {
  }

  virtual bool Triggered(double travel_distance_mm) = 0;
  virtual void TelePrint(int switch_id) = 0;
};

#endif
