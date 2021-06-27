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

#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

namespace Minesweeper {

    enum GameState { S_PLAYING, S_WON, S_LOST };

    enum GameInput { I_POSITION, I_FLAG, I_QUIT, I_NONE };

    class Input {
        public:
            int type;
            int x;
            int y;

            Input (int t) {
                type = t;
            }

            Input (int t, int a, int b) {
                type = t;
                x = a;
                y = b;
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
            int height, width, bombs, flags, remaining;
            int state = S_PLAYING;

            Field (int  h, int w, int b) {

                // Initialize Instance Variables

                height = h;
                width = w;
                bombs = b;
                flags = b;
                remaining = h*w;

                if (bombs > remaining) throw std::invalid_argument("Too many bombs");

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

            void uncover (int x, int y) {
                tiles[y][x].visible = true;
                remaining--;

                if (tiles[y][x].value > 0) return;

                for (int i = y - 1; i <= y + 1; i++) {
                    for (int j = x - 1; j <= x + 1; j++) {
                        if (i < 0 || j < 0 || i >= tiles.size() || j >= tiles[i].size()) continue;

                        if (tiles[i][j].visible || tiles[i][j].flagged) continue;

                        uncover(j, i);
                    }
                }
            }

            void play (int x, int y, bool flag) {
                if (x < 0 || y < 0 || x >= width || y >= height) return;
                
                if (flag) {
                    if (tiles[y][x].visible) return;

                    if (!tiles[y][x].flagged) {
                        if (flags == 0) return;
                        tiles[y][x].flagged = true;
                        flags--;
                        remaining--;
                    }

                    else {
                        tiles[y][x].flagged = false;
                        flags++;
                        remaining++;
                    }
                }

                else {
                    if (tiles[y][x].flagged) return;

                    if (tiles[y][x].value == 9) state = S_LOST;

                    else uncover(x, y);
                }

                if (state != S_LOST && remaining == 0) state = S_WON;

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
                    return Input(I_QUIT);
                }

                else if (cmd == "flag") {
                    return Input(I_FLAG);
                }

                else if (cmd == "plot") {
                    int x, y;

                    std::cin >> x;
                    std::cin >> y;

                    return Input(I_POSITION, x, y);
                }

                return Input(I_NONE);

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

                if (field.state == S_LOST) {
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

                    if (i.type == I_POSITION) {
                        field.play(i.x, i.y, flag_mode);
                    }

                    else if (i.type == I_FLAG) {
                        flag_mode = !flag_mode;
                    }

                    else if (i.type == I_QUIT) {
                        break;
                    }

                    if (field.state != S_PLAYING) {
                        break;
                    }

                }

                end_screen();
            }
    };

};

#endif