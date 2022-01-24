#ifndef HEATER_H
#define HEATER_H

#include "Arduino.h"

/**
 * Wrapper of a heater remote over an IR LED.
 */
class Heater {
 public:
  /**
   * Construct a new Heater object.
   *
   * @param pin Pin connecting to the IR emitter.
   */
  Heater(pin_size_t pin);

  /**
   * Call this before calling switchPower.
   */
  void begin();

  /**
   * Toggle heater on or off. They are using the same command, so heater status
   * needs be tracked by caller.
   */
  void toggle();

 private:
  pin_size_t pin_;
};

#endif /* HEATER_H */
