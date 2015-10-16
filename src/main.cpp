#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "ResourcePath.h"
#include "Overlap.h"

struct Monster {
	sf::Texture tex;
	sf::Sprite sprite;
	bool healthy;
};

struct Score {
	sf::Text scoreText;
	sf::Font font;
	int value;
};

struct Game {
	Monster player;
	Monster m1;
	Score score;
};

void createMonster(std::string path, Monster& monster)
{
	monster.tex.loadFromFile(resourcePath() + path);
	monster.sprite.setTexture(monster.tex);
	monster.healthy = true;
}

void createFont(std::string path, Score& score)
{
	score.font.loadFromFile(resourcePath() + path);
	score.scoreText.setFont(score.font);
	score.scoreText.setPosition(5, 5);
	score.scoreText.setColor(sf::Color::White);
	score.scoreText.setString(sf::String(std::to_string(score.value)));

}

void load(Game& game)
{
	createMonster("assets/charizard.png", game.player);
	createMonster("assets/cookie.png", game.m1);
	game.m1.sprite.setPosition(400, 300);
	game.score.value = 0;
	createFont("assets/georgia.ttf", game.score);
}

void handleEvents(sf::RenderWindow& window, Game& game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void arrowkeys(sf::RenderWindow& window, Monster& player)
{
	float speed = 5;
	float diagSpeed = 5 / sqrt(2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		player.sprite.move(-diagSpeed, -diagSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		player.sprite.move(diagSpeed, -diagSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		player.sprite.move(-diagSpeed, diagSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		player.sprite.move(diagSpeed, diagSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		player.sprite.move(-speed, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		player.sprite.move(speed, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		player.sprite.move(0, -speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		player.sprite.move(0, speed);
	}
}

void collisions(Game& game)
{
	if (overlap(game.player.sprite, game.m1.sprite)) {
		game.m1.healthy = false;
	}
}

void update(sf::RenderWindow& window, Game& game)
{
	arrowkeys(window, game.player);
	collisions(game);
	if (!game.m1.healthy) {
		game.score.value++;
		game.m1.sprite.setPosition((std::rand() % 700) + 50 - 32, (std::rand() % 500) + 50 - 32);
		game.player.sprite.setScale(1.0 + game.score.value * 0.1, 1.0 + game.score.value * 0.1);
		game.score.scoreText.setString(sf::String(std::to_string(game.score.value)));
		game.m1.healthy = true;
	}
}

void draw(sf::RenderWindow& window, Game& game)
{
	window.clear();
	window.draw(game.player.sprite);
	window.draw(game.m1.sprite);
	window.draw(game.score.scoreText);
	window.display();
}

int main()
{
	std::srand(std::time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 600), "Charzilla");
	window.setVerticalSyncEnabled(true);
	Game game;
	load(game);

	while (window.isOpen())
	{
		handleEvents(window, game);
		update(window, game);
		draw(window, game);
	}

	return 0;
}
