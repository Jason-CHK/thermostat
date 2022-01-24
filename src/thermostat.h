#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "button.h"
#include "buzzer.h"
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
#define MAX_TEMP_DIFF_F 1

/**
 * Fahrenheit change for each button press.
 */
#define SET_TEMP_STEP_F 0.5

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

class Thermostat {
 public:
  typedef struct Pins {
    pin_size_t button_up;
    pin_size_t button_dn;
    pin_size_t thermometer;
    pin_size_t heater_remote;
    pin_size_t buzzer;  // Must be PWM pin.
    pin_size_t heater_indicator;
  } ThermostatPins;

  Thermostat(ThermostatPins pins);
  void begin();
  void loop();

 private:
  void initHeater();
  void updateDisplay();

  // Thermostat state.
  bool heater_on_;
  float set_temp_F_;
  Thermometer::Measurement measurement_;

  Button button_up_;
  Button button_dn_;
  Thermometer thermometer_;
  Heater heater_;
  Display display_;
  Buzzer buzzer_;
  pin_size_t heater_indicator_pin_;
};

#endif /* THERMOSTAT_H */
