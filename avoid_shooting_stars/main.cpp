#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

const int W_WIDTH = 1000, W_HEIGHT = 1000;
const int STARS_NUM = 15;
const int MAX_PlAYER_SPEED = 8;

struct Textures {
	Texture background_texture;
	Texture horse_right_texture;
	Texture horse_left_texture;
	Texture item_texture;
};

struct SBuffers {
	SoundBuffer bgm_buffer;
	SoundBuffer move_buffer;
	SoundBuffer earned_item_buffer;
};

struct Sounds {
	Sound bgm;
	Sound move;
	Sound earned_item;
};

struct Player
{
	RectangleShape sprite;
	Texture texture;
	int speed = 5;
	int width = 70, height = 70;
	int life;
};

struct Shooting_stars {
	RectangleShape sprite;
	Texture texture;
	int speed = 3;
	int width = 40, height = 40;
	int respwan_time = 4;
	int is_collide;
};

struct Item {
	RectangleShape sprite;
	int speed = 4;
	int speed_up_effect = 1; 
	int respawn_time = 15;
	int width = 50, height = 50;
	int is_present;
};

int collision_detection(RectangleShape rect1,  RectangleShape rect2) {
		if (
			rect1.getPosition().x+20 < rect2.getPosition().x + rect2.getSize().x &&
			rect1.getPosition().x + rect1.getSize().x-20> rect2.getPosition().x &&
			rect1.getPosition().y < rect2.getPosition().y + rect2.getSize().y &&
			rect1.getSize().y + rect1.getPosition().y > rect2.getPosition().y
			) {
			return 1;
		}
		else {
			return 0;
		}
}

void respwan_star(Shooting_stars * stars) {
	for (int i = 0; i < STARS_NUM; i++) {
		stars[i].sprite.setSize(Vector2f(stars[i].width, stars[i].height));
		stars[i].speed++;
		stars[i].sprite.setPosition(rand() % W_WIDTH, rand() % W_HEIGHT * 0.5);
		stars[i].is_collide = 0;
	}
}

int main(void)
{
	/* window */
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "Avoid Shooting Stars!"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	/* 변수들 */
	long start_time = clock();
	long spent_time;
	int score;
	char info[100];
	int is_gameover;

	srand(time(0));

	/* Textuers */
	struct Textures t;
	t.background_texture.loadFromFile("./resources/images/backgroundimage.png");
	t.horse_left_texture.loadFromFile("./resources/images/horseleft.png");
	t.horse_right_texture.loadFromFile("./resources/images/horseright.png");
	t.item_texture.loadFromFile("./resources/images/item.png");

	/* SoundBuffers & Sound */
	struct SBuffers sb;
	sb.bgm_buffer.loadFromFile("./resources/sounds/BGM.flac");
	sb.move_buffer.loadFromFile("./resources/sounds/move.flac");
	struct Sounds s;
	s.bgm.setBuffer(sb.bgm_buffer);
	s.move.setVolume(70);
	s.move.setBuffer(sb.move_buffer);
	s.move.setVolume(130);

	/* Font */
	Font font;
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	/* Text */
	Text text;
	text.setFont(font);
	text.setPosition(10, 50);

	/* Background */
	RectangleShape bg;
	bg.setTexture(&t.background_texture);
	bg.setSize(Vector2f(W_WIDTH, W_HEIGHT));
	s.bgm.setLoop(1);
	s.bgm.play();

	/* GameOver */
	RectangleShape gameover;
	Texture go_t;
	go_t.loadFromFile("./resources/images/gameover.png");
	gameover.setSize(Vector2f(W_WIDTH, W_HEIGHT));
	gameover.setTexture(&go_t);

	/* Player */
	Player player;
	player.sprite.setTexture(&t.horse_right_texture);
	player.sprite.setFillColor(Color::White);
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(W_WIDTH / 2, W_HEIGHT * 0.8);
	player.life = 1;

	/* Shooting_stars */
	struct Shooting_stars s_star; //sample
	s_star.texture.loadFromFile("./resources/images/shooting-star.png");

	struct Shooting_stars stars[STARS_NUM];
	for (int i = 0; i < STARS_NUM; i++) {
		stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
		stars[i].sprite.setPosition(rand() % W_WIDTH-s_star.width, rand() % W_HEIGHT * 0.4);
		stars[i].sprite.setTexture(&s_star.texture);
		stars[i].is_collide = 0;
	}

	/* Item */
	Item item;
	item.sprite.setSize(Vector2f(item.width, item.height));
	item.sprite.setTexture(&t.item_texture);

	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		score = (spent_time / 1000) * 25;
		sprintf_s(info, "SCORE : %6d", score);
		text.setString(info);

		Event event;
		while (window.pollEvent(event))
		{
			/* 종료 */
			switch (event.type)
			{
			case Event::Closed :
				window.close();
			case Event::KeyPressed :
				s.move.play();
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
				player.sprite.setTexture(&t.horse_left_texture);
				player.sprite.move(-player.speed, 0);
				//s.move.play();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) ||
			Keyboard::isKeyPressed(Keyboard::D)) 
		{
			if (player.sprite.getPosition().x <= W_WIDTH - player.width) 
			{
				player.sprite.setTexture(&t.horse_right_texture);
				player.sprite.move(player.speed, 0);
			//	s.move.play();
			}
		}

		/* Shooting stars update*/
		//stars 움직임
		for (int i = 0; i < STARS_NUM; i++)
		{
			stars[i].sprite.move(0, stars[i].speed);
		}

		//일정 시간 뒤 stars respwan
		if (spent_time % (1000 * s_star.respwan_time) < 1000 / 60 + 1) 
		{
			respwan_star(stars);
		}

		//player와 충돌
		for (int i = 0; i < STARS_NUM; i++) 
		{
			if (collision_detection(player.sprite, stars[i].sprite)) 
			{
				score -= 100;
				player.life--;
				stars[i].is_collide = 1;
			}
		}
		
		/* item update */
		//시간이 지나면 아이템 리스폰
		if (spent_time % (1000 * item.respawn_time) < 1000 / 60 + 1)
		{
			item.sprite.setPosition(rand() % W_WIDTH, rand() % W_HEIGHT * 0.4);
			item.is_present = 1;
		}
		//아이템이 존재하는 동안 움직임
		if (item.is_present)
		{
			item.sprite.move(0, item.speed);
		}
		//플레이어가 아이템을 먹었을 때 효과
		if (collision_detection(player.sprite, item.sprite))
		{
			player.speed += item.speed_up_effect;
			if (player.speed > MAX_PlAYER_SPEED) {
				player.speed = MAX_PlAYER_SPEED;
			}

			item.is_present = 0;
			
		}
		window.clear(Color::Black);
		window.draw(bg);

		//게임오버
		if (player.life < 0)
		{
			window.draw(gameover);
			if (Keyboard::isKeyPressed(Keyboard::Space)) 
			{
				player.life = 3;
				is_gameover = 0;
				start_time = clock();
				for (int i = 0; i < STARS_NUM; i++)
					stars[i].speed = 3;
				respwan_star(stars);
				player.speed = 4;
				
			}
		}
		else 
		{
			window.draw(player.sprite);
			window.draw(text);
			for (int i = 0; i < STARS_NUM; i++)
			{
				if(!(stars[i].is_collide))
					window.draw(stars[i].sprite);
			}
			if (item.is_present)
				window.draw(item.sprite);
		}
		
		window.display();
	}

	return 0;
}