#pragma once

#include "esphome/core/automation.h"
#include "esphome/components/light/light_effect.h"
#include "esphome/components/light/base_light_effects.h"

namespace esphome {
namespace light {

class Flicker2LightEffect : public FlickerLightEffect {
 public:
  explicit Flicker2LightEffect(const std::string &name) : FlickerLightEffect(name) {}

  void apply() override {
    LightColorValues remote = this->state_->remote_values;
    LightColorValues current = this->state_->current_values;
    LightColorValues out;
    const float alpha = this->alpha_;
    const float beta = 1.0f - alpha;
    out.set_state(true);
    out.set_brightness(
        std::max(min_brightness_, std::min(remote.get_brightness() * beta + current.get_brightness() * alpha +
                                               (random_cubic_float() * this->intensity_),
                                           max_brightness_)));
    out.set_red(remote.get_red() * beta + current.get_red() * alpha + (random_cubic_float() * this->intensity_));
    out.set_green(remote.get_green() * beta + current.get_green() * alpha + (random_cubic_float() * this->intensity_));
    out.set_blue(remote.get_blue() * beta + current.get_blue() * alpha + (random_cubic_float() * this->intensity_));
    out.set_white(remote.get_white() * beta + current.get_white() * alpha + (random_cubic_float() * this->intensity_));
    out.set_cold_white(remote.get_cold_white() * beta + current.get_cold_white() * alpha +
                       (random_cubic_float() * this->intensity_));
    out.set_warm_white(remote.get_warm_white() * beta + current.get_warm_white() * alpha +
                       (random_cubic_float() * this->intensity_));
    out.set_color_temperature(remote.get_color_temperature());

    auto call = this->state_->make_call();
    call.set_publish(false);
    call.set_save(false);
    call.set_transition_length_if_supported(0);
    call.from_light_color_values(out);
    call.set_state(true);
    call.perform();
  }

  void set_min_brightness(float min_brightness) { this->min_brightness_ = min_brightness; }
  void set_max_brightness(float max_brightness) { this->max_brightness_ = max_brightness; }

 protected:
  float min_brightness_{};
  float max_brightness_{};
};

}  // namespace light
}  // namespace esphome
