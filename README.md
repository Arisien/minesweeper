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

Tile is a basic class containing relevant information for a Minesweeper square using public instance variables.

* `int value` - The value of a tile goes from [0 - 9]. 9 means it is a bomb while the rest state how many adjacant bombs there are.

* `bool visible` - This boolean states whether the tile is visible to the user.

* `bool flagged` - This boolean states whether the tile has been flagged by the user.

### `class Field`

Field is a class containing all of the core functionality for a Minesweeper board, but lacking a I/O system making it not directly usable by users.

* `Field (int h, int w, int b)` - The constructor takes 3 parameters for the dimensions of the playing field (h & w), as well as the amount of mines (b). It will then generate the field, mines, and mark all tiles with their coresponding values.

* `int state` - This is state of the field, and is represented by integer enums. S_PLAYING means the field has not been completely cleared and flagged, S_WON means it has, and S_LOST means a mine has been triggered.

* `std::vector<std::vector<Tile>> tiles` - This is a 2D vector which stores all the tiles. It is public so it can be acessed directly.

* `void uncover(int x, int y)` - The uncover algorithm is a recursive function with is used for setting all adjacent tiles to visible when a 0 value tile is uncovered by the user.

* `void play (int x, int y, bool flag)` - This function lets you play a move on the field. X and Y are coordinates of tile you want to interact with, and flag is a bool which determines whether you want to flag it or uncover it.

### `class Game`

Game is a class containing a basic I/O system for interacting with the Field class. Instanciating a new Game object and running `game.start()` will create a playable Minesweeper game for the user in a command line interface. The functions of Game can be overriden to replace the UI or inputing methods for your own implementation.

* `virtual void start_screen ()` - This function is run when the game is start. You can override it to initialize your UI or create a start menu.

* `virtual void end_screen ()` - THis function is run when the game is finished. You can override when creating a custom UI.

* `virtual Input input ()` - This is a function that returns user input. The Input object contains an enum stating what type of input it is (I_QUIT, I_POSITION, I_FLAG), as well as 2 integer variables x and y (used by I_POSITION inputs).

* `virtual void render ()` - This function renders the UI whenever a move is played.

* `virtual void start ()` - This function is run to start the game. It contains the game loop, and can be overriden for a custom game loop which does not follow the same sequence.

## Todo

* `ms-cli.cpp` - Full fledged Minesweeper terminal game (colors, menu, options, centered UI, unicode, scores, seeds, save games)
* `ms-gui.cpp` - Minesweeper implementation with GUI

## Contributors

* [Arisien](https://github.com/Arisien) - Main developer

## License
Minesweeper is licensed under the [MIT](LICENSE) license. Feel free to fork the repository or modify the code as you see fit.
