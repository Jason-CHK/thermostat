#include "thermometer.h"

#define THERMOMETER_TYPE DHT11

Thermometer::Thermometer(uint8_t pin, uint32_t poll_interval_ms)
    : dht_(pin, THERMOMETER_TYPE),
      poll_interval_ms_(poll_interval_ms),
      last_poll_millis_(0) {
  measurement_.valid = false;
}

void Thermometer::begin() { dht_.begin(); }

void Thermometer::loop() {
  if (last_poll_millis_ != 0 &&
      millis() - last_poll_millis_ < poll_interval_ms_) {
    return;
  }
  measure();
  last_poll_millis_ = millis();
}

void Thermometer::measure() {
  // First function call takes 250ms, sensor signal can be 2s old.
  float humidity = dht_.readHumidity();
  float tempF = dht_.readTemperature(true /*isFahrenheit*/);

  // Exit early if any read fails. This does not update last_poll_millis_ so it
  // will retry in the next loop call.
  if (isnan(humidity) || isnan(tempF)) {
    return;
  }

  measurement_.humidity = humidity;
  measurement_.temp_F = tempF;
  measurement_.heat_idx_F =
      dht_.computeHeatIndex(tempF, humidity, true /*isFahrenheit*/);
  measurement_.temp_C = convertTempFtoC(tempF);
  measurement_.heat_idx_C = convertTempFtoC(measurement_.heat_idx_F);
  measurement_.valid = true;
}

Thermometer::Measurement Thermometer::measurement() { return measurement_; }

bool Thermometer::Measurement::operator==(const Measurement& o) const {
  return valid == o.valid && humidity == o.humidity && temp_C == o.temp_C &&
         temp_F == o.temp_F && heat_idx_C == o.heat_idx_C &&
         heat_idx_F == o.heat_idx_F;
}

bool Thermometer::Measurement::operator!=(const Measurement& o) const {
  return !(*this == o);
}

float Thermometer::convertTempFtoC(float f) { return (f - 32) * 5 / 9; }

float Thermometer::convertDegreeFtoC(float f) { return f * 5 / 9; }
