# TU-DO Wireless Doorbell

A wireless doorbell based on the ESP8266

## Overview

The TU-DO Wireless Doorbell is an open source and open hardware doorbell which runs over WiFi. It comes with two boards, one for the doorbell and one for the receiver, both of which utilize the ESP8266.

### Doorbell Board

The doorbell board is placed at the door and has a button to ring the doorbell. It is powered by a 9V battery, making it easy to install.

The circuit essentially consists of a button, two indicator leds, an ESP8266, a 9V battery and MOSFET power latch.

![]() INSERT IMAGE HERE

Once the ring button is pressed, power is provided to the ESP8266 and the power latch is activated. The ESP8266 then connect to the WiFi, send a TCP packet to all receivers, and then unlatches the power, thus fully powering down the device.

To provide feedback to the user, two indicator leds are used. A red one indicates that the ESP8266 is powered, while a green one indicates the successful transmission of the TCP packet.

If an error occured, the indicators will flash in a specific pattern. The following table shows the error codes and their corresponding patterns:

INSERT TABLE HERE

In general, a 9V battery should last for multiple months depending on the usage.

### Receiver Board

The receiver board is placed at the receiver's location and has a buzzer to play the ringtone. It is continuously powered by a 5V-12V power supply and acts as a server, waiting for TCP packets from the doorbell.

The circuit essentially consists of a buzzer, a red power indicator led, a white ring indicator led and a ESP8266. Optionally a power mosfet can built in to drive a ring indicator light with a higher voltage (This will also require a higher voltage power supply ex. 12V). This was done to provide more noticable visual feedback in areas that are louder (ex. a workshop in our case).

![]() INSERT IMAGE HERE

Once the ESP8266 receives a TCP packet from the door board, it will play a ringtone on the buzzer and flash the white ring indicator led.


## Gerbers, BOMs and Assembling

The gerbers for the doorbell and receiver boards can be found in the `kicad/(BOARD)/gerbers` folder. Interactive BOMs can be found [here](PAGES FOR IBOMS), which not only provides a list of all components, but also provides a interactive PCB view that allows you to see where each component is placed. This is especially useful for assembling the boards.

The PCB utilizes allot of JST connectors. The reason for this is that it allows the device to easily be built into generic enclosures. That being said, if you have access to a 3D printer, you can also print the enclosures that are provided in the `STL` folder.

## Configuring the firmware and flashing the boards

The firmware has been written using PlatformIO, thus it is highly recommended to use it for flashing the boards.

Before flashing the firware, configure all necessary settings (ex. WiFi credentials, static door ip, number of boards, etc.) in the `src/config.h` header.

> **Important note regarding IP addresses:** The current revision of the firmware relies on all boards having a static IP address. In addition, the receiver boards must be incrementally assigned as the IP addresses after the doorbell board. For example, if your doorbell board runs on `192.168.0.20`, then the first receiver board must run on `192.168.0.21`, the second on `192.168.0.22` etc.

The receiver board IPs are configured in the [platformio.ini](platformio.ini) file. Currently the targets corresponds to the TU-DOs set up, but those can easily be changed to your own setup.

To flash the boards, select the corresponding target in PlatformIO, plug in the ESP8266 of the board that you wish to flash, and click the upload button.

> Note: Attempting to flash the ESP8266 while it is inside the board may not work. It is highly recommended to remove the ESP8266 from the board before flashing.


## Firmware Overview

The firmware code is located in the `[src](src)` folder. The code is split into three parts: `[bell](src/bell/)`, `[door](src/door/)` and `[common](src/common/)`. The `bell` and `door` folders contain the code for the doorbell and receiver boards respectively, while the `common` folder contains code that is shared between the two.

The firmware was written in a object oriented manner and makes heavy use of state machines. Each device is controlled by a "main" class (ex. `Door` for the doorbell and `Bell` for the receiver). These classes then consists of multiple sub classes that handle smaller individual tasks (ex. the `Buzzer` class handles the buzzer on the receiver, the `RingSender` class handles the TCP packet sending on the doorbell, etc.).

![]() INSERT IMAGE OF UML DIAGRAM HERE

The codebase makes use of doxygen and a reference can be found [here]().

For a deeper understanding of the code, it is highly recommended to read the code comments and the doxygen reference.
We have tried to make the code as self explanatory as possible, but if you have any questions, feel free to open an issue.

## License

The TU-DO Wireless Doorbell firmware is licensed under the GPLv3 license. The hardware is licensed under the TAPR Open Hardware License v2.0.

