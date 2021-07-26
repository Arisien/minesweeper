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

    enum GameInput { I_PLOT, I_FLAG, I_QUIT, I_RESET, I_NONE };

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

                    uncover(x, y);
                }

                if (state != S_LOST && remaining == 0) state = S_WON;

            }

    };

    class Game {
        public:
            Field field;
            time_t start_time;
            time_t stop_time;
            bool flag_mode;

            Game (int h, int w, int m) : field {h,w,m} {
                flag_mode = false;
            }

            virtual Input input () {
                return Input(I_NONE);
            }

            virtual void render () {}

            virtual void plot (Input in) {
                
                if (!flag_mode) field.play(in.x, in.y, in.flag);

                else {
                    Tile tile = field.tiles[in.y][in.x];

                    if (tile.visible) {
                        if (in.flag == true) return;
                        if (field.flag_count(in.x, in.y) != tile.value) return;

                        for (int i = in.y - 1; i <= in.y + 1; i++) {
                            for (int j = in.x - 1; j <= in.x + 1; j++) {
                                if (i < 0 || j < 0 || i >= field.tiles.size() || j >= field.tiles[i].size()) continue;
                                if (i == in.y && j == in.x) continue;
                                if (field.tiles[i][j].visible || field.tiles[i][j].flagged) continue;

                                field.play(in.x, in.y, false);
                            }
                        }
                    }

                    else {
                        field.play(in.x, in.y, !in.flag);
                    }

                }
            }

            virtual void game_loop () {
                while (true) {

                    render();
                    
                    Input i = input();

                    if (i.type == I_PLOT && field.state == S_PLAYING) {
                        plot(i);
                    }

                    else if (i.type == I_FLAG) {
                        flag_mode = !flag_mode;
                    }

                    else if (i.type == I_QUIT) {
                        field.state = S_LOST;
                    }

                    else if (i.type == I_RESET) {
                        field = Field(field.height, field.width, field.mines);
                        start_time = time(0);
                        stop_time = start_time;
                    }

                    if (field.state != S_PLAYING && start_time == stop_time) {
                        stop_time = time(0);
                    }

                }
            }

            virtual void start () {
                field = Field(field.height, field.width, field.mines);
                start_time = time(0);
                stop_time = start_time;
                game_loop();
            }
    };

};

#endif