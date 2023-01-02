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
	int life;
};

struct Shooting_stars {
	RectangleShape sprite;
	Texture texture;
	int speed = 5;
	int width = 50, height = 50;
	int respwan_time = 7;
};

int is_collide(RectangleShape obj1, RectangleShape obj2) {
	if (obj1.getGlobalBounds().intersects(obj2.getGlobalBounds())) {
		return 1;
	}
	else {
		return 0;
	}
};

const int W_WIDTH = 1000, W_HEIGHT = 1000;
const int STARS_NUM = 15;

int main(void)
{
	/* window */
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // ������ â ����
	window.setFramerateLimit(60);

	/* ������ */
	long start_time = clock();
	long spent_time;
	int score;
	char info[100];
	int is_gameover;

	/* Font */
	Font font;
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	/* Text */
	Text text;
	text.setFont(font);
	text.setPosition(10, 50);

	/* GameOver */
	RectangleShape gameover;
	Texture go_t;
	go_t.loadFromFile("./resources/images/gameover.png");
	gameover.setSize(Vector2f(400, 400));
	gameover.setTexture(&go_t);

	/* Player */
	Player player;
	player.texture.loadFromFile("./resources/images/horse.png");
	player.sprite.setTexture(&player.texture);
	player.sprite.setFillColor(Color::White);
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(W_WIDTH / 2, W_HEIGHT * 0.8);
	player.life = 3;

	/* Shooting_stars */
	struct Shooting_stars s_star; //sample
	s_star.texture.loadFromFile("./resources/images/shooting-star.png");
	struct Shooting_stars stars[STARS_NUM];
	for (int i = 0; i < STARS_NUM; i++) {
		stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
		stars[i].speed = -(rand() % 8 + 1);
		stars[i].sprite.setPosition(rand() % W_WIDTH-s_star.width, rand() % W_HEIGHT * 0.4);
		stars[i].sprite.setTexture(&s_star.texture);
	}

	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		score = (spent_time / 1000) * 100;
		sprintf_s(info, "SCORE : %10d", score);
		text.setString(info);

		Event event;
		while (window.pollEvent(event))
		{
			/* ���� */
			switch (event.type)
			{
			case Event::Closed :
				window.close();
			default:
				break;
			}
		}

		/* ����Ű */
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
		//stars ������
		for (int i = 0; i < STARS_NUM; i++)
		{
			stars[i].sprite.move(0, s_star.speed);
		}

		//���� �ð� �� stars respwan
		if (spent_time % (1000 * s_star.respwan_time) < 1000 / 60 + 1) 
		{
			for (int i = 0; i < STARS_NUM; i++) 
			{
				stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
				stars[i].speed = -(rand() % 8 + 1);
				stars[i].sprite.setPosition(rand() % W_WIDTH - s_star.width, rand() % W_HEIGHT * 0.3);
			}
		}

		//player�� �浹
		for (int i = 0; i < STARS_NUM; i++) 
		{
			if (is_collide(player.sprite, stars[i].sprite)) {
				score -= 100;
				player.life--;
			}
		}

		
		window.clear(Color::Black);

		if (player.life < 0) {
			gameover.setPosition(450, 450);
			window.draw(gameover);
		}
		else 
		{
			window.draw(player.sprite);
			window.draw(text);
			for (int i = 0; i < STARS_NUM; i++)
				window.draw(stars[i].sprite);
		}
		
		window.display();
	}

	return 0;
}