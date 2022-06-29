import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_ARROW_EXPAND_VERTICAL
from ..generic_desk import GenericDesk, CONF_DESK_ID

DEPENDENCIES = ['generic_desk']

generic_desk_sensor_ns = cg.esphome_ns.namespace('generic_desk')
GenericDeskSensor = generic_desk_sensor_ns.class_(
    'GenericDesk', sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(unit_of_measurement="cm", icon=ICON_ARROW_EXPAND_VERTICAL, accuracy_decimals=1).extend({
    cv.GenerateID(): cv.declare_id(GenericDeskSensor),
    cv.GenerateID(CONF_DESK_ID): cv.use_id(GenericDesk)
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    parent = yield cg.get_variable(config[CONF_DESK_ID])
    var = cg.new_Pvariable(config[CONF_ID])

    yield sensor.register_sensor(var, config)

    cg.add(parent.register_sensor(var))
