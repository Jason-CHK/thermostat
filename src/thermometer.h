#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "DHT.h"

/**
 * Wrapper of the DHT11 sensor library.
 */
class Thermometer {
 public:
  /**
   * Structure to hold a measurement result.
   */
  struct Measurement {
    // Indicate if the measurement is valid.
    bool valid;

    float humidity;
    float temp_C;
    float temp_F;
    float heat_idx_C;
    float heat_idx_F;

    /**
     * Compare to another Measurement by comparing all fields.
     * 
     * Note: This comparison does not consider float point errors.
     */
    bool operator==(const Measurement& o) const;
    bool operator!=(const Measurement& o) const;
  };

  /**
   * Construct a new Thermometer object.
   *
   * @param pin The pin connecting to the DHT11 sensor.
   * @param poll_interval_ms Milliseconds between two polls, minimum 2s for DHT11.
   */
  Thermometer(pin_size_t pin, uint32_t poll_interval_ms);

  /**
   * Call this before calling loop to set up the thermometer.
   */
  void begin();

  /**
   * Call this in the main loop. Can be slow (~250ms).
   */
  void loop();

  /**
   * Get the latest measurement. Repeating calls to measurement before next loop
   * will get the same result.
   *
   * @return Temperature The last temperature measurement.
   */
  Measurement measurement();

  /**
   * Covert an absolute temperature value from Fahrenheit to Celsius.
   *
   * @param f Fahrenheit degrees.
   * @return float Conversion result.
   */
  static float convertTempFtoC(float f);

  /**
   * Covert a Fahrenheit degree to Celsius degree. This is used to convert a
   * temperature delta instead of an absolute temperature.
   *
   * @param f Fahrenheit degrees.
   * @return float Conversion result.
   */
  static float convertDegreeFtoC(float f);

 private:
  /**
   * Force take a new measurement from the thermometer. This takes about 250ms.
   */
  void measure();

  DHT dht_;
  uint32_t poll_interval_ms_;
  uint32_t last_poll_millis_;
  Measurement measurement_;
};

#endif /* THERMOMETER_H */
