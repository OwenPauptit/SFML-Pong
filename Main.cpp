#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int WIDTH = 800, HEIGHT = 600;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Paddle {
	public:
		const int SPEED = 6, PHEIGHT = 60, PWIDTH = 15;
		int x, y;
		bool up = false, down = false;
		Paddle(int);
		void moveUp(), moveDown();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Ball {
	const int STARTVY = 6, STARTVX = 6;
	public:
		const int BHEIGHT = 20, BWIDTH = 20;
		int x, y, vx, vy;
		Ball();
		void reflectY(), reflectX(), move(), spin(), normal();
		int isCollision(Paddle, Paddle);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Paddle::Paddle(int posx) {
	x = posx;
	y = (HEIGHT / 2) - (PHEIGHT / 2);
}

void Paddle::moveUp() {
	if (y > 10) {
		y -= SPEED;
	}
}

void Paddle::moveDown() {
	if (y < HEIGHT - 10 - PHEIGHT) {
		y += SPEED;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Ball::Ball() {
	x = (WIDTH / 2) - (BWIDTH / 2);
	y = (HEIGHT / 2) - (BHEIGHT / 2);
	vx = 0;
	while (vx == 0) {
		vx = -STARTVX + rand() % (STARTVX * 2);
	}
	vy = -STARTVY + rand() % (STARTVY * 2);
}

int Ball::isCollision(Paddle p1, Paddle p2) {
	if ((x < p1.x + p1.PWIDTH && x > p1.x && y + BHEIGHT > p1.y && y < p1.y + p1.PHEIGHT )) {
		reflectX();
		if (y > (p1.y + 5 * p1.PHEIGHT / 6) || y + BHEIGHT < (p1.y + p1.PHEIGHT / 6)) {
			spin();
		}
		else if (y + BHEIGHT >= p1.y + p1.PHEIGHT / 2 && y <= p1.y + p2.PHEIGHT/2) {
			normal();
		}
	}
	else if ((x + BWIDTH > p2.x&& x < p2.x + p2.PWIDTH && y + BHEIGHT > p2.y&& y < p2.y + p2.PHEIGHT)) {
		reflectX();
		if (y > (p2.y + 5 * p2.PHEIGHT / 6) || y + BHEIGHT < (p2.y + p2.PHEIGHT / 6)) {
			spin();
		}
		else if (y + BHEIGHT/2 >= p2.y + p2.PHEIGHT / 2 && y - BHEIGHT/2 <= p2.y + p2.PHEIGHT / 2) {
			normal();
		}
	}
	else if (x < 5){
		return 1;
	}
	else if (x > WIDTH - 5) {
		return 0;
	}
	if (y < 5 || y + BHEIGHT > HEIGHT - 5) {
		reflectY();
	}
	return -1;
}

void Ball::reflectX() {
	vx  = 0 - vx;
}

void Ball::reflectY() {
	vy = 0 - vy;
}

void Ball::move() {
	x += vx;
	y += vy;
}

void Ball::spin() {
	if (vy == 0) {
		vy = 2;
	}
	vy *= 1.5;
	vx *= 1.3;
}

void Ball::normal() {
	vy = 0;
	vx *= 1.2;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int game(int,int);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

	srand(time(0));

	int scores[] = { 0,0 };

	while (true) {

		scores[game(scores[0],scores[1])] ++;
		std::cout << scores[0] << scores[1] << std::endl;

	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int game(int s1, int s2) {

	sf::RenderWindow window(sf::VideoMode(800, 600), "PONG!");
	window.setVerticalSyncEnabled(true);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	sf::Font font;
	if (!font.loadFromFile("8-BIT WONDER.TTF"))
	{
		std::cout << "Font error" << std::endl;
		exit(EXIT_FAILURE);
	}
	sf::Text text;
	text.setFont(font);
	text.setString(std::to_string(s1)+"   "+std::to_string(s2));
	text.setCharacterSize(81);
	text.setPosition((WIDTH / 2- (text.getGlobalBounds().width)/2), 30);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Paddle p1 = Paddle(WIDTH / 20);
	Paddle p2 = Paddle(19 * WIDTH / 20 - p1.PWIDTH);
	Ball   b  = Ball();

	sf::RectangleShape r1(sf::Vector2f(p1.PWIDTH, p1.PHEIGHT));
	sf::RectangleShape r2(sf::Vector2f(p2.PWIDTH, p2.PHEIGHT));
	sf::RectangleShape rb(sf::Vector2f(b.BWIDTH,  b.BHEIGHT));

	int result;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			case sf::Event::Closed:
				window.close();
				exit(EXIT_SUCCESS);
				break;

			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			p2.up = true;
			p2.down = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			p2.up = false;
			p2.down = true;
		}
		else {
			p2.up = false;
			p2.down = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			p1.up = true;
			p1.down = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			p1.up = false;
			p1.down = true;
		}
		else {
			p1.up = false;
			p1.down = false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (p1.up) {
			p1.moveUp();
		}
		if (p1.down) {
			p1.moveDown();
		}
		if (p2.up) {
			p2.moveUp();
		}
		if (p2.down) {
			p2.moveDown();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		b.move();
		result = b.isCollision(p1, p2);
		rb.setPosition(b.x, b.y);

		r1.setPosition(p1.x, p1.y);
		r2.setPosition(p2.x, p2.y);
		window.clear(sf::Color::Black);
		window.draw(r1);
		window.draw(r2);
		window.draw(rb);
		window.draw(text);
		window.display();

		if (result >= 0) {
			return result;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}