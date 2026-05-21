import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_UPDATE_INTERVAL,
    DEVICE_CLASS_EMPTY,
    ICON_EMPTY,
    UNIT_EMPTY,
    STATE_CLASS_MEASUREMENT,
)
from esphome.core import CORE

CODEOWNERS = ["@taweili"]
DEPENDENCIES = []
AUTO_LOAD = []

ehcc_ns = cg.esphome_ns.namespace("ehcc")
EhccComponent = ehcc_ns.class_("EhccComponent", sensor.Sensor, cg.PollingComponent)

CONF_EHCC_ID = CONF_ID

CONFIG_SCHEMA = sensor.sensor_schema(
    EhccComponent,
    unit_of_measurement=UNIT_EMPTY,
    icon=ICON_EMPTY,
    device_class=DEVICE_CLASS_EMPTY,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(
    {
        cv.GenerateID(): cv.declare_id(EhccComponent),
        cv.Optional(CONF_NAME, default="EHCC State"): cv.string,
        cv.Optional(CONF_UPDATE_INTERVAL, default="5s"): cv.positive_time_period_milliseconds,
    }
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    if CONF_UPDATE_INTERVAL in config:
        cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
