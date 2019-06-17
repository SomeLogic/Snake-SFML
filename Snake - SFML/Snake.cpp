#include "Snake.h"

//add coordinates manually
void addToSnake(std::unique_ptr<Snake> &snake, int x, int y) {
	if (snake == NULL) snake = std::make_unique<Snake>(x, y);
	else addToSnake(snake->next, x, y);
}

//insert into snake new coordinates to make it longer
void updateSnake(std::unique_ptr<Snake> &snake, int x, int y, char c) {
	if (snake == NULL) snake = std::make_unique<Snake>(x, y);
	else switch (c) {
	case sf::Keyboard::Left: {
		updateSnake(snake->next, snake->x - 1, snake->y, c); break;
	}
	case sf::Keyboard::Up: {
		updateSnake(snake->next, snake->x, snake->y - 1, c); break;
	}
	case sf::Keyboard::Right: {
		updateSnake(snake->next, snake->x + 1, snake->y, c); break;
	}
	case sf::Keyboard::Down: {
		updateSnake(snake->next, snake->x, snake->y + 1, c); break;
	}
	default: break;
	}
}

//delete first element of "snake" which is tail of our snake
void deleteFromSnake(std::unique_ptr<Snake> &snake) {
	if (snake->next != NULL) {
		snake->x = snake->next->x;
		snake->y = snake->next->y;
		deleteFromSnake(snake->next);
	}
	else {
		snake = NULL;
	}
}