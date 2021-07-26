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

		int bar_size = 52;
		int width, height;
		
		GuiGame (int h, int w, int m) : Game {h,w,m} {

			app.create(sf::VideoMode(32*field.width, 32*field.height), "Minesweeper++");

			texture.loadFromFile("./sprites.png");

			sprite = sf::Sprite(texture);

			width = 32 * w;
			height = bar_size + 32 * h;
		}

        Input input () {

        	sf::Vector2i pos = sf::Mouse::getPosition(app);

			int x = pos.x / 32;
			int y = (pos.y - bar_size) / 32;

			sf::Event event;

			while (app.pollEvent(event))
			{
				if (event.type == sf::Event::Closed){
					return Input(I_QUIT);
				}

				if (event.type == sf::Event::MouseButtonReleased) {
					if (event.key.code == sf::Mouse::Left) {
						
						if (pos.x >= width/2 - 26 && pos.x <= width/2 + 26 && pos.y >= 0 && pos.y <= 52) return Input(I_RESET);

						else return Input(I_PLOT, x, y , false);

					}
					else if (event.key.code == sf::Mouse::Right) {
						return Input(I_PLOT, x, y, true);

					}
				}

			}
			
			return Input(I_NONE);

        }

		void render () {
			
			//Draw Flag Count
			int fc = field.flags;

			int max = (width/2 - 26)/26 - 1;

			if (max > 2) max = 2;
			
			for (int i = max; i >= 0; i--) {
				sprite.setTextureRect(sf::IntRect((fc%10) * 26, 32, 26, 46));
				sprite.setPosition(i*26, 3);
				app.draw(sprite);
				fc /= 10;
			}

			//Draw Face

			sprite.setTextureRect(sf::IntRect(field.state*52, 78, 52, 52));
			sprite.setPosition(width/2 - 26, 0);
			app.draw(sprite);


			//Draw Time

			int elapsed;

			if (start_time == stop_time) elapsed = time(0) - start_time;
			else elapsed = stop_time - start_time;

			for (int i = 0; i <= max; i++) {
				sprite.setTextureRect(sf::IntRect((elapsed%10) * 26, 32, 26, 46));
				sprite.setPosition(width - ((i+1)*26), 3);
				app.draw(sprite);
				elapsed /= 10;
			}

			//Draw Field

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
					sprite.setPosition(j*32, bar_size + i*32);

					app.draw(sprite);
                }
            }
			
            app.display();

		}
};

int main (int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " [height] [width] [mines]\n";
        return 0;
    }

    int height = std::stoi(argv[1]);
    int width = std::stoi(argv[2]);
    int mines = std::stoi(argv[3]);

    GuiGame game(height, width, mines);

    game.start();

    return 0;
}
