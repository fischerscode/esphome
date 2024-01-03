#pragma once

#include "esphome/components/light/color_mode.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"

namespace esphome {
namespace rgbct_virtual {

class VirtualRGBCTLightOutput : public light::LightOutput {
 public:
  void set_lambda(
      std::function<void(float red, float green, float blue, float color_temperature, float white_brightness)> &&func) {
    this->func_ = func;
  }
  void set_cold_white_temperature(float cold_white_temperature) { cold_white_temperature_ = cold_white_temperature; }
  void set_warm_white_temperature(float warm_white_temperature) { warm_white_temperature_ = warm_white_temperature; }
  void set_color_interlock(bool color_interlock) { color_interlock_ = color_interlock; }

  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    if (this->color_interlock_)
      traits.set_supported_color_modes({light::ColorMode::RGB, light::ColorMode::COLOR_TEMPERATURE});
    else
      traits.set_supported_color_modes({light::ColorMode::RGB_COLOR_TEMPERATURE, light::ColorMode::COLOR_TEMPERATURE});
    traits.set_min_mireds(this->cold_white_temperature_);
    traits.set_max_mireds(this->warm_white_temperature_);
    return traits;
  }
  void write_state(light::LightState *state) override {
    float red, green, blue, color_temperature, white_brightness;

    state->current_values_as_rgbct(&red, &green, &blue, &color_temperature, &white_brightness);

    if (this->func_ != nullptr) {
      this->func_(red, green, blue, color_temperature, white_brightness);
    }
    // this->red_->set_level(red);
    // this->green_->set_level(green);
    // this->blue_->set_level(blue);
    // this->color_temperature_->set_level(color_temperature);
    // this->white_brightness_->set_level(white_brightness);
  }

 protected:
  std::function<void(float red, float green, float blue, float color_temperature, float white_brightness)> func_{
      nullptr};
  float cold_white_temperature_;
  float warm_white_temperature_;
  bool color_interlock_{true};
};

}  // namespace rgbct_virtual
}  // namespace esphome
