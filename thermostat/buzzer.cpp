#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin) : pin_(pin) {}

void Buzzer::begin() {
  pinMode(pin_, OUTPUT);
  analogWrite(pin_, 0);
  stop_millis_ = 0;
}

void Buzzer::loop() {
  if (stop_millis_ != 0 && millis() >= stop_millis_) {
    analogWrite(pin_, 0);
    stop_millis_ = 0;
  }
}

void Buzzer::set_time(uint32_t duration_ms) {
  analogWrite(pin_, BUZZER_PWM_DUTY_CYCLE);
  stop_millis_ = millis() + duration_ms;
}
