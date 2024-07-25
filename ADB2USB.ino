/*
 * ADB2USB.ino
 * By Francisco Fabregat
 * June 22, 2024
 *
 * This program uses an Arduino to enable communication between an ADB (Apple Desktop Bus) keyboard and a modern USB computer.
 * It works with a Pro Micro, which uses an AVR microcontroller. It should work with any other AVR Arduino with HID support and 5V power/logic
 * with proper port/pin modifications in the code (See SOFTWARE MODIFICATIONS below), however I have not tried it with any other device.
 * 
 * ------------------
 *      HARDWARE
 * ------------------
 * The hardware modifications to the device are very simple. You need any female 4-pin Mini-DIN cable (commonly used for S-Video) and a 1 kohm resistor.
 * NOTE: The ADB specification states a 750 ohm resistor for the data line pullup resistor, however multiple forums mentioned using a 1 kohm resistor,
 *       so I used a 1 kohm resistor. I have not tried using a 750 ohm pullup resistor, however it should work as it is the resistance in the ADB specification.
 *
 * Below is the pinout for the female ADB connector as seen from the front (from the side where you can see the 4 cavities):
 *      ,--_--.                 
 *     / o4 3o \      1: DATA (YELLOW)
 *    | o2   1o |     2: Not Connected (BLUE)
 *     -  ===  -      3: VCC (5V) (RED)
 *      `-___-'       4: GND (GREEN)
 *
 * NOTE: From what I have seen, the colors mentioned above seem to be common in S-Video cables, but there is no standard as to what the wire colors are.
 *       Always use a multimeter to verify which wires are connected to each pin by checking continuity between a cavity and wire.
 * NOTE: VCC has to be 5V, otherwise the keyboard will NOT turn on. Make sure that it is connected to a 5V source and NOT a 3.3V source.
 *
 * Connect the 1 kohm pullup resistor between VCC and DATA. The DATA wire can be connected to any GPIO pin.
 *
 * --------------------------------
 *      SOFTWARE MODIFICATIONS
 * --------------------------------
 * To get the Arduino to communicate with the ADB keyboard, the proper port/pin of the data line has to be configured in LINES 60-63.
 * Arduino likes to simplify its GPIO by assigning a number to each exposed GPIO pin (shown with silkscreen in the PCB next to the pin).
 * However this number is useless in this program. You need to find out what port and pin the data line is attached to. This can typically be
 * found in your device's datasheet. For reference, GPIO Pin 3 in the Pro Micro is mapped to Port D, Pin 0. You need these two to configure LINES 60-63.
 *
 * You may choose to use the keymap provided in the keymap.h file, which contains the keymap for the English ANSI layout. You may also choose to change the
 * keymap by modifying the keymap.h file. More information is provided in the header of the keymap.h file.
 *
 * ADB keyboards contain a power key used to turn on the computer. This is no longer useful in modern computers, but it is a great key to perform any custom functions.
 * For convenience I have included two functions called handlePowerKeyPress() (starting in LINE 89) and handlePowerKeyRelease() (starting in LINE 100),
 * so you can program what you want the key to do when pressed and released.
 *
 * If you would like to learn more about the ADB protocol, Szymon Łopaciuk wrote a very good post explaining the protocol.
 * Link to post: https://www.lopaciuk.eu/2021/03/26/apple-adb-protocol.html
 *
 * ---------------------
 *      OTHER NOTES
 * ---------------------
 * This program does not yet handle collision detection if two keyboards are plugged into the bus. This may get implemented if I get a second keyboard to test.
 * This program does not support hot plug/unplug of the keyboard. Although not recommended, it should work since it only supports one keyboard and the keyboard address
 * is never changed from its default address of 2.
 */

#include <Keyboard.h>
#include "keymap.h"

/* Define the port/pin for the data bus line. Currently set up for PD0 (Pin 3 in Pro Micro) */
#define BUS_PORT PORTD
#define BUS_PIN PIND
#define BUS_DATA_PIN 0
#define BUS_DDR DDRD

/* Length of each animation frame in milliseconds */
#define ANIMATION_LENGTH 175

/*
 * DEBUG
 * Uncomment the appropriate #define line to enable debug prints for each section
 *
 * COMM_DEBUG: Debug ADB communication. Will print all communication in the bus
 * KEY_DEBUG: Debug Keyboard. Will print the pressed and released keys
 *
 * NOTE: Debug prints WILL slow down program execution. Only enable if needed.
 */
#undef COMM_DEBUG
#undef KEY_DEBUG
//#define COMM_DEBUG  // <-- Uncomment line to enable COMM_DEBUG
//#define KEY_DEBUG   // <-- Uncomment line to enable KEY_DEBUG

/*
 * handlePowerKeyPress
 * This function runs when the power key is pressed
 *
 * TIP: This program uses the Arduino Keyboard library which you can also use to send keyboard commands to the host computer.
 *      Arduino Keyboard library reference: https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
 */
void handlePowerKeyPress(void) {
  // Enter code here
}

/*
 * handlePowerKeyPress
 * This function runs when the power key is released
 *
 * TIP: This program uses the Arduino Keyboard library which you can also use to send keyboard commands to the host computer.
 *      Arduino Keyboard library reference: https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
 */
void handlePowerKeyRelease(void) {
  // Enter code here
}

/* 
 * Macros to set the data bus line high or low.
 * For low: Set the data bus line pin to be an output. The output should be configured as low
 * For high: Set the data bus line to be an input. The pullup resistor will pull the data line high
 */
#define DATA_LOW() (BUS_DDR |= (1<<BUS_DATA_PIN))
#define DATA_HIGH() (BUS_DDR &= ~(1<<BUS_DATA_PIN))

/* Macro to get the value at the data bus line input */
#define DATA_IN() (BUS_PIN & (1<<BUS_DATA_PIN))

/* Function prototypes */
void reset(void);
void zero(void);
void one(void);
void attentionSync(void);
void listen(uint8_t address, uint8_t reg, uint16_t data);
bool talk(uint8_t address, uint8_t reg, uint16_t *data);
void setLights(void);
void showStartAnimation(void);

/* Hold the status of the three lights */
uint8_t num_lock_light = 0;
uint8_t caps_lock_light = 0;
uint8_t scroll_lock_light = 0;

/*
 *  Setup
 *  This function runs once at the beginning
 */
void setup() {

  #if defined COMM_DEBUG || defined KEY_DEBUG
    Serial.begin(9600);
    Serial.println("DEBUG ENABLED");
  #endif

  /* Set bus output as low, and pull bus high */
  BUS_PORT &= ~(1<<BUS_DATA_PIN);
  DATA_HIGH();

  /* Reset registers of all devices on the bus */
  reset();

  uint16_t response = 0;

  /* Wait until keyboard responds */
  while(!talk(2, 3, &response)) {
    reset();
  }

  /* Start emulating keyboard */
  Keyboard.begin();

  /* Show start animation in keyboard */
  showStartAnimation();
}

/*
 *  Loop
 *  This function runs continuously after the setup() function
 */
void loop() {
  /* Variable that holds 16 bit data read from TALK command */
  uint16_t reg0;

  /* Send TALK command to keyboard, and handle response if successful */
  if (talk(2, 0, &reg0)) {
    /* Check MSB of first key event byte (1 if key released, 0 if key pressed) */
    if (reg0 >> 15) {
      #ifdef KEY_DEBUG
        Serial.print("Key Released (1st Byte): 0x");
        Serial.print((reg0 >> 8) & 0x7F, HEX);
        Serial.print(" (");
        Serial.print((keymap[(reg0 >> 8) & 0x7F] >= 32 && keymap[(reg0 >> 8) & 0x7F] <= 126) ? (char) keymap[(reg0 >> 8) & 0x7F] : '-');
        Serial.println(")");
      #endif

      /* Check in keymap if released key is mapped to anything */
      if (keymap[(reg0 >> 8) & 0x7F]) {
        /* Check if key released is Caps Lock */
        if (keymap[(reg0 >> 8) & 0x7F] == KEY_CAPS_LOCK) {
          /*
           * Handle Caps Lock release
           *
           * NOTE: Caps Lock behaves different from other keys in the Apple Extended Keyboard II.
           *       The key is locked in the down position when pressed (activating Caps Lock), and goes back up when pressed again (turning off Caps Lock).
           *       For the host computer to recognize Caps Lock properly, it needs to behave like a normal key. It needs to be pressed and then released for each toggle.
           *       This section sends both a press and a release command for the caps lock key, as the key press in the keyboard is just registered as a key release.
           *       It also turns off the Caps Lock light in the keyboard.
           */
          Keyboard.write(KEY_CAPS_LOCK);
          caps_lock_light = 0;
          setLights();
        } else {
          /* Send a key release command with the key released */
          Keyboard.release(keymap[(reg0 >> 8) & 0x7F]);
        }
      } else if ((reg0 >> 8) & 0x7F == 0x7F) {
        /* Power button release action */
        handlePowerKeyRelease();
      }
    } else {
      #ifdef KEY_DEBUG
        Serial.print("Key Pressed (1st Byte): 0x");
        Serial.print((reg0 >> 8) & 0x7F, HEX);
        Serial.print(" (");
        Serial.print((keymap[(reg0 >> 8) & 0x7F] >= 32 && keymap[(reg0 >> 8) & 0x7F] <= 126) ? (char) keymap[(reg0 >> 8) & 0x7F] : '-');
        Serial.println(")");
      #endif

      /* Check in keymap if pressed key is mapped to anything */
      if (keymap[(reg0 >> 8) & 0x7F]) {
        /* Check if key pressed is Caps Lock */
        if (keymap[(reg0 >> 8) & 0x7F] == KEY_CAPS_LOCK) {
          /*
           * Handle Caps Lock press
           *
           * NOTE: Caps Lock behaves different from other keys in the Apple Extended Keyboard II.
           *       The key is locked in the down position when pressed (activating Caps Lock), and goes back up when pressed again (turning off Caps Lock).
           *       For the host computer to recognize Caps Lock properly, it needs to behave like a normal key. It needs to be pressed and then released for each toggle.
           *       This section sends both a press and a release command for the caps lock key, as the key press in the keyboard is just registered as a key press.
           *       It also turns on the Caps Lock light in the keyboard.
           */
          Keyboard.write(KEY_CAPS_LOCK);
          caps_lock_light = 1;
          setLights();
        } else {
          /* Send a key press command with the key pressed */
          Keyboard.press(keymap[(reg0 >> 8) & 0x7F]);
        }
      } else if ((reg0 >> 8) & 0x7F == 0x7F) {
        /* Power button press action */
        handlePowerKeyPress();
      }
    }

    /* Check MSB of second key event byte (1 if key released, 0 if key pressed) */
    if ((reg0 >> 7) & 1) {
      #ifdef KEY_DEBUG
        Serial.print("Key Released (2nd Byte): 0x");
        Serial.print(reg0 & 0x7F, HEX);
        Serial.print(" (");
        Serial.print((keymap[reg0 & 0x7F] >= 32 && keymap[reg0 & 0x7F] <= 126) ? (char) keymap[reg0 & 0x7F] : '-');
        Serial.println(")");
      #endif

      /* Check in keymap if released key is mapped to anything */
      if (keymap[reg0 & 0x7F]) {
        /* Check if key released is Caps Lock */
        if (keymap[reg0 & 0x7F] == KEY_CAPS_LOCK) {
          /*
           * Handle Caps Lock release
           *
           * NOTE: Caps Lock behaves different from other keys in the Apple Extended Keyboard II.
           *       The key is locked in the down position when pressed (activating Caps Lock), and goes back up when pressed again (turning off Caps Lock).
           *       For the host computer to recognize Caps Lock properly, it needs to behave like a normal key. It needs to be pressed and then released for each toggle.
           *       This section sends both a press and a release command for the caps lock key, as the key press in the keyboard is just registered as a key release.
           *       It also turns off the Caps Lock light in the keyboard.
           */
          Keyboard.write(KEY_CAPS_LOCK);
          caps_lock_light = 0;
          setLights();
        } else {
          /* Send a key release command with the key released */
          Keyboard.release(keymap[reg0 & 0x7F]);
        }
      }
    } else {
      #ifdef KEY_DEBUG
        Serial.print("Key Pressed (2nd Byte): 0x");
        Serial.print(reg0 & 0x7F, HEX);
        Serial.print(" (");
        Serial.print((keymap[reg0 & 0x7F] >= 32 && keymap[reg0 & 0x7F] <= 126) ? (char) keymap[reg0 & 0x7F] : '-');
        Serial.println(")");
      #endif

      /* Check in keymap if pressed key is mapped to anything */
      if (keymap[reg0 & 0x7F]) {
        /* Check if key pressed is Caps Lock */
        if (keymap[reg0 & 0x7F] == KEY_CAPS_LOCK) {
          /*
           * Handle Caps Lock press
           *
           * NOTE: Caps Lock behaves different from other keys in the Apple Extended Keyboard II.
           *       The key is locked in the down position when pressed (activating Caps Lock), and goes back up when pressed again (turning off Caps Lock).
           *       For the host computer to recognize Caps Lock properly, it needs to behave like a normal key. It needs to be pressed and then released for each toggle.
           *       This section sends both a press and a release command for the caps lock key, as the key press in the keyboard is just registered as a key press.
           *       It also turns on the Caps Lock light in the keyboard.
           */
          Keyboard.write(KEY_CAPS_LOCK);
          caps_lock_light = 1;
          setLights();
        } else {
          /* Send a key press command with the key pressed */
          Keyboard.press(keymap[reg0 & 0x7F]);
        }
      }
    }
  }

  /* Delay before polling bus again */
  delay(12);
}

/*
 *  Reset
 *  Resets all devices on the bus
 */
void reset(void) {
  DATA_LOW();
  _delay_ms(3);
  DATA_HIGH();
  delay(1000);
}

/*
 *  Zero
 *  Sends a zero to the bus
 */
void zero(void) {
  DATA_LOW();
  _delay_us(65);
  DATA_HIGH();
  _delay_us(35);
}

/*
 *  One
 *  Sends a one to the bus
 */
void one(void) {
  DATA_LOW();
  _delay_us(35);
  DATA_HIGH();
  _delay_us(65);
}

/*
 *  Attention and Sync
 *  Sends the attention signal to the bus and waits the sync time
 */
void attentionSync(void) {
  DATA_LOW();
  _delay_us(800);
  DATA_HIGH();
  _delay_us(70);
}

/*
 *  LISTEN
 *  Sends command for device to listen for data and store it in a desired register
 *  Parameters:
 *    - uint8_t address: Address of the device to send the LISTEN command. Range of valid addresses is 0 to 15
 *    - uint8_t reg: Register of the device to store the sent data. Range of valid registers is 0 to 3
 *    - uint16_t data: 16 bit data sent to the desired device to store
 */
void listen(uint8_t address, uint8_t reg, uint16_t data) {

  #ifdef COMM_DEBUG
    Serial.print("LISTEN to device with address ");
    Serial.print(address);
    Serial.print(" to register ");
    Serial.print(reg);
    Serial.print(". Data: 0x");
    Serial.println(data, HEX);
  #endif
  
  /* Send Attention and Sync to bus */
  attentionSync();

  /* Send Device Address to bus */
  for (int8_t idx = 3; idx >= 0; idx--) {
    if ((address >> idx) & 0x1)
      one();
    else
      zero();
  }

  /* Send LISTEN Command to bus */
  one();
  zero();

  /* Send Register Number to bus */
  for (int8_t idx = 1; idx >= 0; idx--) {
    if ((reg >> idx) & 0x1)
      one();
    else
      zero();
  }

  /* Send Stop bit to bus */
  zero();

  /* Delay for Tlt */
  _delay_us(200);

  /* Send Start bit to bus */
  one();

  /* Send Data to bus */
  for (int8_t idx = 15; idx >= 0; idx--) {
    if ((data >> idx) & 0x1)
      one();
    else
      zero();
  }

  /* Send Stop bit to bus */
  zero();
}

/*
 *  TALK
 *  Sends command for device to talk with the host with data from a desired register
 *  Parameters:
 *    - uint8_t address: Address of the device to send the TALK command. Range of valid addresses is 0 to 15
 *    - uint8_t reg: Register of the device to fetch the data from. Range of valid registers is 0 to 3
 *    - uint16_t *data: Pointer to a 16 bit location where the response from the device will be stored
 */
bool talk(uint8_t address, uint8_t reg, uint16_t *data) {

  #ifdef COMM_DEBUG
    Serial.print("TALK from device with address ");
    Serial.print(address);
    Serial.print(" from register ");
    Serial.println(reg);
  #endif

  /* Send Attention and Sync to bus */
  attentionSync();

  /* Send Device Address to bus */
  for (int8_t idx = 3; idx >= 0; idx--) {
    if ((address >> idx) & 0x1)
      one();
    else
      zero();
  }

  /* Send TALK Command to bus */
  one();
  one();

  /* Send Register Number to bus */
  for (int8_t idx = 1; idx >= 0; idx--) {
    if ((reg >> idx) & 0x1)
      one();
    else
      zero();
  }

  /* Send Stop bit to bus */
  zero();

  /* Define response */
  uint16_t response = 0;

  unsigned long startTime = micros();

  /* Wait until bus goes low */
  while (DATA_IN()) {
    if (micros() - startTime > 500) {
      /* Timeout occurred */
      return 0;
    }
  }

  /* Delay 70us to ignore first (starting) bit */
  _delay_us(70);

  /* Read 16 bit response */
  for (int i = 0; i < 16; i++) {
    while (DATA_IN());
    _delay_us(50);
    response <<= 1;
    response |= DATA_IN();
    _delay_us(20);
  }

  #ifdef COMM_DEBUG
    Serial.print("Response: 0x");
    Serial.println(response, HEX);
    Serial.println(response, BIN);
  #endif

  /* Set response to data pointer */
  *data = response;

  /* Return success */
  return 1;
}

/*
 *  setLights
 *  Sets keyboard lights based on global variables
 */
void setLights(void) {
  uint16_t data = 0xFFFF;

  if (num_lock_light)
    data &= 0xFFFE;

  if (caps_lock_light)
    data &= 0xFFFD;

  if (scroll_lock_light)
    data &= 0xFFFB;

  listen(2, 2, data);
}

/*
 * showStartAnimation
 * PLays animation to show that the keyboard is connected 
 */
void showStartAnimation(void) {
  /* 0 _ _ */
  num_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* _ 0 _ */
  num_lock_light = 0;
  caps_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* _ _ 0 */
  caps_lock_light = 0;
  scroll_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* _ 0 _ */
  scroll_lock_light = 0;
  caps_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* 0 _ _ */
  caps_lock_light = 0;
  num_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* _ _ _ */
  num_lock_light = 0;
  setLights();
  delay(ANIMATION_LENGTH);

  /* 0 0 0 */
  num_lock_light = 1;
  caps_lock_light = 1;
  scroll_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH);

  /* _ _ _ */
  num_lock_light = 0;
  caps_lock_light = 0;
  scroll_lock_light = 0;
  setLights();
  delay(ANIMATION_LENGTH);

  /* 0 0 0 */
  num_lock_light = 1;
  caps_lock_light = 1;
  scroll_lock_light = 1;
  setLights();
  delay(ANIMATION_LENGTH*5);

  /* _ _ _ */
  num_lock_light = 0;
  caps_lock_light = 0;
  scroll_lock_light = 0;
  setLights();
}
