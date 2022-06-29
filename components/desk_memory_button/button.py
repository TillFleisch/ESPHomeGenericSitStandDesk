import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID
from ..generic_desk import GenericDesk, CONF_DESK_ID

DEPENDENCIES = ['generic_desk']

CONF_MEMORY_BUTTON_ID = 'memory_id'

memory_button_ns = cg.esphome_ns.namespace('memory_button')
MemoryButton = memory_button_ns.class_(
    "MemoryButton", button.Button, cg.Component)

CONFIG_SCHEMA = button.BUTTON_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(MemoryButton),
        cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk),
        cv.Required(CONF_MEMORY_BUTTON_ID): cv.int_
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await button.register_button(var, config)

    cg.add(var.set_memory_id(config[CONF_MEMORY_BUTTON_ID]))
    cg.add(parent.add_button(var))
