/**
 * Entry point for the thermostat Arduino sketch. This file only defines pins.
 * All thermostat logic is handled in the Thermostat class.
 */
#include "thermostat.h"

#define BATTERY_PIN 16
#define BUTTON_UP_PIN 20
#define BUTTON_DN_PIN 3
#define BUZZER_PIN 9
#define HEATER_INDICATOR_PIN 6
#define HEATER_REMOTE_PIN 2
#define THERMOMETER_PIN 21

Thermostat thermostat(Thermostat::Pins{.battery = BATTERY_PIN,
                                       .button_up = BUTTON_UP_PIN,
                                       .button_dn = BUTTON_DN_PIN,
                                       .buzzer = BUZZER_PIN,
                                       .heater_indicator = HEATER_INDICATOR_PIN,
                                       .heater_remote = HEATER_REMOTE_PIN,
                                       .thermometer = THERMOMETER_PIN});

void setup() { thermostat.begin(); }

void loop() { thermostat.loop(); }
