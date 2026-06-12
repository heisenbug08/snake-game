#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

/*********************************/
// Try editing these values

// Grid configuration
#define GRID_X 16
#define GRID_Y 16
#define SQ_SIZE 32
#define TICK_RATE 10

// Colors
#define CLR_GRIDLINE LIGHTGRAY
#define CLR_BG RAYWHITE
#define CLR_TXT DARKGRAY

#define CLR_HEAD DARKBLUE
#define CLR_SNAKE BLUE
#define CLR_FOOD RED

/*********************************/

#define GRID_SIZE (GRID_X * GRID_Y)
#define SCRW (GRID_X * SQ_SIZE)
#define SCRH ((GRID_Y + 1) * SQ_SIZE)

#define POS_X(pos) (pos % GRID_X)
#define POS_Y(pos) (pos / GRID_X)

typedef enum { EMPTY, HEAD, SNAKE, FOOD } Tile;
typedef enum { UP = -GRID_X, LEFT = -1, DOWN = GRID_X, RIGHT = 1 } Direction;

typedef struct Game {
	bool over;
	int score, high;
	Tile grid[GRID_SIZE];
	int snake[GRID_SIZE];
	int head, tail, nextPos;
	Direction dir;
	bool updateDir;
	bool eat;
} Game;

Game NewGame(void);
void UpdateGame(Game *g);
void DrawGame(Game *g);

#endif
