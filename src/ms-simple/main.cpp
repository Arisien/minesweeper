/*
 * ms-simple.cpp
 * 
 * This is a basic implementation of Minesweeper. The program will take command
 * line arguments for width and height of field and amount of mines and then it
 * will create a new instance of the Game object.
 *
 * Author: Arisien @ 26.6.2021
 *
 */

#include "../minesweeper.hpp"

int main (int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " [height] [width] [mines]\n";
        return 0;
    }

    int height = std::stoi(argv[1]);
    int width = std::stoi(argv[2]);
    int mines = std::stoi(argv[3]);

    Minesweeper::Game game = Minesweeper::Game(height, width, mines);

    game.start();

    return 0;
}
