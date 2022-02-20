#include "thermostat.h"

#include "util.h"

Thermostat::Thermostat(ThermostatPins pins)
    : battery_(pins.battery, BATTERY_VIN_PIN_OHM, BATTERY_PIN_GND_OHM,
               BATTERY_POLL_INTERVAL_MS),
      buzzer_(pins.buzzer),
      button_up_(pins.button_up, BUTTON_DEBOUNCE_MS),
      button_dn_(pins.button_dn, BUTTON_DEBOUNCE_MS),
      cloud_(CLOUD_UPDATE_INTERVAL_MS),
      display_(),
      heater_indicator_pin_(pins.heater_indicator),
      heater_(pins.heater_remote),
      thermometer_(pins.thermometer, THERMOMETER_POLL_MS) {}

void Thermostat::begin() {
  display_.begin();
  display_.displayInit();

  battery_.begin();
  button_up_.begin();
  button_dn_.begin();
  buzzer_.begin();
  cloud_.begin();
  display_.begin();
  heater_.begin();
  thermometer_.begin();

  pinMode(heater_indicator_pin_, OUTPUT);
  digitalWrite(heater_indicator_pin_, LOW);

  // Take inital thermometer measurement.
  while (!thermometer_.measurement().valid) thermometer_.loop();
  measurement_ = thermometer_.measurement();

  // Set inital temperature to be nearist to heat index.
  set_temp_F_ = round_float(measurement_.heat_idx_F, SET_TEMP_STEP_F);

  // Take initial battery measurement.
  battery_.loop();
  voltage_ = battery_.voltage();

  // Upload initial temperature to cloud.
  loop_cloud();

  // Ask and store initial heater status.
  initHeater();
  digitalWrite(heater_indicator_pin_, heater_on_ ? HIGH : LOW);

  // Display inital status.
  updateDisplay();
}

void Thermostat::loop() {
  battery_.loop();
  button_up_.loop();
  button_dn_.loop();
  buzzer_.loop();

  // Thermostat logic.
  bool update = false;
  bool toggle_heater = false;

  // Check cloud for updates. This is time consuming so check this before
  // buttons.
  Cloud::WriteVars updatedVars = loop_cloud();
  if (updatedVars.set_temp_F != set_temp_F_) {
    set_temp_F_ = updatedVars.set_temp_F;
    update = true;
  }

  // Check buttons.
  if (button_up_.pressed()) {
    set_temp_F_ += SET_TEMP_STEP_F;
    update = true;
  }
  if (button_dn_.pressed()) {
    set_temp_F_ -= SET_TEMP_STEP_F;
    update = true;
  }

  // Measure temperature.
  Thermometer::Measurement new_measurement = thermometer_.measurement();
  if (new_measurement != measurement_) {
    measurement_ = new_measurement;
    update = true;
  }

  // Measure battery voltage.
  float new_voltage = battery_.voltage();
  if (new_voltage != voltage_) {
    voltage_ = new_voltage;
    update = true;
  }

  // Set heater to be toggled if needed.
  if ((heater_on_ && measurement_.heat_idx_F > set_temp_F_ + MAX_TEMP_DIFF_F) ||
      (!heater_on_ &&
       measurement_.heat_idx_F < set_temp_F_ - MAX_TEMP_DIFF_F)) {
    heater_on_ = !heater_on_;
    toggle_heater = true;
    digitalWrite(heater_indicator_pin_, heater_on_ ? HIGH : LOW);
    update = true;
  }

  // Update displayed states.
  if (update) {
    updateDisplay();
  }

  // Perform time consuming steps. Latency should be minimized between user
  // input to display.
  if (toggle_heater) {
    heater_.toggle();
    buzzer_.set_time(BUZZER_ON_MS);
  }
  thermometer_.loop();
}

void Thermostat::initHeater() {
  display_.displaySetup();
  while (true) {
    loop_cloud();
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
  status.battery_V = voltage_;
  display_.displayStatus(status);
}
