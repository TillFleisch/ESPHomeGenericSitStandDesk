import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_ARROW_EXPAND_VERTICAL

from .. import CONF_DESK_ID, GenericDesk

DEPENDENCIES = ["generic_desk"]

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement="cm", icon=ICON_ARROW_EXPAND_VERTICAL, accuracy_decimals=1
).extend({cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk)})


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])
    var = await sensor.new_sensor(config)

    cg.add(parent.register_sensor(var))
