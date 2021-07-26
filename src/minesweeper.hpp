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

    enum GameInput { I_PLOT, I_FLAG, I_QUIT, I_NONE };

    class Input {
        public:
            int type;
            int x;
            int y;
            bool flag;

            Input (int t) {
                type = t;
            }

            Input (int t, int a, int b, bool f) {
                type = t;
                x = a;
                y = b;
                flag = f;
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
            int height, width, mines, flags, remaining;
            int state = S_PLAYING;

            Field (int  h, int w, int m) {

                // Initialize Instance Variables

                height = h;
                width = w;
                mines = m;
                flags = m;
                remaining = h*w;

                if (mines > remaining) throw std::invalid_argument("Too many mines");

                // Generate Tile Vector
                
                tiles = std::vector<std::vector<Tile>>(height);

                for (int i = 0; i < height; i++) {
                    tiles[i] = std::vector<Tile>(width);

                    for (int j = 0; j < width; j++) {
                        tiles[i][j] = Tile();
                    }
                }

                // Generate Mines

                std::srand(std::time(NULL));

                for (int i = 0; i < mines; i++) {
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

            int flag_count (int x, int y) {
                    int count = 0;

                    for (int i = y - 1; i <= y + 1; i++) {
                        for (int j = x - 1; j <= x + 1; j++) {
                            if (i < 0 || j < 0 || i >= tiles.size() || j >= tiles[i].size()) continue;
                            if (i == y && j == x) continue;
                            if (tiles[i][j].flagged) count++;
                        }
                    }

                    return count;
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

            Game (int h, int w, int m) : field {h,w,m} {
                field = Field(h,w,m);
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
                    bool f;

                    std::cout << "X: ";
                    std::cin >> x;
                    std::cout << "Y: ";
                    std::cin >> y;
                    std::cout << "Flag: ";
                    std::cin >> f;

                    return Input(I_PLOT, x, y, f);
                }

                return Input(I_NONE);
            }

            virtual void render () {

                std::system("clear");

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
                //std::system("cls||clear");
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

            virtual void plot (Input inp) {
                
                if (!flag_mode) field.play(inp.x, inp.y, inp.flag);

                else {
                    Tile tile = field.tiles[inp.y][inp.x];

                    if (tile.visible) {
                        if (inp.flag == true) return;
                        if (field.flag_count(inp.x, inp.y) != tile.value) return;

                        for (int i = inp.y - 1; i <= inp.y + 1; i++) {
                            for (int j = inp.x - 1; j <= inp.x + 1; j++) {
                                if (i < 0 || j < 0 || i >= field.tiles.size() || j >= field.tiles[i].size()) continue;
                                if (i == inp.y && j == inp.x) continue;
                                if (field.tiles[i][j].visible || field.tiles[i][j].flagged) continue;

                                field.play(inp.x, inp.y, false);
                            }
                        }
                    }

                    else {
                        field.play(inp.x, inp.y, !inp.flag);
                    }

                }
            }

            virtual void game_loop () {
                while (true) {

                    render();

                    Input i = input();

                    if (i.type == I_PLOT) {
                        plot(i);
                    }

                    else if (i.type == I_FLAG) {
                        flag_mode = !flag_mode;
                    }

                    else if (i.type == I_QUIT) {
                        field.state = S_LOST;
                        break;
                    }

                    if (field.state != S_PLAYING) {
                        break;
                    }

                }
            }

            virtual void start () {
                start_screen();

                game_loop();

                end_screen();
            }
    };

};

#endif