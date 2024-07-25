
# ADB2USB Keyboard

This project is an Arduino program enabling communication between an ADB (Apple Desktop Bus) keyboard (such as the Apple Extended Keyboard II) and a modern USB computer.

This project with a Pro Micro, which uses an AVR microcontroller. It should work with any other AVR Arduino with HID support and 5V power/logic.

Please see the Other Notes section below for more information on the current limitations to this project.

* Fun fact: The Pro Micro board uses an ATmega32U4 microcontroller made by Microchip. The Apple Extended Keyboard II uses a PIC microcontroller, also made by Microchip.

Why did I make this? You may ask. Let me answer this question by providing a little background knowledge. (Feel free to skip the background knowledge section if you are not interested)


## Background Knowledge
It is a warm summer evening in North Texas. It is too hot to do anything outside, so like a normal person after work I decide to browse Ebay to make yet another irresponsible financial decision. I found an Apple Extended Keyboard Il in very good condition, only to notice it uses a protocol called ADB and not USB. I end up buying the keyboard anyway, seeing that this will give me a project to do indoors for the summer.

It is another but warmer summer evening in North Texas in which I
received the keyboard in the mail. After spending the last few days understanding the ADB protocol, making trips to MicroCenter, and deciding to write the ADB to USB adapter myself, I finally got to test my adapter on the Keyboard. As the evening cooled down a few degrees outside, I managed to fix the timing issues and got the keyboard fully working.

That same evening I realized that this project only took me a few days to get fully working, and I would need more things to do to further extend the project and avoid going outside. This is when I realized I can clean up the project, document everything as well as I could, and publish this project to the public for others who do wish to go outside during the summer instead of being indoors making their own ADB to USB adapter.
## Hardware

The hardware modifications to the device are very simple. You need any female 4-pin Mini-DIN cable (commonly used for S-Video) and a 1 kohm resistor.
 
NOTE: The ADB specification states a 750 ohm resistor for the data line pullup resistor, however multiple forums mentioned using a 1 kohm resistor, so I used a 1 kohm resistor. I have not tried using a 750 ohm pullup resistor, however it should work as it is the resistance in the ADB specification.

Below is the pinout for the female ADB connector as seen from the front (from the side where you can see the 4 cavities):
```
  ,--_--.                 
 / o4 3o \      1: DATA (YELLOW)
| o2   1o |     2: Not Connected (BLUE)
 -  ===  -      3: VCC (5V) (RED)
  `-___-'       4: GND (GREEN)
```
NOTE: From what I have seen, the colors mentioned above seem to be common in S-Video cables, but there is no standard as to what the wire colors are. Always use a multimeter to verify which wires are connected to each pin by checking continuity between a cavity and wire.

NOTE: VCC has to be 5V, otherwise the keyboard will NOT turn on. Make sure that it is connected to a 5V source and NOT a 3.3V source.

Connect the 1 kohm pullup resistor between VCC and DATA. The DATA wire can be connected to any GPIO pin.

In my current configuration, I have Arduino Pin 3 (Port D, Pin 0) as the DATA pin.

## Pro Micro Specific Configuration
There are two models of the Pro Micro available to purchase. Below are the specifics for both boards available.

If you are unsure on which model you are using, the one with a USB-C port is the SparkFun Qwiic Pro Mico and the one with the Micro USB port is the regular Pro Micro.

NOTE: I have not tried it with the regular Pro Micro boards, so I cannot verify if it works as expected. The information on the regular Pro Micro came from online research. I have only tried it using the SparkFun Qwiic Pro Micro.

### SparkFun Qwiic Pro Micro
For more information on properly setting up the hardware in the Arduino IDE, follow this guide by SparkFun: https://learn.sparkfun.com/tutorials/qwiic-pro-micro-usb-c-atmega32u4-hookup-guide

NOTE: Make sure that in the Arduino IDE you have the processor configured for "ATmega32U4 (5V, 16MHz)", otherwise it will not work.

### Regular Pro Micro
This board is sold as either a 5V, 16MHz configuration or a 3.3V, 8MHz configuration. Make sure that you have the 5V, 16MHz version, otherwise it will not work.

Solder the jumper J1 found in the front side of the board, next to the Micro USB port so the microcontroller is powered by USB and not the LDO.

## Software Modifications

To get the Arduino to communicate with the ADB keyboard, the proper port/pin of the data line has to be configured in LINES 60-63 in ADB2USB.ino.

Arduino likes to simplify its GPIO by assigning a number to each exposed GPIO pin (shown with silkscreen in the PCB next to the pin). However this number is useless in this program. You need to find out what port and pin the data line is attached to. This can typically be found in your device's datasheet. For reference, GPIO Pin 3 in the Pro Micro is mapped to Port D, Pin 0. You need these two to configure LINES 60-63 in ADB2USB.ino.

```c++
/* Define the port/pin for the data bus line. Currently set up for PD0 (Pin 3 in Pro Micro) */
#define BUS_PORT PORTD
#define BUS_PIN PIND
#define BUS_DATA_PIN 0
#define BUS_DDR DDRD
```

You may choose to use the keymap provided in the keymap.h file, which contains the keymap for the English ANSI layout. You may also choose to change the keymap by modifying the keymap.h file. More information is provided in the header of the keymap.h file.

ADB keyboards contain a power key used to turn on the computer. This is no longer useful in modern computers, but it is a great key to perform any custom functions. For convenience I have included two functions called handlePowerKeyPress() (starting in LINE 89 in ADB2USB.ino) and handlePowerKeyRelease() (starting in LINE 100 in ADB2USB.ino), so you can program what you want the key to do when pressed and released.

```c++
void handlePowerKeyPress(void) {
  // Enter code here
}

void handlePowerKeyRelease(void) {
  // Enter code here
}
```

If you would like to learn more about the ADB protocol, Szymon Łopaciuk wrote a very good post explaining the protocol.
Link to post: https://www.lopaciuk.eu/2021/03/26/apple-adb-protocol.html
## Other Notes
This program does not yet handle collision detection if two keyboards are plugged into the bus. This may get implemented if I get a second keyboard to test.

This program does not support hot plug/unplug of the keyboard. Although not recommended, it should work since it only supports one keyboard and the keyboard address is never changed from its default address of 2.