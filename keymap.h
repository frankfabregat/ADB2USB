/*
 * keymap.h
 * By Francisco Fabregat
 * June 22, 2024
 *
 * This file contains an array of 128 bytes, each corresponding to a different key.
 * The Apple Extended Keyboard II will send a 7-bit identifier for the key pressed/released. 2^7=128
 * The 7-bit identifier acts as the index in the array, and the value at that index is the letter or key mapped to that identifier.
 * There are some identifiers which I was unable to find what they are usually mapped to (the ones commented with UNKNOWN), and they have a value of 0.
 *
 * NOTE: This keymap is for the English ANSI keyboard layout. The keymap could potentially be different for other layouts and localities.
 */

uint8_t keymap[128] = {
    'a',   // 0x00, A
    's',   // 0x01, S
    'd',   // 0x02, D
    'f',   // 0x03, F
    'h',   // 0x04, H
    'g',   // 0x05, G
    'z',   // 0x06, Z
    'x',   // 0x07, X
    'c',   // 0x08, C
    'v',   // 0x09, V
    0,     // 0x0A, UNKNOWN
    'b',   // 0x0B, B
    'q',   // 0x0C, Q
    'w',   // 0x0D, W
    'e',   // 0x0E, E
    'r',   // 0x0F, R
    'y',   // 0x10, Y
    't',   // 0x11, T
    '1',   // 0x12, 1
    '2',   // 0x13, 2
    '3',   // 0x14, 3
    '4',   // 0x15, 4
    '6',   // 0x16, 6
    '5',   // 0x17, 5
    '=',   // 0x18, = and +
    '9',   // 0x19, 9
    '7',   // 0x1A, 7
    '-',   // 0x1B, - and _
    '8',   // 0x1C, 8
    '0',   // 0x1D, 0
    ']',   // 0x1E, } amd ]
    'o',   // 0x1F, O
    'u',   // 0x20, U
    '[',   // 0x21, { and [
    'i',   // 0x22, I
    'p',   // 0x23, P
    KEY_RETURN, // 0x24, Enter/Return
    'l',   // 0x25, L
    'j',   // 0x26, J
    0x27,    // 0x27, '
    'k',   // 0x28, K
    ';',   // 0x29, ; and :
    '\\',   // 0x2A, \ and |
    ',',   // 0x2B, , and <
    '/',   // 0x2C, / and ?
    'n',   // 0x2D, N
    'm',   // 0x2E, M
    '.',   // 0x2F, . and >
    KEY_TAB, // 0x30, Tab key
    ' ',   // 0x31, Space key
    '`',   // 0x32, ` and ~
    KEY_BACKSPACE, // 0x33, Backspace key
    0,     // 0x34, 0
    KEY_ESC, //0x35, Escape key
    KEY_LEFT_CTRL, // 0x36, Left control key
    KEY_LEFT_GUI,  // 0x37, Command key
    KEY_LEFT_SHIFT,  // 0x38, Left shift key
    KEY_CAPS_LOCK, // 0x39, Caps lock key
    KEY_LEFT_ALT,  // 0x3A, Left alt key
    KEY_LEFT_ARROW,  // 0x3B, Left arrow key
    KEY_RIGHT_ARROW, // 0x3C, Right arrow key
    KEY_DOWN_ARROW,  // 0x3D, Down arrow key
    KEY_UP_ARROW,    // 0x3E, Up arrow key
    0,     // 0x3F, UNKNOWN
    0,     // 0x40, UNKNOWN
    KEY_KP_DOT,  // 0x41, Keypad .
    0,     // 0x42, UNKNOWN
    KEY_KP_ASTERISK, // 0x43, Keypad *
    0,     // 0x44, UNKNOWN
    KEY_KP_PLUS, // 0x45, Keypad +
    0,     // 0x46, UNKNOWN
    KEY_NUM_LOCK,  // 0x47, Num Lock key
    0,     // 0x48, UNKNOWN
    0,     // 0x49, UNKNOWN
    0,     // 0x4A, UNKNOWN
    KEY_KP_SLASH,  // 0x4B, Keypad /
    KEY_KP_ENTER,  // 0x4C, Keypad Enter
    0,     // 0x4D, UNKNOWN
    KEY_KP_MINUS,  // 0x4E, Keypad -
    0,     // 0x4F, UNKNOWN
    0,     // 0x50, UNKNOWN
    '=',     // 0x51, Keypad =
    KEY_KP_0,  // 0x52, Keypad 0
    KEY_KP_1,  // 0x53, Keypad 1
    KEY_KP_2,  // 0x54, Keypad 2
    KEY_KP_3,  // 0x55, Keypad 3
    KEY_KP_4,  // 0x56, Keypad 4
    KEY_KP_5,  // 0x57, Keypad 5
    KEY_KP_6,  // 0x58, Keypad 6
    KEY_KP_7,  // 0x59, Keypad 7
    0,         // 0x5A, UNKNOWN 
    KEY_KP_8,  // 0x5B, Keypad 8
    KEY_KP_9,  // 0x5C, Keypad 9
    0,         // 0x5D, UNKNOWN
    0,         // 0x5E, UNKNOWN
    0,         // 0x5F, UNKNOWN
    KEY_F5,    // 0x60, F5
    KEY_F6,    // 0x61, F6
    KEY_F7,    // 0x62, F7
    KEY_F3,    // 0x63, F3
    KEY_F8,    // 0x64, F8
    KEY_F9,    // 0x65, F9
    0,         // 0x66, UNKNOWN
    KEY_F11,   // 0x67, F11
    0,         // 0x68, UNKNOWN
    KEY_F13,   // 0x69, F13
    0,         // 0x6A, UNKNOWN
    KEY_F14,   // 0x6B, F14
    0,         // 0x6C, UNKNOWN
    KEY_F10,   // 0x6D, F10
    0,         // 0x6E, UNKNOWN
    KEY_F12,   // 0x6F, F12
    0,         // 0x70, UNKNOWN
    KEY_F15,   // 0x71, F15
    KEY_INSERT,  // 0x72, Insert key
    KEY_HOME,    // 0x73, Home key
    KEY_PAGE_UP, // 0x74, Page up key
    KEY_DELETE,  // 0x75, Delete key
    KEY_F4,    // 0x76, F4
    KEY_END,   // 0x77, End key
    KEY_F2,    // 0x78, F2
    KEY_PAGE_DOWN, // 0x79, Page down key
    KEY_F1,    // 0x7A, F1
    KEY_RIGHT_SHIFT, // 0x7B, Right shift
    KEY_RIGHT_ALT, // 0x7C, Right alt key
    KEY_RIGHT_CTRL,  // 0x7D, Right control key
    0, // 0x7E, UNKNOWN
    0, // 0x7F, Power button
};