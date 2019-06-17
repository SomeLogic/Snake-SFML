#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include <random>
#include <fstream>
#include <string>

#define row_size 20
#define column_size 15

struct Apple {
	int x, y;
};

struct Coord {
	int x, y;

	Coord(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

int runGame();
void updateGame(sf::Time elapsedTime, std::unique_ptr<Snake> &snake, Apple &apple, 
	char direction, sf::Sprite headSprite, sf::Sprite appleSprite, int &score, int &highscore);
int randomIntGenerator(int min, int max);
void drawSnake(std::unique_ptr<Snake> &snake, sf::Sprite headSprite, char direction);
void drawApple(Apple apple, sf::Sprite appleSprite);
Apple genApplePos(Apple &apple);
bool isAppleCollected(const std::unique_ptr<Snake> &snake, const Apple apple);
bool isPartOfSnake(const std::unique_ptr<Snake> &snake, const Apple apple);
bool isLose(std::unique_ptr<Snake> &snake, std::vector<Coord> &coord);
void saveHighscore(int& highscore, int& failCounter);
void readHighscore(int& highscore, int& failCounter);