# Minesweeper++

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Minesweeper++ is a library for making implementations of the classic game Minesweeper, as well as a group of various implementations using the header.

## Usage

*Usage depends on each Minesweeper implementation*

### `ms-simple.cpp`

To run Minesweeper simple, you need to pass command line arguments for field height and width as well as amount of mines. Playing on a 6*6 field with 12 mines will look something like this:

* `ms-simple 6 6 12`

The game is played by writing commands to the console. The following are supported commands:

* `plot x y` - This will either place a flag or try to clear the selected tile.
* `flag` - This will toggle flag mode, which dictates whether a flag is placed/removed or the tile is revealed.
* `quit` - This will quit the program and result in game over

## Building

*Note: a C++ compiler is required to compile the programs. Alternatively download the binaries from releases*

* Compile a Minesweeper implementation with `g++ ms-[name].cpp`
* Run the binary with `./a.out`

## Documentation

*If you would like to develop your own implementation of Minesweeper, check out the `minesweeper.hpp` header file*

### `class Tile`

### `class Field`

### `class Game`

## Todo

* finish doc
* int remaining - store amount of tiles left to flag/uncover so the program does not need to iterate through entire vector each time to check if the game is finished.
* `ms-cli.cpp` - Full fledged Minesweeper terminal game (colors, menu, options, centered UI, unicode, scores, seeds, save games)
* `ms-gui.cpp` - Minesweeper implementation with GUI

## Contributors

* [Arisien](https://github.com/Arisien) - Main developer

## License
Minesweeper is licensed under the [MIT](LICENSE) license. Feel free to fork the repository or modify the code as you see fit.
