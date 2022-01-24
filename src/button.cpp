#include "button.h"

// Button state to digital read result.
#define HELD LOW
#define RELEASED HIGH

Button::Button(pin_size_t pin, uint32_t debounce_ms)
    : pin_(pin), debounce_ms_(debounce_ms) {}

void Button::begin() {
  pinMode(pin_, INPUT_PULLUP);
  pending_state_ = digitalRead(pin_);
  curr_state_ = pending_state_;
  last_debounce_millis_ = millis();
  pressed_ = false;
}

void Button::loop() {
  bool reading = digitalRead(pin_);
  uint32_t now = millis();

  // Update pending state if reading is different and refresh debounce time.
  if (reading != pending_state_) {
    last_debounce_millis_ = now;
    pending_state_ = reading;
  }

  // Update current state if pending state has held for more than debounce time.
  if ((now - last_debounce_millis_) > debounce_ms_) {
    if (curr_state_ != pending_state_) {
      curr_state_ = pending_state_;

      // Set pressed state at rising edge (button being released).
      if (curr_state_ == RELEASED) {
        pressed_ = true;
      }
    }
  }
}

bool Button::pressed() {
  bool pressed = pressed_;
  pressed_ = false;
  return pressed;
}
