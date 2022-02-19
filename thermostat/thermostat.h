#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "battery.h"
#include "button.h"
#include "buzzer.h"
#include "cloud.h"
#include "display.h"
#include "heater.h"
#include "thermometer.h"

/**
 * Thermometer has a minimal poll interval of 2000ms. Setting any number below
 * that would not further decrease the actual poll interval.
 */
#define THERMOMETER_POLL_MS 5000

/**
 * Maximum allowed Fahrenheit degree difference between set temperature and heat
 * index before toggling the heater.
 */
#define MAX_TEMP_DIFF_F 0.5

/**
 * Fahrenheit change for each button press.
 */
#define SET_TEMP_STEP_F 0.5

/**
 * 10k resistor between Vin and battery pin.
 */
#define BATTERY_VIN_PIN_OHM 10000

/**
 * 5.1k resistor between battery pin and ground.
 */
#define BATTERY_PIN_GND_OHM 5100

/**
 * Battery voltage doesn't change quickly.
 */
#define BATTERY_POLL_INTERVAL_MS 10000

/**
 * Debounce detection window for buttons. The lower this is, the more responsive
 * it will be when setting temperature (especially when repetitively pressing
 * buttons). However it's easier for button to bounce if this is too small.
 */
#define BUTTON_DEBOUNCE_MS 20

/**
 * How long to turn the buzzer on after a heater toggle action.
 */
#define BUZZER_ON_MS 300

/**
 * How long to wait between two cloud updates.
 */
#define CLOUD_UPDATE_INTERVAL_MS 500

class Thermostat {
 public:
  typedef struct Pins {
    uint8_t battery;  // Must be an analog pin.
    uint8_t button_up;
    uint8_t button_dn;
    uint8_t buzzer;  // Must be a PWM pin.
    uint8_t heater_indicator;
    uint8_t heater_remote;
    uint8_t thermometer;
  } ThermostatPins;

  Thermostat(ThermostatPins pins);
  void begin();
  void loop();

 private:
  void initHeater();
  void updateDisplay();
  inline Cloud::WriteVars loop_cloud() {
    return cloud_.loop(Cloud::ReadVars{.heater_on = heater_on_,
                                       .set_temp_F = set_temp_F_,
                                       .humidity = measurement_.humidity,
                                       .temp_C = measurement_.temp_C,
                                       .temp_F = measurement_.temp_F,
                                       .heat_idx_C = measurement_.heat_idx_C,
                                       .heat_idx_F = measurement_.heat_idx_F,
                                       .voltage = voltage_});
  }

  // Thermostat state.
  bool heater_on_;
  float set_temp_F_;
  Thermometer::Measurement measurement_;
  float voltage_;

  Battery battery_;
  Button button_up_;
  Button button_dn_;
  Buzzer buzzer_;
  Cloud cloud_;
  Display display_;
  Heater heater_;
  Thermometer thermometer_;

  uint8_t heater_indicator_pin_;
};

#endif /* THERMOSTAT_H */
