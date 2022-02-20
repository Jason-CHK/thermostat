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

class Cloud {
 public:
  struct WriteVars {
    float set_temp_F;
  };

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
  inline Cloud(int _) {}
  inline void begin() {}
  inline WriteVars loop(ReadVars vars) {
    return WriteVars{.set_temp_F = vars.set_temp_F};
  }
#else   // !defined(ENABLE_CLOUD)
  Cloud(int update_interval_ms);
  void begin();
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
