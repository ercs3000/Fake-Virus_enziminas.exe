# Fake-Virus_enziminas.exe
A fake lightweight virus that does no damage.
This project demonstrates the use of GDI+ in C++ to create various visual effects on the screen, such as color tints, sepia filters, glitches, disco effects, deep fry effects, flickering, screen shaking, and more. The application runs these effects in a random order to create a chaotic visual experience.


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
     g++ -std=c++17 -O2 -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections -fexceptions enziminas.cpp -lgdiplus -static-libgcc -static-libstdc++ -lgdi32 -mwindows -o enziminas.exe
     ```

## How It Works

The program uses GDI+ to manipulate the screen's contents by creating various visual effects. The effects are applied randomly in a loop, and the screen is updated with new effects every 25 milliseconds.

## Notes

- This project is intended for educational and experimental purposes. The effects are designed to create a chaotic visual experience and are not optimized for performance.
- **DO NOT USE IT ON YOUR BOSS OR PROFESSOR, I AM NOT RESPONSIBLE FOR ANY JOB LOSS.**

