/*
 * ms-gui.cpp
 * 
 * This is a GUI implementation of Minesweeper. SFML is used for the graphics.
 *
 * Author: Arisien @ 26.6.2021
 *
 */

#include <SFML/Graphics.hpp>
#include <ctime>

#include "../minesweeper.hpp"

using namespace Minesweeper;

class GuiGame: public Game {
	public:
		sf::RenderWindow app;
		sf::Texture texture;
		sf::Sprite sprite;

		GuiGame (int h, int w, int m) : Game {h,w,m} {

			app.create(sf::VideoMode(32*field.width, 32*field.height), "Minesweeper++");

			texture.loadFromFile("./tiles.jpg");

			sprite = sf::Sprite(texture);
			
		}

        Input input () {

        	sf::Vector2i pos = sf::Mouse::getPosition(app);

			int x = pos.x / 32;
			int y = pos.y / 32;

			sf::Event event;

			while (app.pollEvent(event))
			{
				if (event.type == sf::Event::Closed){
					return Input(I_QUIT);
				}

				if (event.type == sf::Event::MouseButtonReleased) {
					if (event.key.code == sf::Mouse::Left) {
						return Input(I_PLOT, y, x , false);

					}
					else if (event.key.code == sf::Mouse::Right) {
						return Input(I_PLOT, y, x, true);

					}
				}

			}
			
			return Input(I_NONE);

        }

		void render () {

            for (int i = 0; i < field.height; i++) {
                for (int j = 0; j < field.width; j++) {

                    Tile tile = field.tiles[i][j];

					int xPos = 0;

                    if (tile.flagged) {
						xPos = 11*32; //11th sprite - FLAG
					}
                    else if (tile.visible) {
						xPos = tile.value*32; //nth sprite - VALUE
					}
                    else {
						xPos = 10*32; //10th sprite - EMPTY
					}

					sprite.setTextureRect(sf::IntRect(xPos, 0, 32, 32));
					sprite.setPosition(i*32, j*32);

					app.draw(sprite);
                }
            }
			
            app.display();

		}

		void end_screen () {
			
			Input in = Input(I_NONE);
			while (in.type == I_NONE) {
				in = input();
				render();
			}

			app.close();
		}

};

int main () {

	GuiGame game(5, 5, 5);

	game.start();

	return 0;
}
