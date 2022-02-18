#ifndef BATTERY_H
#define BATTERY_H

#include "Arduino.h"

// 5k1, 10k

/**
 * Define operating voltage based on board type. Add unsupported board types
 * here.
 *
 * Board type can be found in boards.txt, with key xxx.build.board where xxx is
 * the board model.
 */
#if defined(ARDUINO_AVR_NANO_EVERY)
#define OPERATING_VOLTAGE 5
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#define OPERATING_VOLTAGE 3.3
#else
#error "Operating voltage not defined for this board."
#endif

/**
 * Monitors battery voltage.
 *
 * Connections: Vin - R1 - pin - R2 - GND
 * Formula: Vin = operating voltage * (pin reading / 1024) * ((R1 + R2) / R2)
 */
class Battery {
 public:
  /**
   * Construct a new Battery object.
   *
   * @param pin The voltage sensing pin, must be an analog pin.
   * @param vin_pin_ohm Resistance between Vin and pin (R1).
   * @param pin_gnd_ohm Resistance between pin and GND (R2).
   * @param poll_interval_ms Milliseconds between two polls.
   */
  Battery(uint8_t pin, int vin_pin_ohm, int pin_gnd_ohm, int poll_interval_ms);

  /**
   * Call this at setup to enable the battery pin.
   */
  void begin();

  /**
   * Get a reading of battery voltage Vin. This might be cached.
   *
   * @return float Voltage of Vin.
   */
  float voltage();

 private:
  uint8_t pin_;
  int poll_interval_ms_;
  uint32_t last_poll_millis_;
  float voltage_;

  /**
   * Cached multiplier. Multiply pin reading with this to calculate final
   * voltage.
   */
  float multiplier_;
};

#endif /* BATTERY_H */
