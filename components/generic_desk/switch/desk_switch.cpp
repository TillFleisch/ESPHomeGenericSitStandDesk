#include "esphome/core/log.h"
#include "desk_switch.h"

namespace esphome
{
    namespace desk_switch
    {

        static const char *TAG = "desk.switch";

        void DeskSwitch::loop()
        {
            // Repeatedly sends the according command to move the desk in the desired direction
            uint32_t time = millis();
            if (this->state && time - last_send > 250)
            {
                if (this->direction)
                    uart_device->write_array({0xea, 0xf5, 0x11, 0x00, 0x00, 0x11, 0xc5, 0x14});
                else
                    uart_device->write_array({0xea, 0xf5, 0x33, 0x00, 0x00, 0x33, 0x4f, 0x75});
                uart_device->flush();
                last_send = millis();
            }
        }

        void DeskSwitch::setup()
        {
        }

        void DeskSwitch::write_state(bool state)
        {
            this->publish_state(state);
            if (!state)
            {
                // Send a Idle command to cancle the execution of the current desk movement.
                uart_device->write_array({0xea, 0xf5, 0x22, 0x00, 0x00, 0x22, 0x8a, 0x45});
                uart_device->flush();
            }
        }

        void DeskSwitch::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Desk switch %s", (this->direction ? "up" : "down"));
        }

    } // namespace desk_switch
} // namespace esphome