#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

// Controls volume of the buzzer. Choose from [0, 255].
#define BUZZER_PWM_DUTY_CYCLE 100

/**
 * Wrapper of an active buzzer. PWM is used to reduce buzzer volume. No resistor
 * is needed.
 */
class Buzzer {
 public:
  /**
   * Construct a new Buzzer object.
   *
   * @param pin Pin connecting to the positive pin of the buzzer. Must be a PWM
   *     pin.
   */
  Buzzer(pin_size_t pin);

  /**
   * Call this at setup to enable the buzzer pin.
   */
  void begin();

  /**
   * Call this in the main loop.
   */
  void loop();

  /**
   * Turn the buzzer on for a given duration. After the duration passed the
   * buzzer will turn off.
   *
   * @param duration_ms Duration in millisecond.
   */
  void set_time(uint32_t duration_ms);

 private:
  pin_size_t pin_;

  /**
   * Time to turn off the buzzer.
   */
  uint32_t stop_millis_;
};

#endif /* BUZZER_H */
