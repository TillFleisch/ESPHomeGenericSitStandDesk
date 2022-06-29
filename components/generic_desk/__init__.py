import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.components.uart import UARTComponent, CONF_UART_ID
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']
AUTO_LOAD = ['desk_memory_button', 'desk_switch', 'desk_height_sensor',
             'desk_is_moving_sensor', 'sensor', 'binary_sensor', 'button', 'switch']
MULTI_CONF = True

CONF_DESK_ID = 'desk_id'
CONF_BASE_HEIGHT = 'base_height'
CONF_CORRECTION_TERM = 'correction_term'


generic_desk_ns = cg.esphome_ns.namespace('generic_desk')
GenericDesk = generic_desk_ns.class_(
    'GenericDesk', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(GenericDesk),
    cv.Required(CONF_UART_ID): cv.use_id(UARTComponent),
    cv.Required(CONF_BASE_HEIGHT): cv.float_,
    cv.Optional(CONF_CORRECTION_TERM, 1.0): cv.float_
}).extend(cv.polling_component_schema('250ms'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_base_height(config[CONF_BASE_HEIGHT]))
    cg.add(var.set_correction_term(config[CONF_CORRECTION_TERM]))
