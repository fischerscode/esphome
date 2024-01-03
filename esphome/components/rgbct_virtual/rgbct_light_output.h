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
  void set_convert_color_temp(bool convert_color_temp) { convert_color_temp_ = convert_color_temp; }

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

    if (white_brightness > 0 && this->convert_color_temp_) {
      float temp = 10000 / (this->warm_white_temperature_ +
                            (1 - color_temperature) * (this->cold_white_temperature_ - this->warm_white_temperature_));

      if (temp <= 66) {
        red = 1;
      } else {
        red = 1.292936186062745 * pow(temp - 60, -0.1332047592);
        if (red < 0) {
          red = 0;
        }
        if (red > 1) {
          red = 1;
        }
      }

      if (temp <= 66) {
        green = 0.3900815787690196 * log(temp) - 0.6318414437886275;
      } else {
        green = 1.129890860895294 * pow(temp - 60, -0.0755148492);
      }
      if (green < 0) {
        green = 0;
      }
      if (green > 1) {
        green = 1;
      }

      if (temp >= 66) {
        blue = 1;
      } else if (temp <= 19) {
        blue = 0;
      } else {
        blue = 0.543206789110196 * log(temp - 10) - 1.19625408914;
        if (blue < 0) {
          blue = 0;
        }
        if (blue > 1) {
          blue = 1;
        }
      }
    }

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
  bool convert_color_temp_;
};

}  // namespace rgbct_virtual
}  // namespace esphome
