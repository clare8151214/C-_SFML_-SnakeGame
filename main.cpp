#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
int main() {
	sf::Vector2i fieldSize(20, 15);
	//畫分數用
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	

	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile("block.png")) {
		std::cout << "block.png is not found." << std::endl;
		return EXIT_FAILURE;
	}
	sf::Sprite block(blockTexture);
	sf::Vector2f blockSize(block.getLocalBounds().width , block.getLocalBounds().height);

	sf::VideoMode mode(
		unsigned int(fieldSize.x * blockSize.x), 
		unsigned int(fieldSize.y * blockSize.y)
	);
	sf::RenderWindow w(mode, L"貪吃蛇");
	
	enum  class Direction
	{
		UP,DOWN,LEFT,RIGHT
	};
	Direction direction = Direction::RIGHT;

	std::vector<sf::Vector2i> snake = { sf::Vector2i(3,4) };
	std::vector<sf::Vector2i>::iterator it;
	//兩個食物
	sf::Vector2i food(rand() % fieldSize.x, rand() % fieldSize.y);
	sf::Vector2i food2(rand() % fieldSize.x, rand() % fieldSize.y);

	sf::Clock clock;
	bool isDead = false;
	int sum = 0;
	while (w.isOpen()) {
		sf::Event evt;
		if (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Up) {
					direction = Direction::UP;
				}
				if (evt.key.code == sf::Keyboard::Down) {
					direction = Direction::DOWN;
				}
				if (evt.key.code == sf::Keyboard::Left) {
					direction = Direction::LEFT;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					direction = Direction::RIGHT;
				}
				
			}
			//死後可按任意鍵重來
			if (isDead) {
				if (evt.type == sf::Event::KeyPressed) {
					isDead = false;
					sum = 0;
					snake = { sf::Vector2i(3,4) };
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
				}
			}

		}
		

		
		if (clock.getElapsedTime().asSeconds() >= 0.4f) {
			sf::Vector2i head = snake[0];
			if (direction == Direction::UP) {
				head.y--;
			}
			if (direction == Direction::DOWN) {
				head.y++;
			}
			if (direction == Direction::LEFT) {
				head.x--;
			}
			if (direction == Direction::RIGHT) {
				head.x++;
			}
			//檢查是否出界
			if (head.x < 0 || head.x >= fieldSize.x ||
				head.y < 0 || head.y >= fieldSize.y) {
				isDead = true;
			}
			//檢查是否撞身體
			for (it = snake.begin() + 1; it != snake.end(); it++) {
				if (head.x == it->x && head.y == it->y) {
					isDead = true;
				}
			}
			if (!isDead) {
				snake.insert(snake.begin(), head);
				if (food == head) {
					++sum;
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
				}
				else if (food2 == head) {
					++sum;
					food2.x = rand() % fieldSize.x;
					food2.y = rand() % fieldSize.y;
				}
				else {
					snake.pop_back();
				}


			}
			

			clock.restart();

		}
		

		//Dead message
		if (isDead) {
			w.clear(sf::Color::Red);
			sf::Text text("your are dead", font, 15);
			w.draw(text);
		}
		else {
			w.clear();
			std::string s = "your score is " + std::to_string(sum);
			sf::Text text(s, font, 15);
			w.draw(text);
		}
		
		
		
		//draw food position
		sf::Vector2f foodPos(
			food.x* blockSize.x,
			food.y* blockSize.y
		);
		block.setPosition(foodPos);
		w.draw(block);

		//draw food2 position
		sf::Vector2f food2Pos(
			food2.x* blockSize.x,
			food2.y* blockSize.y
		);
		block.setPosition(food2Pos);
		w.draw(block);


		//draw body position
		for (const sf::Vector2i& body : snake) {
			sf::Vector2f bodyPos(
						body.x * blockSize.x,
						body.y * blockSize.y
					);
			block.setPosition(bodyPos);
			w.draw(block);
		}
		
		
		w.display();


	}

	return EXIT_SUCCESS;
}