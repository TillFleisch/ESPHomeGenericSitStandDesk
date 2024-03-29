#include "desk_memory_button.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace memory_button
    {

        static const char *const TAG = "Desk.memoryButton";

        void MemoryButton::dump_config()
        {
            LOG_BUTTON("", "Memory Button", this);
        }

        void MemoryButton::press_action()
        {
            for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
            {
                switch (this->memory_id)
                {
                case 0:
                    uart_device->write_array({0xea, 0xf5, 0x41, 0x00, 0x00, 0x41, 0xd4, 0x28});
                    break;
                case 1:
                    uart_device->write_array({0xea, 0xf5, 0x42, 0x00, 0x00, 0x42, 0x94, 0x6d});
                    break;
                case 2:
                    uart_device->write_array({0xea, 0xf5, 0x43, 0x00, 0x00, 0x43, 0x54, 0x51});
                    break;
                case 3:
                    uart_device->write_array({0xea, 0xf5, 0x44, 0x00, 0x00, 0x44, 0x14, 0xe7});
                    break;

                default:
                    break;
                }
                uart_device->flush();
                delay(5);
            }
        }

    } // namespace memory_button
} // namespace esphome