#include "Game.h"

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

sf::RenderWindow window(sf::VideoMode(1000, 850), "Snake 2.0!", sf::Style::Close | sf::Style::Resize);

int runGame()
{
	int highscore = 0, failCounter = 0;
	readHighscore(highscore, failCounter);
	int difficulty = 125, score = 0;
	char direction = sf::Keyboard::Right, newDirection = sf::Keyboard::Right;
	sf::RectangleShape shape({ 40.0f, 40.0f });
	shape.setFillColor(sf::Color::Green);

	sf::Sprite appleSprite;
	sf::Texture appleTexture;
	appleTexture.loadFromFile("apple.png", { 0, 0, 50, 50 });
	appleSprite.setTexture(appleTexture);

	sf::Sprite headSprite;
	sf::Texture headTexture;
	headTexture.loadFromFile("head.png", { 0, 0, 50, 50 });
	headSprite.setTexture(headTexture);

	std::unique_ptr<Snake> snake;
	Apple apple = genApplePos(apple);
	addToSnake(snake, column_size / 2, row_size / 2 - 2);
	addToSnake(snake, column_size / 2, row_size / 2 - 1);
	addToSnake(snake, column_size / 2, row_size / 2);
	addToSnake(snake, column_size / 2, row_size / 2 + 1);

	sf::Clock clock;
	sf::Time elapsedTime = sf::seconds(0.0f);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			newDirection = direction;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && newDirection != sf::Keyboard::Left)
				newDirection = sf::Keyboard::Right;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && newDirection != sf::Keyboard::Right)
				newDirection = sf::Keyboard::Left;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && newDirection != sf::Keyboard::Down)
				newDirection = sf::Keyboard::Up;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && newDirection != sf::Keyboard::Up)
				newDirection = sf::Keyboard::Down;
		}
		elapsedTime = clock.getElapsedTime();
		if (elapsedTime >= sf::milliseconds(100))
		{
			updateGame(elapsedTime, snake, apple, newDirection, headSprite, appleSprite, score, highscore);
			clock.restart();
			direction = newDirection;
		}
	}
	return 0;
}

void updateGame(sf::Time elapsedTime, std::unique_ptr<Snake> &snake, Apple &apple, 
	char direction, sf::Sprite headSprite, sf::Sprite appleSprite, int &score, int &highscore){
	sf::RectangleShape scoreShape({ 1000.0f, 100.0f });
	scoreShape.setFillColor(sf::Color(113, 113, 113));
	scoreShape.setPosition({ 0.0f,0.0f });

	//delete unnecessary tail if apple wasn't collected
	if (!isAppleCollected(snake, apple)) {
		deleteFromSnake(snake);
	}
	else {
		score++;
		while (isPartOfSnake(snake, apple)) {
			apple = genApplePos(apple);
		}
	}

	std::vector<Coord> coord;
	if (isLose(snake, coord)) {
		if (score > highscore) {
			highscore = score;
			saveHighscore;
		}
		window.close();
	}

	std::string scoreString = "Score: " + std::to_string(score);

	sf::Font arialFont;
	arialFont.loadFromFile("Arial.ttf");
	sf::Text scoreText;
	scoreText.setFont(arialFont);
	scoreText.setString(scoreString);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition({ 30.0f, 25.0f });

	//add new head 
	updateSnake(snake, snake->x, snake->y, direction);

	window.clear(sf::Color(150, 150, 150));
	drawSnake(snake, headSprite, direction);
	drawApple(apple, appleSprite);
	window.draw(scoreShape);
	window.draw(scoreText);
	window.display();
}

//generate random integer between min and max
int randomIntGenerator(int min, int max) {
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

//generate random apple position #worksFine
Apple genApplePos(Apple &apple) {
	apple.x = randomIntGenerator(1, 18); //row
	apple.y = randomIntGenerator(1, 13); //column
	return apple;
}

//check if apple is on the same position as snake is #worksFine
bool isAppleCollected(const std::unique_ptr<Snake> &snake, const Apple apple) {
	if (snake != NULL && snake->next == NULL) {
		if (apple.x == snake->x && apple.y == snake->y) return true;
	}
	//if current snake part is not same as apple position, check next snake part
	else if (isAppleCollected(snake->next, apple)) return true;
	return false;
}

bool isPartOfSnake(const std::unique_ptr<Snake> &snake, const Apple apple) {
	if (snake != NULL) {
		if (apple.x == snake->x && apple.y == snake->y) return true;
	}
	//if current snake part is not same as apple position, check next snake part
	if (snake->next != NULL)
		if (isPartOfSnake(snake->next, apple) == true) return true;

	return false;
}

//check if snake is on position of game window's border
bool isLose(std::unique_ptr<Snake> &snake, std::vector<Coord> &coord) {
	if (snake != NULL) {
		if (snake->next != NULL) coord.insert(coord.end(), { snake->x, snake->y });
		if (snake->x == 0 || snake->x == 20 || snake->y == 0 || snake->y == 15) return true;
	}
	if (snake != NULL && snake->next == NULL) {
		for (int i = 0; i < coord.size(); i++) {
			if (coord.at(i).x == snake->x && coord.at(i).y == snake->y) return true;
		}
	}
	if (snake != NULL && isLose(snake->next, coord)) return true;
	return false;
}

void drawSnake(std::unique_ptr<Snake> &snake, sf::Sprite headSprite, char direction) {
	if (snake->next != NULL) {
		drawSnake(snake->next, headSprite, direction);
		sf::RectangleShape body({ 34.0f, 34.0f });
		body.setPosition({ snake->x * 50.0f + 8.0f, snake->y * 50.0f + 108.0f});
		body.setFillColor(sf::Color::Green);
		window.draw(body);
	}
	else if (snake->next == NULL) {
		switch (direction) {
		case sf::Keyboard::Left:
			headSprite.scale(-1, 1);
			//headSprite.rotate(180.0f); 
			headSprite.setOrigin(50.0f, 0.0f); break;
		case sf::Keyboard::Up:
			headSprite.rotate(270.0f); 
			headSprite.setOrigin(50.0f, 0.0f); break;
		case sf::Keyboard::Down: 
			headSprite.rotate(90.0f);
			headSprite.setOrigin(0.0f, 50.0f); break; 
		}
		headSprite.setPosition({ snake->x * 50.0f + 5.0f, snake->y * 50.0f +105.0f});
		window.draw(headSprite);
		headSprite.setOrigin(0.0f, 0.0f);
		headSprite.setRotation(0.0f);
	}
}

void drawApple(Apple apple, sf::Sprite appleSprite){
	appleSprite.setPosition({ apple.x * 50.0f, apple.y * 50.0f + 100.0f });
	window.draw(appleSprite);
}

void saveHighscore(int& highscore, int& failCounter) {
	std::ofstream output("C:\\Games\\Highscore.dat", std::ios::binary);
	output.write(reinterpret_cast<char*> (&highscore), sizeof(&highscore));
	output.write(reinterpret_cast<char*> (&failCounter), sizeof(&failCounter));
	output.close();
}

void readHighscore(int& highscore, int& failCounter) {
	std::ifstream input("C:\\Games\\Highscore.dat", std::ios::binary);
	if (input.good()) {
		input.read(reinterpret_cast<char*> (&highscore), sizeof(&highscore));
		input.read(reinterpret_cast<char*> (&failCounter), sizeof(&failCounter));
	}
	input.close();
}
