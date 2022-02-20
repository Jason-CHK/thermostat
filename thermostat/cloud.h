#ifndef CLOUD_H
#define CLOUD_H

/**
 * To enable cloud integration on other boards, add the board model here.
 */
#if defined(ARDUINO_SAMD_NANO_33_IOT)
#define ENABLE_CLOUD
#endif

#if defined(ENABLE_CLOUD)
#include <ArduinoIoTCloud.h>
#endif

/**
 * Wrapper around Arduino IoT Cloud.
 */
class Cloud {
 public:
  /**
   * Struct to hold all writable variables to modify thermostat state.
   */
  struct WriteVars {
    float set_temp_F;
  };

  /**
   * Struct to hold all read only variables to report to cloud.
   */
  struct ReadVars {
    bool heater_on;
    float set_temp_F;
    float humidity;
    float temp_C;
    float temp_F;
    float heat_idx_C;
    float heat_idx_F;
    float voltage;
  };

#if !defined(ENABLE_CLOUD)
  // No-op Cloud class definition.
  inline Cloud(int _) {}
  inline void begin() {}
  inline WriteVars loop(ReadVars vars) {
    return WriteVars{.set_temp_F = vars.set_temp_F};
  }
#else   // !defined(ENABLE_CLOUD)
  /**
   * Construct a new Cloud object.
   *
   * @param update_interval_ms Milliseconds between two cloud updates.
   */
  Cloud(int update_interval_ms);

  /**
   * Initialize connection to cloud. Block until connected.
   */
  void begin();

  /**
   * Call this in the main loop to sync states with cloud.
   *
   * @param vars Variables to be reported to cloud.
   * @return WriteVars Cloud-written variables.
   */
  WriteVars loop(ReadVars vars);

 private:
  WiFiConnectionHandler conn_;
  int update_interval_ms_;
  int last_update_millis_;

  // Cloud-synced variables.
  bool ready_;
  bool heater_on_;
  float set_temp_F_;
  float humidity_;
  float temp_C_;
  float temp_F_;
  float heat_idx_C_;
  float heat_idx_F_;
  float voltage_;

  // Read-write property references for time syncs.
  Property* set_temp_F_prop_;
#endif  // !defined(ENABLE_CLOUD)
};

#endif /* CLOUD_H */
