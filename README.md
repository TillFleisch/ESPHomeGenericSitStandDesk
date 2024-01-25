# ESPHomeGenericSitStandDesk

I have one of those generic relatively cheap Sit Stand Desks. In an effort to monitor my desk usage I developed this overkill solution for integrating the desk into a smart home like [Home Assistant](https://www.home-assistant.io/).

In addition to reporting the height, the ESP can also send commands to activate saved memory profiles or move the desk manually.

In combination with a chair occupancy sensor like [this one](https://community.home-assistant.io/t/diy-zigbee-chair-occupancy-sensor/239517), you can move the desk into sitting position upon taking a seat.

This is a custom Component for [ESPHome](https://esphome.io/) based on the [ESPHome UART Bus](https://esphome.io/components/uart.html).

## Requirements

- a compatible Standing Desk (so far only my desk has been tested)
- ESPHome 2021.12.0 or higher
- Home Assistant Core 2021.12 or higher

## Usage

```yaml
external_components:
  - source: github://TillFleisch/ESPHomeGenericSitStandDesk@main


uart:
  id: uart_bus
  tx_pin: TX
  rx_pin: RX
  baud_rate: 57600

generic_desk:
  id: desk1
  uart_id: uart_bus
  # The Desks height measured in the lowest position
  base_height: 66.4
  # Optional correction factor, in case the height does not scale properly
  correction_term: 1.24836

sensor:
  - platform: generic_desk
    desk_id: desk1
    id: desk_height
    name: "Desk Height"

binary_sensor:
 - platform: generic_desk
   desk_id: desk1
   id: desk_is_moving
   name: "Desk is Moving"

switch:
  - platform: generic_desk
    desk_id: desk1
    up:
      name: "Up"
    down:
      name: "Down"

button:
  - platform: generic_desk
    desk_id: desk1
    M1:
      name: "M1"
    M2:
      name: "M2"
    M3:
      name: "M3"
    M4:
      name: "M4"
```

An example configuration can be found [here](desk.yaml).

## Generic Desk

The desks hub component to which other components refer.

- **id**(**Required**, string): Controller ID which will be used by other components to refer to this desk.
- **display_uart**(**Required**, string): ID of the UART-Component connected desk-controller.
- **base_height**(**Required**, float): The Desks height measured in the lowest position in cm.
- **correction_term**(**Optional**, float): Optional correction factor, in case the height does not scale properly

## Sensor

A sensor which reports the current height of the desk in cm.

- **desk_id**(**Required**, string): The desk controllers id to which this sensor belongs.
- All other options from [Sensor](https://esphome.io/components/sensor/index.html#base-sensor-configuration)

## Binary Sensor

A binary sensor which reports if the desk is currently moving.

- **desk_id**(**Required**, string): The desk controllers id to which this binary sensor belongs.
- All other options from [Binary Sensor](https://esphome.io/components/binary_sensor/index.html#base-binary-sensor-configuration)

## Switch

Switches which control the desks movement.

- **desk_id**(**Required**, string): The desk controllers id to which switches belongs.
- **up**(**Optional**): Moves the desk upwards. May contain all other options from [Switch](https://esphome.io/components/switch/index.html#base-switch-configuration).
- **down**(**Optional**): Moves the desk downwards. May contain all other options from [Switch](https://esphome.io/components/switch/index.html#base-switch-configuration).

## Button

Buttons which select a preset on the desks controller.

- **desk_id**(**Required**, string): The desk controllers id to which buttons belongs.
- **M1**(**Optional**): Activates the `M1` preset. May contain all other options from [Button](https://esphome.io/components/button/index.html#base-button-configuration).
- **M2**(**Optional**): Activates the `M2` preset. May contain all other options from [Button](https://esphome.io/components/button/index.html#base-button-configuration).
- **M3**(**Optional**): Activates the `M3` preset. May contain all other options from [Button](https://esphome.io/components/button/index.html#base-button-configuration).
- **M4**(**Optional**): Activates the `M4` preset. May contain all other options from [Button](https://esphome.io/components/button/index.html#base-button-configuration).

## Wiring

On my particular model the Handset is connected via a RJ-45 connector.

Control Box|Handset
:-:|:-:
<img src="images/control_box.jpg" alt="Control Box" width="250"/>|  <img src="images/handset.jpg" alt="Handset" width="250"/>

The ESP is connected according to this wiring diagram. It replaces the Handset.

```
ESP             RJ-45 (T-568B)   Control Box
            1 (Orange/White)    - 
            2 (Orange)          -
VIN ------- 3 (Green/White)     - 5V
            4 (Blue)            -
            5 (Blue/White)      -
GND ------- 6 (Green)           - GND
TX  ------- 7 (Brown/White)     - RX
RX  ------- 8 (Brown)           - TX
```

Be aware that most ESPs run on 3.3v!

## Communication

For information regarding the communication between the Controller and Handset look [here](communication.md).
