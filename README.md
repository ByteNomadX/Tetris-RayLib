# Tetris Raylib

Simple Tetris game written in C using Raylib.

## Features

- Classic Tetris gameplay
- Written in C with modular code
- Uses Raylib for graphics and input

## Requirements

- GCC (or any compatible C compiler)
- Raylib library installed
- pkg-config utility

### Installing dependencies

Linux (Debian/Ubuntu)
``` bash
sudo apt update ;
sudo apt install build-essential git pkg-config libraylib-dev
```
macOS (Homebrew)
``` bash
brew update ;
brew install gcc raylib pkg-config
``` 
## Building

Clone the repository:
``` bash
git clone https://github.com/ByteNomadX/Tetris-RayLib.git ;
cd Tetris-RayLib
```

Build the game:
``` bash
make
```

Run the game:
``` bash
./tetris
```

## Cleaning

To remove object files:
``` bash
make clean
```

## How to play ?

- **Left Arrow**: Move tetromino left  
- **Right Arrow**: Move tetromino right  
- **Down Arrow**: Accelerate falling  
- **Up Arrow**: Rotate tetromino
- **M**: Pause/Resume music
- **S**: Enable/Disable sounds
- **P**: Pause game

## Notes

- Make sure pkg-config is installed. It automatically detects Raylib's include and library paths.
- The Makefile uses pkg-config to simplify compilation, so users don't need to manually specify library paths.
- Tested on Linux and macOS.

## Music

- Tetris Theme - Korobeiniki - Arranged for Piano by GregorQuendel -- https://freesound.org/s/719393/ -- License: Attribution NonCommercial 4.0
