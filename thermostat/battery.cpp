#include "battery.h"

Battery::Battery(uint8_t pin, int vin_pin_ohm, int pin_gnd_ohm,
                 int poll_interval_ms)
    : pin_(pin),
      poll_interval_ms_(poll_interval_ms),
      last_poll_millis_(0),
      multiplier_(OPERATING_VOLTAGE * (vin_pin_ohm + pin_gnd_ohm) /
                  pin_gnd_ohm / 1024) {}

void Battery::begin() {
  pinMode(pin_, INPUT);
}

float Battery::voltage() {
  if (last_poll_millis_ != 0 &&
      millis() - last_poll_millis_ < poll_interval_ms_) {
    return voltage_;
  }
  voltage_ = analogRead(pin_) * multiplier_;
  return voltage_;
}
