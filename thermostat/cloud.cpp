#include "cloud.h"

#include "arduino_secrets.h"

Cloud::Cloud(int update_interval_ms)
    : conn_(SECRET_SSID, SECRET_PASS),
      update_interval_ms_(update_interval_ms),
      last_update_millis_(0),
      heater_on_(false),
      set_temp_F_(0),
      humidity_(0),
      temp_C_(0),
      temp_F_(0),
      heat_idx_C_(0),
      heat_idx_F_(0),
      voltage_(0) {}

void Cloud::begin() {
  ArduinoCloud.setThingId(SECRET_THING_ID);

  ArduinoCloud.addProperty(heater_on_, Permission::Read);
  set_temp_F_prop_ =
      &ArduinoCloud.addProperty(set_temp_F_, Permission::ReadWrite);
  ArduinoCloud.addProperty(humidity_, Permission::Read);
  ArduinoCloud.addProperty(temp_C_, Permission::Read);
  ArduinoCloud.addProperty(temp_F_, Permission::Read);
  ArduinoCloud.addProperty(heat_idx_C_, Permission::Read);
  ArduinoCloud.addProperty(heat_idx_F_, Permission::Read);
  ArduinoCloud.addProperty(voltage_, Permission::Read);

  ArduinoCloud.begin(conn_, /* enable_watchdog */ false);

  // Block wait until connected to Arduino cloud.
  while (ArduinoCloud.connected() == 0) {
    ArduinoCloud.update();
    delay(1000);
  }
}

Cloud::WriteVars Cloud::loop(Cloud::ReadVars vars) {
  if (set_temp_F_ != vars.set_temp_F) {
    set_temp_F_prop_->updateLocalTimestamp();
  }

  heater_on_ = vars.heater_on;
  set_temp_F_ = vars.set_temp_F;
  humidity_ = vars.humidity;
  temp_C_ = vars.temp_C;
  temp_F_ = vars.temp_F;
  heat_idx_C_ = vars.heat_idx_C;
  heat_idx_F_ = vars.heat_idx_F;
  voltage_ = vars.voltage;

  // Only sync to cloud after update interval since last update.
  if (last_update_millis_ == 0 ||
      millis() - last_update_millis_ >= update_interval_ms_) {
    last_update_millis_ = millis();
  }

  return Cloud::WriteVars{.set_temp_F = set_temp_F_};
}
