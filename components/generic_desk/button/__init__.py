import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID

from .. import CONF_DESK_ID, GenericDesk

DEPENDENCIES = ["generic_desk"]

CONF_MEMORY_ID_1 = "M1"
CONF_MEMORY_ID_2 = "M2"
CONF_MEMORY_ID_3 = "M3"
CONF_MEMORY_ID_4 = "M4"

ID_CONFIGS = [
    CONF_MEMORY_ID_1,
    CONF_MEMORY_ID_2,
    CONF_MEMORY_ID_3,
    CONF_MEMORY_ID_4,
]

memory_button_ns = cg.esphome_ns.namespace("memory_button")
MemoryButton = memory_button_ns.class_("MemoryButton", button.Button, cg.Component)

BUTTON_CONFIG = (
    button.button_schema(MemoryButton)
    .extend({cv.GenerateID(): cv.declare_id(MemoryButton)})
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk),
            cv.Optional(CONF_MEMORY_ID_1): BUTTON_CONFIG,
            cv.Optional(CONF_MEMORY_ID_2): BUTTON_CONFIG,
            cv.Optional(CONF_MEMORY_ID_3): BUTTON_CONFIG,
            cv.Optional(CONF_MEMORY_ID_4): BUTTON_CONFIG,
        }
    ),
    cv.has_at_least_one_key(*ID_CONFIGS),
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])

    for id, config_id in enumerate(ID_CONFIGS):
        if button_config := config.get(config_id):
            var = await button.new_button(button_config)
            cg.add(var.set_memory_id(id))
            cg.add(parent.add_button(var))
