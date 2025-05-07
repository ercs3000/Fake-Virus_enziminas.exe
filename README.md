# Fake-Virus_enziminas.exe
A fake lightweight virus that does no damage.
This project demonstrates the use of GDI+ in C++ to create various visual effects on the screen, such as color tints, sepia filters, glitches, disco effects, deep fry effects, flickering, screen shaking, and more. The application runs these effects in a random order to create a chaotic visual experience.

## Features

- **Tint Effect**: Applies a semi-transparent color overlay over the screen.
- **Sepia Effect**: Applies a warm, vintage sepia tone to the screen.
- **Glitch Effect**: Randomly draws colorful rectangles on the screen to simulate a glitchy appearance.
- **Disco Effect**: Applies a fast hue shift across the screen, creating a disco-like color transition effect.
- **Deep Fry Effect**: Increases contrast with intense colors to create an over-saturated, extreme visual effect.
- **Flicker Effect**: Randomly flashes the screen between black and white to simulate a flickering effect.
- **Screen Shake Effect**: Simulates a screen vibration by shifting the screen's content randomly.
- **Move Screen Effect**: Shifts the pixels of the entire screen in a random direction (up, down, left, or right), creating a moving screen effect.

## Requirements

- **Windows OS** (due to GDI+ usage).
- **TDM-GCC or MinGW** (GCC compiler for C++).
- **GDI+ Library**: Used for drawing graphics on the screen.

## Compilation and Build Instructions

1. **Install TDM-GCC or MinGW** (if not already installed):
   - You can download TDM-GCC from here: (https://jmeubank.github.io/tdm-gcc/).
   - Or you can download MinGW from here: (https://osdn.net/projects/mingw/releases/).

2. **Install GDI+ Library**:
   - The GDI+ library comes pre-installed with most versions of Windows, so no additional installation is typically required.

3. **Compile the Program**:
   - Open the Command Prompt (CMD) and navigate to the folder containing the source file.
   - Run the following command to compile the C++ file:
     ```bash
     g++ enziminas.cpp -lgdi32 -lmsimg32 -lgdiplus -o enziminas.exe
     ```

## How It Works

The program uses GDI+ to manipulate the screen's contents by creating various visual effects. The effects are applied randomly in a loop, and the screen is updated with new effects every 50 milliseconds.

### Effects Breakdown:

- **Tint**: A color overlay that gives the screen a tinted appearance.
- **Sepia**: Applies a sepia tone, similar to old photographs.
- **Glitches**: Random rectangles with various colors simulate a glitchy appearance.
- **Disco**: The screen color is continuously shifted in hue for a disco effect.
- **Deep Fry**: A contrast-heavy effect that makes the screen appear over-saturated and harsh.
- **Flicker**: The screen flashes black and white to simulate flickering.
- **Screen Shake**: The screen shifts randomly, simulating a "vibration" effect.
- **Move Screen**: The screen shifts its content randomly.

## Notes

- This project is intended for educational and experimental purposes. The effects are designed to create a chaotic visual experience and are not optimized for performance.
- **DO NOT USE IT ON YOUR BOSS OR PROFESSOR, I AM NOT OBLIGATED FOR ANY JOB LOSS.**

