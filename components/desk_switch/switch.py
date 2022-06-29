import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from ..generic_desk import GenericDesk, CONF_DESK_ID

CONF_DIRECTION = "direction"

desk_switch_ns = cg.esphome_ns.namespace('desk_switch')
DeskSwitch = desk_switch_ns.class_('DeskSwitch', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DeskSwitch),
    cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk),
    cv.Required(CONF_DIRECTION): cv.boolean
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    cg.add(var.set_direction(config[CONF_DIRECTION]))
    cg.add(parent.add_switch(var))
