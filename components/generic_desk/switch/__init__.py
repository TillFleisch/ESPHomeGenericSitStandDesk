import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

from .. import CONF_DESK_ID, GenericDesk

CONF_UP = "up"
CONF_DOWN = "down"

desk_switch_ns = cg.esphome_ns.namespace("desk_switch")
DeskSwitch = desk_switch_ns.class_("DeskSwitch", switch.Switch, cg.Component)

SWITCH_CONFIG = (
    switch.switch_schema(DeskSwitch)
    .extend({cv.GenerateID(): cv.declare_id(DeskSwitch)})
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk),
            cv.Optional(CONF_UP): SWITCH_CONFIG,
            cv.Optional(CONF_DOWN): SWITCH_CONFIG,
        }
    ),
    cv.has_at_least_one_key(CONF_DOWN, CONF_UP),
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])

    if up_config := config.get(CONF_UP):
        var = await switch.new_switch(up_config)
        await cg.register_component(var, up_config)

        cg.add(var.set_direction(True))
        cg.add(parent.add_switch(var))

    if down_config := config.get(CONF_DOWN):
        var = await switch.new_switch(down_config)
        await cg.register_component(var, down_config)

        cg.add(var.set_direction(False))
        cg.add(parent.add_switch(var))
