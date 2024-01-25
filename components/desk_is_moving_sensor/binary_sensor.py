import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from ..generic_desk import GenericDesk, CONF_DESK_ID

DEPENDENCIES = ["generic_desk"]


CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk)}
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])
    var = await binary_sensor.new_binary_sensor(config)

    cg.add(parent.register_binary_sensor(var))
