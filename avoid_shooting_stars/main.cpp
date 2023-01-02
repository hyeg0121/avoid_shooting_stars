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
		if (Keyboard::isKeyPressed(Keyboard::Up) ||
			Keyboard::isKeyPressed(Keyboard::W)) {
			if (player.sprite.getPosition().y >= 0) {
				player.sprite.move(0, -player.speed);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) ||
			Keyboard::isKeyPressed(Keyboard::D)) {
			if (player.sprite.getPosition().x <= W_WIDTH - player.width) {
				player.sprite.move(player.speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) ||
			Keyboard::isKeyPressed(Keyboard::S)) {
			if (player.sprite.getPosition().y <= W_HEIGHT - player.height) {
				player.sprite.move(0, player.speed);
			}
		}



	}

	return 0;
}