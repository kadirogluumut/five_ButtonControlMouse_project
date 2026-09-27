# ESP32 BLE Mouse

A DIY Bluetooth Low Energy (BLE) mouse built with an ESP32, using push buttons for left click, right click, and cursor movement.

## Features

- Acts as a standard BLE HID mouse — pairs with any BLE-capable computer, phone, or tablet
- Left click and right click (menu) buttons
- Cursor movement via 4 direction buttons (up / down / left / right)
- Acceleration: the longer a direction button is held, the faster the cursor moves
- Software debouncing on click buttons for reliable clicks

## Hardware Requirements

- ESP32 development board (e.g. ESP32 Lite V1 / any ESP32 with BLE support)
- 6x momentary push buttons
- Jumper wires / breadboard

## Wiring

| Function          | ESP32 Pin |
|-------------------|-----------|
| Left click        | GPIO 27   |
| Right click / menu| GPIO 26   |
| Move up           | GPIO 33   |
| Move down         | GPIO 32   |
| Move left         | GPIO 25   |
| Move right        | GPIO 4    |

All buttons use the internal pull-up resistors (`INPUT_PULLUP`), so wire each button between its GPIO pin and **GND** — no external resistor needed.

## Dependencies

- [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse) library

Install it via the Arduino Library Manager, or manually:

1. Download the library as a `.zip` from the link above.
2. In Arduino IDE: `Sketch > Include Library > Add .ZIP Library...`
3. Select the downloaded file.

## Setup

1. Install the [ESP32 board package](https://github.com/espressif/arduino-esp32) in Arduino IDE if you haven't already.
2. Install the `ESP32-BLE-Mouse` library (see above).
3. Wire the buttons as described in the [Wiring](#wiring) section.
4. Open `esp32_ble_mouse_buttons.ino` in Arduino IDE.
5. Select your ESP32 board and port, then upload.
6. Open Serial Monitor at `115200` baud to see status messages (optional).
7. On your computer/phone, open Bluetooth settings and pair with **"ESP32 Mouse"**.

## Configuration

All tunable values are defined at the top of the sketch:

| Constant           | Default | Description                                              |
|---------------------|---------|------------------------------------------------------------|
| `DEBOUNCE_MS`        | 20      | Debounce time for click buttons (ms)                       |
| `MOVE_REPEAT_MS`     | 15      | How often a movement step is sent while a button is held (ms) |
| `MOVE_SPEED_MIN`     | 2       | Cursor speed right after a direction button is pressed (px/step) |
| `MOVE_SPEED_MAX`     | 14      | Maximum cursor speed after the button has been held for a while (px/step) |
| `ACCEL_RAMP_MS`      | 800     | Time to ramp from `MOVE_SPEED_MIN` to `MOVE_SPEED_MAX` (ms) |

## Usage

- **Left click**: press the left click button
- **Right click / open menu**: press the right click button
- **Move cursor**: press and hold a direction button; hold longer for faster movement
- **Diagonal movement**: press two adjacent direction buttons together (e.g. up + right)

## License

MIT — feel free to use, modify, and share.
