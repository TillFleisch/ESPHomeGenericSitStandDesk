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
  #Optional correction factor, in case the height does not scale properly
  correction_term: 1.24836

sensor:
  - platform: desk_height_sensor
    desk_id: desk1
    id: desk_height
    name: "Desk Height"

binary_sensor:
 - platform: desk_is_moving_sensor
   desk_id: desk1
   id: desk_is_moving
   name: "Desk is Moving"

switch:
  - platform: desk_switch
    desk_id: desk1
    name: "Up"
    direction: true

  - platform: desk_switch
    desk_id: desk1
    name: "Down"
    direction: false  

button:
  - platform: desk_memory_button
    desk_id: desk1
    name: "M1"
    memory_id: 1

  - platform: desk_memory_button
    desk_id: desk1
    name: "M2"
    memory_id: 2

  - platform: desk_memory_button
    desk_id: desk1
    name: "M3"
    memory_id: 3

  - platform: desk_memory_button
    desk_id: desk1
    name: "M4"
    memory_id: 4
```
An example configuration can be found [here](desk.yaml).

## Entities
This Component can provide the following entities:

**Controls:**
- M1
- M2 
- M3
- M4
- Up
- Down

**Sensors:**
- height
- is moving


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

