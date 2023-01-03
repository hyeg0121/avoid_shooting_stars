#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

/* TODO LIST
1) 아이템 넣기
2) 별똥별 떨어지는 속도 조금씩 올리기
*/
using namespace sf;

struct Textures {
	Texture background_texture;
	Texture horse_right_texture;
	Texture horse_left_texture;
};

struct SBuffers {
	SoundBuffer bgm_buffer;
	SoundBuffer move_buffer;
};

struct Sounds {
	Sound bgm;
	Sound move;
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
	int speed;
	int width = 40, height = 40;
	int respwan_time = 4;
	int increased_speed;
};

int circular_collision_detection(RectangleShape rect1,  RectangleShape rect2) {
		if (
			rect1.getPosition().x < rect2.getPosition().x + rect2.getSize().x &&
			rect1.getPosition().x + rect1.getSize().x> rect2.getPosition().x &&
			rect1.getPosition().y < rect2.getPosition().y + rect2.getSize().y &&
			rect1.getSize().y + rect1.getPosition().y > rect2.getPosition().y
			) {
			return 1;
		}
		else {
			return 0;
		}
}


const int W_WIDTH = 1000, W_HEIGHT = 1000;
const int STARS_NUM = 15;

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
	s_star.increased_speed = 1;

	struct Shooting_stars stars[STARS_NUM];
	for (int i = 0; i < STARS_NUM; i++) {
		stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
		stars[i].speed = 5;
		stars[i].sprite.setPosition(rand() % W_WIDTH-s_star.width, rand() % W_HEIGHT * 0.4);
		stars[i].sprite.setTexture(&s_star.texture);
	}

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

		/* Shooting stars*/
		//stars 움직임
		for (int i = 0; i < STARS_NUM; i++)
		{
			stars[i].sprite.move(0, stars[i].speed);
		}

		//일정 시간 뒤 stars respwan
		if (spent_time % (1000 * s_star.respwan_time) < 1000 / 60 + 1) 
		{
			for (int i = 0; i < STARS_NUM; i++) 
			{
				stars[i].sprite.setSize(Vector2f(s_star.width, s_star.height));
				stars[i].speed;
				stars[i].sprite.setPosition(rand() % W_WIDTH - s_star.width, rand() % W_HEIGHT * 0.3);
			}
		}

		//player와 충돌
		for (int i = 0; i < STARS_NUM; i++) 
		{
			if (circular_collision_detection(player.sprite, stars[i].sprite)) {
				score -= 100;
				player.life--;
			}
		}

		
		window.clear(Color::Black);
		window.draw(bg);

		//게임오버
		if (player.life < 0) {
			window.draw(gameover);
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				player.life = 1;
				is_gameover = 0;
				score = 0;
			}
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