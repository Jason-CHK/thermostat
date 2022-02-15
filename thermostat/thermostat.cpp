#include "thermostat.h"

#include <math.h>

Thermostat::Thermostat(ThermostatPins pins)
    : button_up_(pins.button_up, BUTTON_DEBOUNCE_MS),
      button_dn_(pins.button_dn, BUTTON_DEBOUNCE_MS),
      thermometer_(pins.thermometer, THERMOMETER_POLL_MS),
      heater_(pins.heater_remote),
      display_(),
      buzzer_(pins.buzzer),
      heater_indicator_pin_(pins.heater_indicator) {}

void Thermostat::begin() {
  button_up_.begin();
  button_dn_.begin();
  thermometer_.begin();
  heater_.begin();
  display_.begin();
  buzzer_.begin();
  pinMode(heater_indicator_pin_, OUTPUT);
  digitalWrite(heater_indicator_pin_, LOW);

  // Ask and store initial heater status.
  initHeater();
  digitalWrite(heater_indicator_pin_, heater_on_ ? HIGH : LOW);

  // Take inital thermometer measurement.
  while (!thermometer_.measurement().valid) thermometer_.loop();
  measurement_ = thermometer_.measurement();

  // Set inital temperature to be nearist to heat index.
  set_temp_F_ =
      round(measurement_.heat_idx_F / SET_TEMP_STEP_F) * SET_TEMP_STEP_F;

  // Display inital status.
  updateDisplay();
}

void Thermostat::loop() {
  button_up_.loop();
  button_dn_.loop();
  buzzer_.loop();

  // Thermostat logic.
  bool update = false;
  bool toggle_heater = false;
  if (button_up_.pressed()) {
    set_temp_F_ += SET_TEMP_STEP_F;
    update = true;
  }
  if (button_dn_.pressed()) {
    set_temp_F_ -= SET_TEMP_STEP_F;
    update = true;
  }
  if (thermometer_.measurement() != measurement_) {
    measurement_ = thermometer_.measurement();
    update = true;
  }
  if ((heater_on_ && measurement_.heat_idx_F > set_temp_F_ + MAX_TEMP_DIFF_F) ||
      (!heater_on_ &&
       measurement_.heat_idx_F < set_temp_F_ - MAX_TEMP_DIFF_F)) {
    heater_on_ = !heater_on_;
    toggle_heater = true;
    digitalWrite(heater_indicator_pin_, heater_on_ ? HIGH : LOW);
    update = true;
  }
  if (update) {
    updateDisplay();
  }

  // Perform time consuming steps.
  if (toggle_heater) {
    heater_.toggle();
    buzzer_.set_time(BUZZER_ON_MS);
  }
  thermometer_.loop();
}

void Thermostat::initHeater() {
  display_.displaySetup();
  while (true) {
    button_up_.loop();
    button_dn_.loop();
    if (button_up_.pressed()) {
      heater_on_ = true;
      return;
    }
    if (button_dn_.pressed()) {
      heater_on_ = false;
      return;
    }
  }
}

void Thermostat::updateDisplay() {
  Display::Status status;
  status.heater_on = heater_on_;
  status.set_temp_C = Thermometer::convertTempFtoC(set_temp_F_);
  status.set_temp_F = set_temp_F_;
  status.room_temp_C = measurement_.temp_C;
  status.room_temp_F = measurement_.temp_F;
  status.humidity_pct = measurement_.humidity;
  status.heat_idx_C = measurement_.heat_idx_C;
  status.heat_idx_F = measurement_.heat_idx_F;
  status.max_diff_C = Thermometer::convertDegreeFtoC(MAX_TEMP_DIFF_F);
  status.max_diff_F = MAX_TEMP_DIFF_F;
  display_.displayStatus(status);
}
