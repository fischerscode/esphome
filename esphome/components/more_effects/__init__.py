import esphome.config_validation as cv
import logging
import esphome.codegen as cg
from esphome.components.light.effects import flicker_effect_to_code, register_monochromatic_effect

from esphome.const import (
    CONF_ALPHA,
    CONF_INTENSITY,
    CONF_MAX_BRIGHTNESS,
    CONF_MIN_BRIGHTNESS,
    CONF_NAME,
)
from esphome.components.light import light_ns
from esphome.components.light.types import FlickerLightEffect

Flicker2LightEffect = light_ns.class_("Flicker2LightEffect", FlickerLightEffect)

@register_monochromatic_effect(
    "flicker2",
    Flicker2LightEffect,
    "Flicker",
    {
        cv.Optional(CONF_ALPHA, default=0.95): cv.percentage,
        cv.Optional(CONF_INTENSITY, default=0.015): cv.percentage,
        cv.Optional(CONF_MIN_BRIGHTNESS, default=0): cv.percentage,
        cv.Optional(CONF_MAX_BRIGHTNESS, default=1): cv.percentage,
    },
)
async def flicker2_effect_to_code(config, effect_id):
    var = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(var.set_alpha(config[CONF_ALPHA]))
    cg.add(var.set_intensity(config[CONF_INTENSITY]))
    cg.add(var.set_min_brightness(config[CONF_MIN_BRIGHTNESS]))
    cg.add(var.set_max_brightness(config[CONF_MAX_BRIGHTNESS]))
    return var

CONFIG_SCHEMA = cv.Schema({})

async def to_code(config):
    return