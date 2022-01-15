#include "esphome.h"
#define DESK_MOVING_DEBOUNCE_THRESHOLD 3

/**
 * @brief Custom Uart Sensor for generic Sit-Stand-Desks. This Sensor reports the desk height
 *
 */
class UartDeskSensor : public PollingComponent, public UARTDevice, public Sensor
{
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
  float correction_term = 1;

  int desk_moving_debounce_counter = 0;

  /**
   * @brief CRC-16
   *
   * @param data The Data to calculate the Checksum on
   * @param len The Nr of Bytes in Data
   * @return Checksum for data
   */
  uint16_t crc16(const uint8_t *data, uint8_t len)
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

public:
  /**
   * @brief Desk height sensor
   *
   */
  Sensor *desk_height = new Sensor();

  /**
   * @brief Desk movement sensor
   *
   */
  Sensor *desk_is_moving = new Sensor();

  /**
   * @brief Construct a new Uart Desk Sensor object.
   *
   * @param uart_bus_id The Uart Bus id
   * @param base_height The desks base height (measured in CM at the lowest desk position)
   * @param correction_term A linear correction term to counter drift.
   *
   * This is usefull if the reported height does not scale with the real desk height.
   */
  UartDeskSensor(UARTComponent *uart_bus_id, float base_height = 0.0, float correction_term = 1.0) : PollingComponent(250), UARTDevice(uart_bus_id)
  {
    this->base_height = base_height;
    this->correction_term = correction_term;
  }

  /**
   * @brief Request and retrieve the desk height from the desk controller
   *
   */
  void update() override
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
      float height = (((float)(buffer[7] << 8 | buffer[8])) / 100) * correction_term + base_height;

      // Publish the desk height if it changed
      if (height != desk_height->state)
      {
        desk_height->publish_state(height);
        desk_is_moving->publish_state(true);
        desk_moving_debounce_counter = 0;
      }

      // Debounce desk is moving to prevent state flickering
      if (desk_moving_debounce_counter == DESK_MOVING_DEBOUNCE_THRESHOLD)
      {
        desk_is_moving->publish_state(false);
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
};
