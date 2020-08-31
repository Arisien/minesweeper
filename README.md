# Minesweeper++

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Minesweeper++ is a C++ implementation of Minesweeper. Currently, it has a CLI interface but that might change in the future. If you would like to contribute feel free to submit a pull request.

## Todo

* Improve the menu (options selected with arrow keys)
* Make the board customizable (custom width, height, bomb amount)
* Ability to enter custom seed (not random)
* Implement unicode character support (eg. 🏳 for flag)
* Implement arrow keys movement instead of plotting coordinates (maybe keep both as options)
* Redo UI to be more usable
* Remove the `rlutil.h` dependency, rewrite the functionality it serves.

## Contributors

* [Arisien](https://github.com/Arisien) - Main developer

## Dependencies

* [rlutil](http://tapiov.net/rlutil/docs/HTML/files/rlutil-h.html) cli library by [tapio](https://github.com/tapio)

## Usage

*Note: a C++ compiler is required to run the program. Alternatively download the binaries from releases*

* Create a new game by running the binary: `./a.out`
* To uncover a point use `plot x y`
* To flag a point use `flag x y`
* To quit use `quit`

## License
Minesweeper is licensed under the [MIT](LICENSE) license. Feel free to fork the repository or modify the code as you see fit.
