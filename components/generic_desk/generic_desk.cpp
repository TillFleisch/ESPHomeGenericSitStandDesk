#include "esphome/core/log.h"
#include "generic_desk.h"

namespace esphome
{
    namespace generic_desk
    {

        static const char *TAG = "generic_desk.sensor";

        void GenericDesk::setup()
        {
        }

        void GenericDesk::loop()
        {
        }

        void GenericDesk::update()
        {

            static uint8_t buffer[14] = {0};
            int i = 0;

            // Read UART Bus
            while (available())
            {
                buffer[i % 14] = (uint8_t)read();
                i++;
            }

            // Check if the start sequence is correct and the CRC is matching
            if (buffer[0] == 0xE4 && buffer[1] == 0xF7 && crc16(buffer + 2, 10) == (buffer[12] << 8 | buffer[13]))
            {

                height = (((float)(buffer[7] << 8 | buffer[8])) / 100) * this->correction_term + this->base_height;

                // Publish the desk height if it changed
                if (height != last_height)
                {
                    last_height = height;
                    for (auto *height_sensor : this->height_sensors)
                        height_sensor->publish_state(height);
                    for (auto *moving_sensor : this->moving_sensors)
                        moving_sensor->publish_state(true);
                    desk_moving_debounce_counter = 0;
                }

                // Debounce desk is moving to prevent state flickering
                if (desk_moving_debounce_counter == DESK_MOVING_DEBOUNCE_THRESHOLD)
                {
                    // Set all sensors to false
                    for (auto *moving_sensor : this->moving_sensors)
                        moving_sensor->publish_state(false);

#ifdef USE_SWITCH
                    // Reset switches
                    for (auto *desk_switch : this->desk_switches)
                        desk_switch->publish_state(false);
#endif

                    desk_moving_debounce_counter++;
                }
                if (desk_moving_debounce_counter < DESK_MOVING_DEBOUNCE_THRESHOLD)
                {
                    desk_moving_debounce_counter++;
                }
            }
            else
            {
                // Send an Idle Command to retrieve a Status update as response
                write_array({0xea, 0xf5, 0x22, 0x00, 0x00, 0x22, 0x8a, 0x45});
            }
            delay(1);
        }

        void GenericDesk::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Generic SitStand Desk");
            for (auto *height_sensor : this->height_sensors)
                LOG_SENSOR("", "Height sensor: ", height_sensor);
            for (auto *moving_sensor : this->moving_sensors)
                LOG_BINARY_SENSOR("", "Is Moving binary sensor: ", moving_sensor);
        }

        /**
         * @brief CRC-16
         *
         * @param data The Data to calculate the Checksum on
         * @param len The Nr of Bytes in Data
         * @return Checksum for data
         */
        uint16_t GenericDesk::crc16(const uint8_t *data, uint8_t len)
        {
            uint16_t crc = 0xFFFF;
            while (len--)
            {
                crc ^= *data++;
                for (uint8_t i = 0; i < 8; i++)
                {
                    if ((crc & 0x01) != 0)
                    {
                        crc >>= 1;
                        crc ^= 0xA001;
                    }
                    else
                    {
                        crc >>= 1;
                    }
                }
            }
            return crc >> 8 | crc << 8;
        }

    } // namespace generic_desk
} // namespace esphome