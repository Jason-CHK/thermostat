#ifndef CLOUD_H
#define CLOUD_H

#include <ArduinoIoTCloud.h>

#include "thermometer.h"

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

  Cloud(int update_interval_ms);
  void begin();
  WriteVars loop(ReadVars vars);

  void update();

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
};

#endif /* CLOUD_H */
