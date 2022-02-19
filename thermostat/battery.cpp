#include "battery.h"

#include "util.h"

Battery::Battery(uint8_t pin, int vin_pin_ohm, int pin_gnd_ohm,
                 int poll_interval_ms)
    : pin_(pin),
      poll_interval_ms_(poll_interval_ms),
      last_poll_millis_(0),
      voltage_(0),
      multiplier_(OPERATING_VOLTAGE * (vin_pin_ohm + pin_gnd_ohm) /
                  pin_gnd_ohm / 1024) {}

void Battery::begin() { pinMode(pin_, INPUT); }

void Battery::loop() {
  if (last_poll_millis_ != 0 &&
      millis() - last_poll_millis_ < poll_interval_ms_) {
    return;
  }
  float voltage = analogRead(pin_) * multiplier_;
  if (voltage_ == 0) voltage_ = voltage;
  voltage_ = SMOOTHING_FACTOR * voltage + (1 - SMOOTHING_FACTOR) * voltage_;
  last_poll_millis_ = millis();
}

float Battery::voltage() { return round_float(voltage_, VOLTAGE_UNIT); }
