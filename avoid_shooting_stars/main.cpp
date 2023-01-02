#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player
{
	RectangleShape sprite;
	Texture texture;
	int speed = 3;
	int width = 70, height = 70;
};

const int W_WIDTH = 1000, W_HEIGHT = 1000;
int main(void)
{
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	/* Player */
	Player player;
	player.sprite.setFillColor(Color::White);
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(W_WIDTH / 2, W_HEIGHT * 0.8);

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			/* 종료 */
			switch (event.type)
			{
			case Event::Closed :
				window.close();
			default:
				break;
			}
		}

		/* 방향키 */
		if (Keyboard::isKeyPressed(Keyboard::Left) ||
			Keyboard::isKeyPressed(Keyboard::A)) {
			if (player.sprite.getPosition().x >= 0) {
				player.sprite.move(-player.speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) ||
			Keyboard::isKeyPressed(Keyboard::D)) {
			if (player.sprite.getPosition().x <= W_WIDTH - player.width) {
				player.sprite.move(player.speed, 0);
			}
		}



		window.clear(Color::Black);

		window.draw(player.sprite);

		window.display();
	}

	return 0;
}