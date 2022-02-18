#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

/**
 * Wrapper of a button with automatic debouncing.
 */
class Button {
 public:
  /**
   * Construct a new Button object.
   *
   * @param pin The pin corresponding to the button. The other end of the button
   *     should be grounded, as the pin will be pulled up internally.
   * @param debounce_ms Debounce window of the button.
   */
  Button(uint8_t pin, uint32_t debounce_ms);

  /**
   * Call this before calling loop to set up the button.
   */
  void begin();

  /**
   * Call this in the main loop.
   */
  void loop();

  /**
   * Wether button has been pressed since last call to pressed. Pressed state is
   * set at the rising edge (button held down and released).
   *
   * Calling this method will clears the pressed status.
   */
  bool pressed();

 private:
  uint8_t pin_;
  uint32_t debounce_ms_;

  /**
   * The current button state exposed to the system, i.e. whether the program
   * thinks the button is pushed.
   */
  bool curr_state_;

  /**
   * The real button state but is pending debounce, i.e. whether the button is
   * actually being pushed.
   */
  bool pending_state_;

  /**
   * The last time pending state changed.
   */
  uint32_t last_debounce_millis_;

  bool pressed_;
};

#endif /* BUTTON_H */
