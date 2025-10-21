# Gemini Project Context: Arduino RGB LED Controller

## Directory Overview

This directory contains a simple Arduino project. The main file is `sketch_oct21a_copy_20251021093536.ino`, which is an Arduino sketch.

## Project Overview

This project is an Arduino sketch that controls an RGB LED using a push button. The code is written in C/C++ for the Arduino platform.

- **Functionality:** Pressing the button cycles the RGB LED through three colors: red, green, and blue.
- **Hardware:** The sketch is designed for a board with an RGB LED connected to GPIO pins 32 (Red), 33 (Green), and 25 (Blue), and a push button on GPIO pin 4. This appears to be for an ESP32 board, given the pin numbers.

## Building and Running

1.  **IDE:** Open the `.ino` file with the [Arduino IDE](https://www.arduino.cc/en/software) or a compatible editor like PlatformIO.
2.  **Board:** Select the correct board (e.g., an ESP32 development board).
3.  **Upload:** Compile and upload the sketch to the microcontroller.

## Development Conventions

- The code follows standard Arduino C/C++ conventions.
- Pin definitions are managed using `#define` macros.
- The main logic is contained within the `setup()` and `loop()` functions.
- A simple state machine with a `switch` statement handles the color cycling.
- Basic debouncing for the button is implemented with a small `delay()`.
