/*
 * keys.h
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#ifndef INC_KEYS_H_
#define INC_KEYS_H_

#include "stm32g0xx_hal.h"
#include "tx_api.h"
#include "ux_api.h"
#include "ux_device_class_hid.h"
#include "stdbool.h"
#include "config.h"
#include "tim.h"

#define MAX_CONCURRENT_KEYS 6 // (UX_DEVICE_CLASS_HID_MAX_EVENTS_QUEUE - 2)

typedef enum {
	KEY_MOD_NONE =   0x00,
	KEY_MOD_LCTRL =  0x01,
	KEY_MOD_LSHIFT = 0x02,
	KEY_MOD_LALT =   0x04,
	KEY_MOD_LMETA =  0x08,
	KEY_MOD_RCTRL =  0x10,
	KEY_MOD_RSHIFT = 0x20,
	KEY_MOD_RALT =   0x40,
	KEY_MOD_RMETA =  0x80,
	KEY_MOD_LAYER_CHANGE_MOMENTARY = 0xf0,
	KEY_MOD_LAYER_CHANGE_TOGGLE = 0xf1
} keyMod_TypeDef;

typedef enum {
	KEY_NAME_NONE = 0x00, // No key pressed
	KEY_NAME_ERR_OVF = 0x01, // Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
	KEY_NAME_POST_FAIL = 0x02, //  Keyboard POST Fail
	KEY_NAME_ERROR = 0x03, //  Keyboard Error Undefined

	KEY_NAME_A = 0x04,
	KEY_NAME_B = 0x05,
	KEY_NAME_C = 0x06,
	KEY_NAME_D = 0x07,
	KEY_NAME_E = 0x08,
	KEY_NAME_F = 0x09,
	KEY_NAME_G = 0x0a,
	KEY_NAME_H = 0x0b,
	KEY_NAME_I = 0x0c,
	KEY_NAME_J = 0x0d,
	KEY_NAME_K = 0x0e,
	KEY_NAME_L = 0x0f,
	KEY_NAME_M = 0x10,
	KEY_NAME_N = 0x11,
	KEY_NAME_O = 0x12,
	KEY_NAME_P = 0x13,
	KEY_NAME_Q = 0x14,
	KEY_NAME_R = 0x15,
	KEY_NAME_S = 0x16,
	KEY_NAME_T = 0x17,
	KEY_NAME_U = 0x18,
	KEY_NAME_V = 0x19,
	KEY_NAME_W = 0x1a,
	KEY_NAME_X = 0x1b,
	KEY_NAME_Y = 0x1c,
	KEY_NAME_Z = 0x1d,

	KEY_NAME_1 = 0x1e,
	KEY_NAME_2 = 0x1f,
	KEY_NAME_3 = 0x20,
	KEY_NAME_4 = 0x21,
	KEY_NAME_5 = 0x22,
	KEY_NAME_6 = 0x23,
	KEY_NAME_7 = 0x24,
	KEY_NAME_8 = 0x25,
	KEY_NAME_9 = 0x26,
	KEY_NAME_0 = 0x27,

	KEY_NAME_ENTER = 0x28, // Keyboard Return (ENTER)
	KEY_NAME_ESC = 0x29, // Keyboard ESCAPE
	KEY_NAME_BACKSPACE = 0x2a, // Keyboard DELETE (Backspace)
	KEY_NAME_TAB = 0x2b, // Keyboard Tab
	KEY_NAME_SPACE = 0x2c, // Keyboard Spacebar
	KEY_NAME_MINUS = 0x2d, // Keyboard - and _
	KEY_NAME_EQUAL = 0x2e, // Keyboard = and +
	KEY_NAME_LEFTBRACE = 0x2f, // Keyboard [ and {
	KEY_NAME_RIGHTBRACE = 0x30, // Keyboard ] and }
	KEY_NAME_BACKSLASH = 0x31, // Keyboard \ and |
	KEY_NAME_HASHTILDE = 0x32, // Keyboard Non-US # and ~
	KEY_NAME_SEMICOLON = 0x33, // Keyboard ; and :
	KEY_NAME_APOSTROPHE = 0x34, // Keyboard ' and "
	KEY_NAME_GRAVE = 0x35, // Keyboard ` and ~
	KEY_NAME_COMMA = 0x36, // Keyboard , and <
	KEY_NAME_DOT = 0x37, // Keyboard . and >
	KEY_NAME_SLASH = 0x38, // Keyboard / and ?
	KEY_NAME_CAPSLOCK = 0x39, // Keyboard Caps Lock

	KEY_NAME_F1 = 0x3a, // Keyboard F1
	KEY_NAME_F2 = 0x3b, // Keyboard F2
	KEY_NAME_F3 = 0x3c, // Keyboard F3
	KEY_NAME_F4 = 0x3d, // Keyboard F4
	KEY_NAME_F5 = 0x3e, // Keyboard F5
	KEY_NAME_F6 = 0x3f, // Keyboard F6
	KEY_NAME_F7 = 0x40, // Keyboard F7
	KEY_NAME_F8 = 0x41, // Keyboard F8
	KEY_NAME_F9 = 0x42, // Keyboard F9
	KEY_NAME_F10 = 0x43, // Keyboard F10
	KEY_NAME_F11 = 0x44, // Keyboard F11
	KEY_NAME_F12 = 0x45, // Keyboard F12

	KEY_NAME_SYSRQ = 0x46, // Keyboard Print Screen
	KEY_NAME_SCROLLLOCK = 0x47, // Keyboard Scroll Lock
	KEY_NAME_PAUSE = 0x48, // Keyboard Pause
	KEY_NAME_INSERT = 0x49, // Keyboard Insert
	KEY_NAME_HOME = 0x4a, // Keyboard Home
	KEY_NAME_PAGEUP = 0x4b, // Keyboard Page Up
	KEY_NAME_DELETE = 0x4c, // Keyboard Delete Forward
	KEY_NAME_END = 0x4d, // Keyboard End
	KEY_NAME_PAGEDOWN = 0x4e, // Keyboard Page Down
	KEY_NAME_RIGHT = 0x4f, // Keyboard Right Arrow
	KEY_NAME_LEFT = 0x50, // Keyboard Left Arrow
	KEY_NAME_DOWN = 0x51, // Keyboard Down Arrow
	KEY_NAME_UP = 0x52, // Keyboard Up Arrow

	KEY_NAME_NUMLOCK = 0x53, // Keyboard Num Lock and Clear
	KEY_NAME_KPSLASH = 0x54, // Keypad /
	KEY_NAME_KPASTERISK = 0x55, // Keypad *
	KEY_NAME_KPMINUS = 0x56, // Keypad -
	KEY_NAME_KPPLUS = 0x57, // Keypad +
	KEY_NAME_KPENTER = 0x58, // Keypad ENTER
	KEY_NAME_KP1 = 0x59, // Keypad 1 and End
	KEY_NAME_KP2 = 0x5a, // Keypad 2 and Down Arrow
	KEY_NAME_KP3 = 0x5b, // Keypad 3 and PageDn
	KEY_NAME_KP4 = 0x5c, // Keypad 4 and Left Arrow
	KEY_NAME_KP5 = 0x5d, // Keypad 5
	KEY_NAME_KP6 = 0x5e, // Keypad 6 and Right Arrow
	KEY_NAME_KP7 = 0x5f, // Keypad 7 and Home
	KEY_NAME_KP8 = 0x60, // Keypad 8 and Up Arrow
	KEY_NAME_KP9 = 0x61, // Keypad 9 and Page Up
	KEY_NAME_KP0 = 0x62, // Keypad 0 and Insert
	KEY_NAME_KPDOT = 0x63, // Keypad . and Delete

	KEY_NAME_BACKSLASH_EN = 0x64, // Keyboard Non-US \ and |
	KEY_NAME_COMPOSE = 0x65, // Keyboard Application
	KEY_NAME_POWER = 0x66, // Keyboard Power
	KEY_NAME_KPEQUAL = 0x67, // Keypad =

	KEY_NAME_F13 = 0x68, // Keyboard F13
	KEY_NAME_F14 = 0x69, // Keyboard F14
	KEY_NAME_F15 = 0x6a, // Keyboard F15
	KEY_NAME_F16 = 0x6b, // Keyboard F16
	KEY_NAME_F17 = 0x6c, // Keyboard F17
	KEY_NAME_F18 = 0x6d, // Keyboard F18
	KEY_NAME_F19 = 0x6e, // Keyboard F19
	KEY_NAME_F20 = 0x6f, // Keyboard F20
	KEY_NAME_F21 = 0x70, // Keyboard F21
	KEY_NAME_F22 = 0x71, // Keyboard F22
	KEY_NAME_F23 = 0x72, // Keyboard F23
	KEY_NAME_F24 = 0x73, // Keyboard F24

	KEY_NAME_EXECUTE = 0x74, // Keyboard Execute
	KEY_NAME_HELP = 0x75, // Keyboard Help
	KEY_NAME_PROPS = 0x76, // Keyboard Menu
	KEY_NAME_FRONT = 0x77, // Keyboard Select
	KEY_NAME_STOP = 0x78, // Keyboard Stop
	KEY_NAME_AGAIN = 0x79, // Keyboard Again
	KEY_NAME_UNDO = 0x7a, // Keyboard Undo
	KEY_NAME_CUT = 0x7b, // Keyboard Cut
	KEY_NAME_COPY = 0x7c, // Keyboard Copy
	KEY_NAME_PASTE = 0x7d, // Keyboard Paste
	KEY_NAME_FIND = 0x7e, // Keyboard Find
	KEY_NAME_MUTE = 0x7f, // Keyboard Mute
	KEY_NAME_VOLUMEUP = 0x80, // Keyboard Volume Up
	KEY_NAME_VOLUMEDOWN = 0x81, // Keyboard Volume Down
	KEY_NAME_LOCKING_CAPS_LOCK = 0x82, //  Keyboard Locking Caps Lock
	KEY_NAME_LOCKING_NUM_LOCK = 0x83, //  Keyboard Locking Num Lock
	KEY_NAME_LOCKING_SCROLL_LOCK = 0x84, //  Keyboard Locking Scroll Lock
	KEY_NAME_KP_COMMA = 0x85,	// KNAME_eypad = Comma,
	KEY_NAME_KP_EQUALS = 0x86, //  Keypad Equal Sign
	KEY_NAME_RO = 0x87, // Keyboard International1
	KEY_NAME_KATAKANAHIRAGANA = 0x88, // Keyboard International2
	KEY_NAME_YEN = 0x89, // Keyboard International3
	KEY_NAME_HENKAN = 0x8a, // Keyboard International4
	KEY_NAME_MUHENKAN = 0x8b, // Keyboard International5
	KEY_NAME_KPJPCOMMA = 0x8c, // Keyboard International6
	KEY_NAME_INTERNATIONAL_7 = 0x8d, //  Keyboard International7
	KEY_NAME_INTERNATIONAL_8 = 0x8e, //  Keyboard International8
	KEY_NAME_INTERNATIONAL_9 = 0x8f, //  Keyboard International9
	KEY_NAME_HANGEUL = 0x90, // Keyboard LANG1
	KEY_NAME_HANJA = 0x91, // Keyboard LANG2
	KEY_NAME_KATAKANA = 0x92, // Keyboard LANG3
	KEY_NAME_HIRAGANA = 0x93, // Keyboard LANG4
	KEY_NAME_ZENKAKUHANKAKU = 0x94, // Keyboard LANG5
	KEY_NAME_LANG6 = 0x95, // Keyboard LANG6
	KEY_NAME_LANG7 = 0x96, // Keyboard LANG7
	KEY_NAME_LANG8 = 0x97, // Keyboard LANG8
	KEY_NAME_LANG9 = 0x98, // Keyboard LANG9
	KEY_NAME_ALT_ERASE = 0x99, // Keyboard Alternate Erase
	KEY_NAME_SYS_REQ = 0x9a, // Keyboard SysReq/Attention
	KEY_NAME_CANCEL = 0x9b, // Keyboard Cancel
	KEY_NAME_CLEAR = 0x9c, // Keyboard Clear
	KEY_NAME_PRIOR = 0x9d, // Keyboard Prior
	KEY_NAME_RETURN = 0x9e, // Keyboard Return
	KEY_NAME_SEPARATOR = 0x9f, // Keyboard Separator
	KEY_NAME_OUT = 0xa0, // Keyboard Out
	KEY_NAME_OPEN = 0xa1, // Keyboard Oper
	KEY_NAME_CLEAR_AGAIN = 0xa2, // Keyboard Clear/Again
	KEY_NAME_CR_SEL = 0xa3, // Keyboard CrSel/Props
	KEY_NAME_EX_SEL = 0xa4, // Keyboard ExSel

	KEY_NAME_00 = 0xb0, // Keypad 00
	KEY_NAME_000 = 0xb1, // Keypad 000
	KEY_NAME_THOUSANDS_SEPARATOR = 0xb2, // Thousands Separator
	KEY_NAME_DECIMAL_SEPARATOR = 0xb3, // Decimal Separator
	KEY_NAME_CURRENCY_UNIT = 0xb4, // Currency Unit
	KEY_NAME_CURRENCY_SUB_UNIT = 0xb5, // Currency Sub-unit
	KEY_NAME_KPLEFTPAREN = 0xb6, // Keypad (
	KEY_NAME_KPRIGHTPAREN = 0xb7, // Keypad )
	KEY_NAME_SQUIGGLE_OPEN = 0xb8, // Keypad {
	KEY_NAME_SQUIGGLE_CLOSE = 0xb9, // Keypad }
	KEY_NAME_KP_TAB = 0xba, // Keypad Tab
	KEY_NAME_KP_BACKSPACE = 0xbb, // Keypad Backspace
	KEY_NAME_KP_A = 0xbc, // Keypad A
	KEY_NAME_KP_B = 0xbd, // Keypad B
	KEY_NAME_KP_C = 0xbe, // Keypad C
	KEY_NAME_KP_D = 0xbf, // Keypad D
	KEY_NAME_KP_E = 0xc0, // Keypad E
	KEY_NAME_KP_F = 0xc1, // Keypad F
	KEY_NAME_KP_XOR = 0xc2, // Keypad XOR
	KEY_NAME_KP_HAT = 0xc3, // Keypad ^
	KEY_NAME_KP_PERCENT = 0xc4, // Keypad %
	KEY_NAME_KP_LESS_THAN = 0xc5, // Keypad <
	KEY_NAME_KP_GREATER_THAN = 0xc6, // Keypad >
	KEY_NAME_KP_AND = 0xc7, // Keypad &
	KEY_NAME_KP_AND_AND = 0xc8, // Keypad &&
	KEY_NAME_KP_PIPE = 0xc9, // Keypad |
	KEY_NAME_KP_PIPE_PIPE = 0xca, // Keypad ||
	KEY_NAME_KP_COLON = 0xcb, // Keypad :
	KEY_NAME_KP_HASH = 0xcc, // Keypad #
	KEY_NAME_KP_SPACE = 0xcd, // Keypad Space
	KEY_NAME_KP_AT = 0xce, // Keypad @
	KEY_NAME_KP_BANG = 0xcf, // Keypad !
	KEY_NAME_KP_MEMORY_STORE = 0xd0, // Keypad Memory Store
	KEY_NAME_KP_MEMORY_RECALL = 0xd1, // Keypad Memory Recall
	KEY_NAME_KP_MEMORY_CLEAR = 0xd2, // Keypad Memory Clear
	KEY_NAME_KP_MEMORY_ADD = 0xd3, // Keypad Memory Add
	KEY_NAME_KP_MEMORY_SUB = 0xd4, // Keypad Memory Subtract
	KEY_NAME_KP_MEMORY_MULT = 0xd5, // Keypad Memory Multiply
	KEY_NAME_KP_MEMORY_DIV = 0xd6, // Keypad Memory Divide
	KEY_NAME_KP_PLUS_MINUS = 0xd7, // Keypad +/-
	KEY_NAME_KP_CLEAR = 0xd8, // Keypad Clear
	KEY_NAME_KP_ENTRY = 0xd9, // Keypad Clear Entry
	KEY_NAME_KP_BIN = 0xda, // Keypad Binary
	KEY_NAME_KP_OCT = 0xdb, // Keypad Octal
	KEY_NAME_KP_DEC = 0xdc, // Keypad Decimal
	KEY_NAME_KP_HEX = 0xdd, // Keypad Hexadecimal

	KEY_NAME_LEFTCTRL = 0xe0, // Keyboard Left Control
	KEY_NAME_LEFTSHIFT = 0xe1, // Keyboard Left Shift
	KEY_NAME_LEFTALT = 0xe2, // Keyboard Left Alt
	KEY_NAME_LEFTMETA = 0xe3, // Keyboard Left GUI
	KEY_NAME_RIGHTCTRL = 0xe4, // Keyboard Right Control
	KEY_NAME_RIGHTSHIFT = 0xe5, // Keyboard Right Shift
	KEY_NAME_RIGHTALT = 0xe6, // Keyboard Right Alt
	KEY_NAME_RIGHTMETA = 0xe7, // Keyboard Right GUI

	KEY_NAME_MEDIA_PLAYPAUSE = 0xe8,
	KEY_NAME_MEDIA_STOPCD = 0xe9,
	KEY_NAME_MEDIA_PREVIOUSSONG = 0xea,
	KEY_NAME_MEDIA_NEXTSONG = 0xeb,
	KEY_NAME_MEDIA_EJECTCD = 0xec,
	KEY_NAME_MEDIA_VOLUMEUP = 0xed,
	KEY_NAME_MEDIA_VOLUMEDOWN = 0xee,
	KEY_NAME_MEDIA_MUTE = 0xef,
	KEY_NAME_MEDIA_WWW = 0xf0,
	KEY_NAME_MEDIA_BACK = 0xf1,
	KEY_NAME_MEDIA_FORWARD = 0xf2,
	KEY_NAME_MEDIA_STOP = 0xf3,
	KEY_NAME_MEDIA_FIND = 0xf4,
	KEY_NAME_MEDIA_SCROLLUP = 0xf5,
	KEY_NAME_MEDIA_SCROLLDOWN = 0xf6,
	KEY_NAME_MEDIA_EDIT = 0xf7,
	KEY_NAME_MEDIA_SLEEP = 0xf8,
	KEY_NAME_MEDIA_COFFEE = 0xf9,
	KEY_NAME_MEDIA_REFRESH = 0xfa,
	KEY_NAME_MEDIA_CALC = 0xfb,
} keyName_TypeDef;

typedef enum {
	KEY_RELEASED = 0x00,
	KEY_PRESSED  = 0x01,
	KEY_TAP		 = 0x02
} keyState_TypeDef;

typedef struct {
	keyName_TypeDef key;
	keyState_TypeDef state;
} keyEvent_TypeDef;

typedef enum {
	KEY_LAYER_0 = 0x00,
	KEY_LAYER_1 = 0x01,
	KEY_LAYER_2 = 0x02,
	KEY_LAYER_3 = 0x03,
	KEY_LAYER_4 = 0x04,
	KEY_LAYER_5 = 0x05,
	KEY_LAYER_6 = 0x06,
	KEY_LAYER_7 = 0x07,
	KEY_LAYER_8 = 0x08,
	KEY_LAYER_9 = 0x09,
	KEY_LAYER_NEXT = 0xf0,
	KEY_LAYER_PREV = 0xf1
} keyLayerChange_TypeDef;

typedef struct {
	keyName_TypeDef key_name;
	keyMod_TypeDef mod_key;
	keyLayerChange_TypeDef layer_change; /* Layer to change to if key_mod == KEY_MOD_LAYER_CHANGE */
	uint32_t mod_delay; /* Amount of time the key needs to be held down before the mod key triggers in ms. Used for home row mods. */
} keyLayer_TypeDef;

typedef struct {

	uint16_t pin;
	GPIO_TypeDef *port;
	keyState_TypeDef current_state, previous_state; /* Used to determine presses and releases */
	uint32_t press_timestamp;
	bool holding_mod_key;

	uint8_t current_layer;
	keyLayer_TypeDef layers[NUMBER_OF_LAYERS];

	TX_QUEUE *event_queue;

} key_HandleTypeDef;


void key_init(key_HandleTypeDef *key);
void key_poll(key_HandleTypeDef *key);





typedef struct {
	keyName_TypeDef up_key_name;
	keyName_TypeDef down_key_name;
} knobLayer_TypeDef;

typedef struct {

	TIM_HandleTypeDef *htim;
	int16_t current_count, previous_count;

	uint8_t current_layer;
	knobLayer_TypeDef layers[NUMBER_OF_LAYERS];

	TX_QUEUE *event_queue;

} knob_HandleTypeDef;

void knob_init(knob_HandleTypeDef *knob, TIM_HandleTypeDef *htim, TX_QUEUE *event_queue);
void knob_poll(knob_HandleTypeDef *knob);





typedef enum {
	KEYBOARD_HID_OK              = 0x00U,
	KEYBOARD_HID_ERROR           = 0x01U,
	KEYBOARD_HID_BUSY            = 0x02U,
	KEYBOARD_HID_TIMEOUT         = 0x03U,
	KEYBOARD_HID_QUEUE_EMPTY     = 0x04U,
	KEYBOARD_HID_NONE_TAPPED     = 0x05U,
	KEYBOARD_HID_FOUND_TAPPED    = 0x06U,
	KEYBOARD_HID_ALREADY_PRESSED = 0x07U,
	KEYBOARD_HID_KEY_NOT_FOUND   = 0x08U,
	KEYBOARD_HID_LIST_FULL       = 0x09U
} keyboardHID_StatusTypeDef;

/* Linked list node structure. For keeping track of which keys are pressed and which need to be released */
typedef struct KeyNode {
    keyName_TypeDef key;
    bool tap;
    uint8_t taps;
    struct KeyNode* next;
} keyNode_TypeDef;

typedef struct {
	bool initialised;
	bool send_immediately; /* When a key is tapped multiple times, send another report immediately to pulse the key */
	UX_SLAVE_CLASS_HID *hid_class;
	UX_SLAVE_CLASS_HID_EVENT hid_event;
	uint8_t num_keys_pressed;
	keyNode_TypeDef *keys_pressed;
	TX_QUEUE *event_queue;
	keyEvent_TypeDef key_event;
} keyboardHID_HandleTypeDef;

keyboardHID_StatusTypeDef keyboardHID_init(keyboardHID_HandleTypeDef *keyboard, TX_QUEUE *event_queue, UX_SLAVE_CLASS_HID *hid_class);
keyboardHID_StatusTypeDef keyboardHID_process(keyboardHID_HandleTypeDef *keyboard);

#endif /* INC_KEYS_H_ */
