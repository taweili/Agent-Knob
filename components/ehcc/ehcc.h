#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ehcc {

class EhccComponent : public sensor::Sensor, public PollingComponent {
 public:
  /// Return the current state value, clamped to 0–999.
  int get_state() const;

  /// Set the state value, clamping to 0–999.
  void set_state(int value);

  /// Configure the polling update interval in milliseconds.
  void set_update_interval(uint32_t ms) { this->PollingComponent::set_update_interval(ms); }

  /// Called once during startup after all components are set up.
  void setup() override;

  /// Log component configuration details.
  void dump_config() override;

  /// Called periodically to update the sensor value.
  void update() override { this->publish_state(this->get_state()); }

 protected:
  static constexpr const char *TAG = "ehcc.component";

  int state_{0};

  /// Clamp value to valid sensor range [0, 999].
  int clamp_state(int value) const {
    if (value < 0) return 0;
    if (value > 999) return 999;
    return value;
  }
};

}  // namespace ehcc
}  // namespace esphome
