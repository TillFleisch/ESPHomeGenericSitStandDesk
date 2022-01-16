# Communication

The control box and handset communicate via a UART Bus (Baud rate 57600).
Monitoring the communication between handset and controller revealed how they talk to each other.

## Controller -> Handset
The controller sends a status report upon receiving any valid data. The response contains 28 Bytes. The first 4 Bytes indicate which device is talking. The last 4 Bytes contain a CRC-16(Modbus) checksum for the data.

| start sequence | data                          | crc-16 (big endian)|
|:--------------:|:-----------------------------:|:------------------:|
|e4 f7           | 00 00 00 14 1e 00 55 00 00 00 | 36 74              |

The 20 Bytes of data contain the target height and the desks current height. A single bit is used for special input acknowledgement.

|data||||
|:--:|:-:|:-:|:-:|
confirmation bit| target height | current height | unknown
| 00 00 00 |  14 1e | 00 55 | 00 00 00|
| 00 01 00 |  14 1e | 00 55 | 00 00 00|

The hight is given in cm with 2 decimal digits. This means dividing by 100 yields the correct height. For `0x00 0x55` this results in `0.85 cm` which is my desks lowest height (offset). The base height offset must be added to retrieve the real height value.

The control unit can acknowledge actions requested from the handset. If the user holds the memory button, the controller goes into 'save memory setting' mode. This is acknowledged by setting the confirmation Bit.
After entering a memory number (1-4) the controller saves the data and removes the acknowledgement bit. The height is saved to persistent memory.
This procedure is also used for resetting and collision detection settings.

Error messages were not retrieved from the device.


## Handset -> Controller

Commands send from the handset to the controller have a similar structure. The start sequence is different and the actual data only has 8 bytes.

| start sequence | data        | crc-16 (big endian)|
|:--------------:|:-----------:|:------------------:|
|ea f5           | 41 00 00 41 | d4 28              |

A list of all commands I was able to retrieve from the device:

| Command   | Data  |
| :------   | :---: |
| Idle      | ea f5 22 00 00 22 8a 45 |
| Idle      | ea f5 22 00 01 22 8b d5 |
| Memory    | ea f5 40 00 00 40 14 14 |
| Memory 1  | ea f5 41 00 00 41 d4 28 |
| Memory 2  | ea f5 42 00 00 42 94 6d |
| Memory 3  | ea f5 43 00 00 43 54 51 |
| Memory 4  | ea f5 44 00 00 44 14 E7 |
| Up        | ea f5 11 00 00 11 c5 14 |
| Down      | ea f5 33 00 00 33 4f 75 |
| Up + Down | ea f5 88 00 00 88 2b e2 |
| M+1       | ea f5 60 00 00 60 1e 0c |
| M+2       | ea f5 70 00 00 70 1b 00 |
| M+2 -> 1  | ea f5 71 00 00 71 db 3c |
| M+2 -> 2  | ea f5 72 00 00 72 9b 79 |
| M+2 -> 3  | ea f5 73 00 00 73 5b 45 |
| M+2 -> 4  | ea f5 74 00 00 74 1b f3 |

Holding a button is equivalent to sending the data repeatedly. The controller will respond after holding the button for 3 seconds.


`Idle` can be used to retrieve a status update from the controller without any effect on the desks settings.

`Up + Down` (holding) is used to reset the desk height. 

`Memory` (holding) is used to save a memory profile. After receiving the confirmation bit one of the memory commands can be send to save the current height to memory.

`Memory 1` - `Memory 4` can be send to activate the according memory setting. The desk will move to the desired target height.

`M+1` (holding) is used to switch between inches and cm. I assume the conversion takes place on the handset and not the controller. (No proof)

`M+2` (holding) is used to change the anti-collision strength rating.
After receiving the confirmation bit one of the follow up commands can be sent to set the collision strength setting.

`M+2 -> 1` a followup command for `M+2`.

`M+2 -> 1` no collision.

`M+2 -> 2` lightest collision.

`M+2 -> 3` medium collision.

`M+2 -> 4` highest collision.






