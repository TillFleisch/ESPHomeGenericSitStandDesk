#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_BUTTON
#include "./button/desk_memory_button.h"
#endif
#ifdef USE_SWITCH
#include "./switch/desk_switch.h"
#endif

#define DESK_MOVING_DEBOUNCE_THRESHOLD 3

namespace esphome
{
    namespace generic_desk
    {

        class GenericDesk : public uart::UARTDevice, public PollingComponent
        {
        public:
            void setup() override;
            void loop() override;
            void update() override;
            void dump_config() override;
#ifdef USE_SENSOR
            void register_sensor(sensor::Sensor *obj) { this->height_sensors.push_back(obj); }
#endif
#ifdef USE_BINARY_SENSOR
            void register_binary_sensor(binary_sensor::BinarySensor *obj) { this->moving_sensors.push_back(obj); }
#endif
            void set_base_height(float base_height) { this->base_height = base_height; }
            void set_correction_term(float correction_term) { this->correction_term = correction_term; }
#ifdef USE_BUTTON
            void add_button(memory_button::MemoryButton *button) { button->set_uart_device(static_cast<uart::UARTDevice *>(this)); }
#endif
#ifdef USE_SWITCH
            void add_switch(desk_switch::DeskSwitch *switch_)
            {
                this->desk_switches.push_back(switch_);
                switch_->set_uart_device(static_cast<uart::UARTDevice *>(this));
            }
#endif

        private:
            uint16_t crc16(const uint8_t *data, uint8_t len);
#ifdef USE_SENSOR
            std::vector<sensor::Sensor *> height_sensors;
#endif
#ifdef USE_BINARY_SENSOR
            std::vector<binary_sensor::BinarySensor *> moving_sensors;
#endif
#ifdef USE_SWITCH
            std::vector<desk_switch::DeskSwitch *> desk_switches;
#endif

            /**
             * @brief The desks base height in CM. Measured in the lowest position.
             *
             */
            float base_height = 0.0;

            /**
             * @brief A linear correction term to counter drift.
             *
             * This is usefull if the reported height does not scale with the real desk height.
             *
             */
            float correction_term = 1.0f;

            /**
             * @brief counter used to debounce binary outputs
             *
             * */
            int desk_moving_debounce_counter = 0;

            /**
             * @brief The Desks current height
             *
             * */
            float height = 0.0;

            /**
             * @brief The Desks last known height
             *
             * */
            float last_height = 0.0;
        };

    } // namespace generic_desk
} // namespace esphome
