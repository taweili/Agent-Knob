#include "ehcc.h"

namespace esphome {
namespace ehcc {

void EhccComponent::setup() {
  ESP_LOGCONFIG(TAG, "EHCC: Setting up component");
  this->state_ = esphome::random_uint32() % 1000;
  ESP_LOGCONFIG(TAG, "EHCC: Initial state: %d", this->state_);
  ESP_LOGCONFIG(TAG, "EHCC: Update interval: %lu ms", (unsigned long) this->get_update_interval());
}

int EhccComponent::get_state() const {
  return this->clamp_state(this->state_);
}

void EhccComponent::set_state(int value) {
  int clamped = this->clamp_state(value);
  if (clamped != value) {
    ESP_LOGW(TAG, "EHCC: State value %d clamped to %d", value, clamped);
  }
  this->state_ = clamped;
  this->publish_state(this->state_);
}

void EhccComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "EHCC Component:");
  ESP_LOGCONFIG(TAG, "  State: %d", this->state_);
  ESP_LOGCONFIG(TAG, "  Update interval: %lu ms", (unsigned long) this->get_update_interval());
}

}  // namespace ehcc
}  // namespace esphome
