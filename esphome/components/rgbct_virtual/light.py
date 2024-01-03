import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import (
    CONF_COLOR_INTERLOCK,
    CONF_OUTPUT_ID,
    CONF_COLD_WHITE_COLOR_TEMPERATURE,
    CONF_WARM_WHITE_COLOR_TEMPERATURE,
    CONF_LAMBDA,
)

CONF_CONVERT_COLOR_TEMP = "convert_color_temp"

CODEOWNERS = ["@fischerscode"]

rgbct_virtual_ns = cg.esphome_ns.namespace("rgbct_virtual")
VirtualRGBCTLightOutput = rgbct_virtual_ns.class_("VirtualRGBCTLightOutput", light.LightOutput)

def validate_conversion_requires_interlock(value):
    if (
        CONF_COLOR_INTERLOCK in value
        and CONF_CONVERT_COLOR_TEMP in value
        and value[CONF_CONVERT_COLOR_TEMP]
        and not [CONF_COLOR_INTERLOCK]
    ):
        raise cv.Invalid(
            "Color temperature conversion requires enabling color interlock.",
            path=[CONF_CONVERT_COLOR_TEMP],
        )
    return value

CONFIG_SCHEMA = cv.All(
    light.RGB_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(VirtualRGBCTLightOutput),
            cv.Required(CONF_COLD_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
            cv.Required(CONF_WARM_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
            cv.Optional(CONF_COLOR_INTERLOCK, default=False): cv.boolean,
            cv.Optional(CONF_LAMBDA): cv.lambda_,
            cv.Optional(CONF_CONVERT_COLOR_TEMP, default=False): cv.boolean, 
        }
    ),
    light.validate_color_temperature_channels,
    validate_conversion_requires_interlock,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(config[CONF_LAMBDA], [(cg.float_, "red"),(cg.float_, "green"),(cg.float_, "blue"),(cg.float_, "color_temperature"),(cg.float_, "white_brightness")], return_type=cg.void)
        cg.add(var.set_lambda(lambda_))

    cg.add(var.set_cold_white_temperature(config[CONF_COLD_WHITE_COLOR_TEMPERATURE]))
    cg.add(var.set_warm_white_temperature(config[CONF_WARM_WHITE_COLOR_TEMPERATURE]))

    cg.add(var.set_color_interlock(config[CONF_COLOR_INTERLOCK]))
    cg.add(var.set_convert_color_temp(config[CONF_CONVERT_COLOR_TEMP]))
