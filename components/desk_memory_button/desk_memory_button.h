#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
    namespace memory_button
    {

        class MemoryButton : public button::Button, public Component
        {
        public:
            void dump_config() override;
            void set_memory_id(int id) { this->memory_id = id - 1; };
            void set_uart_device(uart::UARTDevice *uart_device) { this->uart_device = uart_device; };

        protected:
            void press_action() override;
            int memory_id = 0;
            uart::UARTDevice *uart_device;
        };

    } // namespace output
} // namespace esphome