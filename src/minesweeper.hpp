/*
 * minesweeper.hpp
 * 
 * This header provides core Minesweeper mechanics with the Field class as well
 * as a basic I/O system for making an implementation using the Game class. See
 * the documentation for this header at https://github.com/Arisien/minesweeper.
 *
 * Author: Arisien @ 26.6.2021
 *
 */

#ifndef MINESWEEPER_GAME
#define MINESWEEPER_GAME

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

namespace Minesweeper {

    enum game_states { PLAYING, WON, LOST };

    enum inputs { POSITION, FLAG, QUIT, NONE };

    class Position {
        public:
            int x, y;

            Position () {
                x = 0;
                y = 0;
            }

            Position (int a, int b) {
                x = a;
                y = b;
            }
    };

    class Input {
        public:
            int type;
            Position position;

            Input (int t, Position p) : position { p } {
                type = t;
                position = p;
            }
    };

    class Tile {
        public:
            int value;
            bool visible = false;
            bool flagged = false;
    };

    class Field {
        public:
            std::vector<std::vector<Tile>> tiles;
            int height, width, bombs, flags;
            int state = PLAYING;

            Field (int  h, int w, int b) {

                // Initialize Instance Variables

                height = h;
                width = w;
                bombs = b;
                flags = b;

                if (b > h*w) throw std::invalid_argument("Too many bombs");

                // Generate Tile Vector
                
                tiles = std::vector<std::vector<Tile>>(height);

                for (int i = 0; i < height; i++) {
                    tiles[i] = std::vector<Tile>(width);

                    for (int j = 0; j < width; j++) {
                        tiles[i][j] = Tile();
                    }
                }

                // Generate Bombs

                std::srand(std::time(NULL));

                for (int i = 0; i < bombs; i++) {
                    int x,y;
                    do {
                        y = rand() % height;
                        x = rand() % width;
                    } while (tiles[y][x].value == 9);
                    tiles[y][x].value = 9;
                }

                // Initialize Tile Values

                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        if (tiles[i][j].value == 9) continue;

                        int count = 0;

                        for (int k = i - 1; k <= i + 1; k++) {
                            for (int l = j - 1; l <= j + 1; l++) {
                                if (k < 0 || l < 0 || k >= tiles.size() || l >= tiles[i].size()) continue;

                                if (k == i && l == j) continue;

                                if (tiles[k][l].value == 9) count++;
                            }
                        }

                        tiles[i][j].value = count;
                    }
                }
                
            }

            int check_state () {
                if (state != PLAYING) return state;

                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        if (!tiles[i][j].flagged && !tiles[i][j].visible) return PLAYING;
                    }
                }

                state = WON;

                return WON;
            }

            void uncover (int x, int y) {
                tiles[y][x].visible = true;

                for (int i = y - 1; i <= y + 1; i++) {
                    for (int j = x - 1; j <= x + 1; j++) {
                        if (i < 0 || j < 0 || i >= tiles.size() || j >= tiles[i].size()) continue;

                        if (tiles[i][j].visible || tiles[i][j].flagged) continue;

                        if (tiles[i][j].value == 0) uncover(j, i);

                        else tiles[i][j].visible = true;
                    }
                }
            }

            void play (int x, int y, bool flag) {
                
                if (flag) {
                    if (tiles[y][x].visible) return;

                    if (!tiles[y][x].flagged) {
                        if (flags == 0) return;
                        tiles[y][x].flagged = true;
                        flags--;
                    }

                    else {
                        tiles[y][x].flagged = false;
                        flags++;
                    }
                }

                else {
                    if (tiles[y][x].flagged) return;

                    if (tiles[y][x].value > 0) {
                        if (tiles[y][x].value == 9) state = LOST;

                        tiles[y][x].visible = true;
                    }

                    else uncover(x, y);

                    check_state();
                }

            }

    };

    class Game {
        public:
            Field field;
            bool flag_mode;

            Game (int h, int w, int b) : field {h,w,b} {
                field = Field(h,w,b);
                flag_mode = false;
            }

            virtual Input input () {

                std::cout << "$";

                std::string cmd;

                std::cin >> cmd;

                if (cmd == "quit")  {
                    return Input(QUIT, Position());
                }

                else if (cmd == "flag") {
                    return Input(FLAG, Position());
                }

                else if (cmd == "plot") {
                    int x, y;

                    std::cin >> x;
                    std::cin >> y;

                    return Input(POSITION, Position(x,y));
                }

                return Input(NONE, Position());

            }

            virtual void render () {
                std::cout << "Minesweeper++" << std::endl << std::endl;

                for (int i = 0; i < field.height; i++) {
                    for (int j = 0; j < field.width; j++) {

                        Tile tile = field.tiles[i][j];

                        if (tile.flagged) std::cout << "F";
                        else if (tile.visible) std::cout << tile.value;
                        else std::cout << "#";

                        std::cout << " ";

                    }
                    std::cout << std::endl;
                }

                std::cout << std::endl << "Flags: " << field.flags << std::endl;
                
                std::string flag_str = "ON";

                if (!flag_mode) flag_str = "OFF";

                std::cout << "Flag mode is " << flag_str << std::endl;
            }

            virtual void start_screen () {
                std::system("cls||clear");
            }

            virtual void end_screen () {

                std::cout << "Game Over! ";

                if (field.state == LOST) {
                    std::cout << "Better luck next time." << std::endl;
                }

                else {
                    std::cout << "Congratulations" << std::endl;
                }
                
            }

            virtual void start () {
                start_screen();

                while (true) {

                    render();

                    Input i = input();

                    if (i.type == POSITION) {
                        Position position = i.position;
                        field.play(position.x, position.y, flag_mode);
                    }

                    else if (i.type == FLAG) {
                        flag_mode = !flag_mode;
                    }

                    else if (i.type == QUIT) {
                        break;
                    }

                    if (field.state != PLAYING) {
                        break;
                    }

                }

                end_screen();
            }
    };

};

#endif