#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

struct Snake {
	int x, y;
	std::unique_ptr<Snake> next;

	Snake() {
		x = 0;
		y = 0;
		next = nullptr;
	}

	Snake(int x, int y) {
		this->x = x;
		this->y = y;
		next = nullptr;
	}
};

void addToSnake(std::unique_ptr<Snake> &snake, int x, int y);
void updateSnake(std::unique_ptr<Snake> &snake, int x, int y, char c);
void deleteFromSnake(std::unique_ptr<Snake> &snake);