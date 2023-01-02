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
	int width = 50, height = 50;
};

struct Shooting_stars {
	RectangleShape sprite;
	Texture texture;
	int speed = 5;
	int width = 50, height = 50;
	int respwan_time = 7;
};

const int W_WIDTH = 1000, W_HEIGHT = 1000;
const int STARS_NUM = 10;

int main(void)
{
	/* window */
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	/* 변수들 */
	long start_time = clock();
	long spent_time;
	int score;
	char info[100];

	/* Font */
	Font font;
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	/* Text */
	Text text;
	text.setFont(font);
	text.setPosition(10, 50);

	/* Player */
	Player player;
	player.sprite.setFillColor(Color::White);
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(W_WIDTH / 2, W_HEIGHT * 0.8);

	/* Shooting_stars */
	struct Shooting_stars s_star; //sample
	struct Shooting_stars stars[STARS_NUM];
	for (int i = 0; i < STARS_NUM; i++) {
		stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
		stars[i].speed = -(rand() % 7 + 1);
		stars[i].sprite.setPosition(rand() % W_WIDTH-s_star.width, rand() % W_HEIGHT * 0.3);
	}

	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		score = spent_time / 1000;
		sprintf_s(info, "SCORE : %10d", score);
		text.setString(info);

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
			Keyboard::isKeyPressed(Keyboard::A)) 
		{
			if (player.sprite.getPosition().x >= 0) 
			{
				player.sprite.move(-player.speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) ||
			Keyboard::isKeyPressed(Keyboard::D)) 
		{
			if (player.sprite.getPosition().x <= W_WIDTH - player.width) 
			{
				player.sprite.move(player.speed, 0);
			}
		}

		/* Shooting stars*/
		//stars 움직임
		for (int i = 0; i < STARS_NUM; i++)
		{
			stars[i].sprite.move(0, s_star.speed);
		}

		//일정 시간 뒤 stars respwan
		if (spent_time % (1000 * s_star.respwan_time) < 1000 / 60 + 1) {
			for (int i = 0; i < STARS_NUM; i++) {
				stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
				stars[i].speed = -(rand() % 7 + 1);
				stars[i].sprite.setPosition(rand() % W_WIDTH - s_star.width, rand() % W_HEIGHT * 0.3);
			}
		}

		
		window.clear(Color::Black);

		window.draw(player.sprite);
		window.draw(text);
		for (int i = 0; i < STARS_NUM; i++)
			window.draw(stars[i].sprite);

		window.display();
	}

	return 0;
}