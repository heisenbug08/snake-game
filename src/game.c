#include "game.h"
#include "raylib.h"
#include "stdlib.h"
#include <stdbool.h>

/*********************************/
// Private value declarations

static Color palette[] = {CLR_HEAD, CLR_SNAKE, CLR_FOOD};

static int frameCount = 0, now;

static const char *TITLE_TXT = "GAME OVER";
static const char *SUB_TXT = "[SPACE] TO PLAY";
static const char *SCORE_FORMAT = "SCORE: %d | HIGH: %d";
static const char *SCORE_TXT = "SCORE: 0 | HIGH: 0";

static const int TITLE_FONT = SCRH * 0.12;
static const int SPACE_FONT = SCRH * 0.08;
static const int SCORE_FONT = SQ_SIZE - 4;

/*********************************/
// Private function definitions

static void PlaceFood(Game *g) {
	int food;
	do
		food = GetRandomValue(0, GRID_SIZE-1);
	while (g->grid[food]);
	g->grid[food] = FOOD;
}

static void InitGame(Game *g) {
	g->over = false;
	g->score = 0;
	SCORE_TXT = TextFormat(SCORE_FORMAT, 0, g->high);

	for (int i = 0; i < GRID_SIZE; i++) {
		g->snake[i] = 0;
		g->grid[i] = EMPTY;
	}

	g->head = 1, g->tail = 0;
	g->grid[0] = SNAKE;
	g->snake[1] = 1;
	g->grid[1] = HEAD;

	g->dir = RIGHT;
	PlaceFood(g);
}

/*********************************/
// Public function definitions

Game NewGame(void) {
	Game newgame = {0};
	InitGame(&newgame);
	return newgame;
}

void UpdateGame(Game *g) {
	if (g->over && IsKeyPressed(KEY_SPACE))
		InitGame(g);

	if (g->updateDir && !g->over) {
		if (g->dir == LEFT || g->dir == RIGHT) {
			if (IsKeyPressed(KEY_UP)) {
				g->dir = UP;
				g->updateDir = false;
			}
			if (IsKeyPressed(KEY_DOWN)) {
				g->dir = DOWN;
				g->updateDir = false;
			}
		} else {
			if (IsKeyPressed(KEY_LEFT)) {
				g->dir = LEFT;
				g->updateDir = false;
			}
			if (IsKeyPressed(KEY_RIGHT)) {
				g->dir = RIGHT;
				g->updateDir = false;
			}
		}
	}

	// Tick-wise updates
	now = (int)(GetTime() * TICK_RATE);
	if (now > frameCount) {
		frameCount = now;

		if (!g->over) {
			g->eat = false;
			g->nextPos = g->snake[g->head] + g->dir;

			// Game Over condition
			if (g->nextPos > GRID_SIZE - 1 || g->nextPos < 0 ||
				g->grid[g->nextPos] == SNAKE ||
				abs(POS_X(g->nextPos) - POS_X(g->snake[g->head])) > 1) {
				g->over = true;
				return;
			}

			if (g->grid[g->nextPos] == FOOD) {
				g->eat = true;
				PlaceFood(g);
				if (++g->score > g->high)
					g->high = g->score;
				SCORE_TXT = TextFormat(SCORE_FORMAT, g->score, g->high);
			}

			g->grid[g->snake[g->head]] = SNAKE;
			g->head = (g->head + 1) % GRID_SIZE;
			g->snake[g->head] = g->nextPos;
			g->grid[g->snake[g->head]] = HEAD;

			if (!g->eat) {
				g->grid[g->snake[g->tail]] = EMPTY;
				g->tail = (g->tail + 1) % GRID_SIZE;
			}

			g->updateDir = true;
		}
	}
}

void DrawGame(Game *g) {
	BeginDrawing();

	ClearBackground(CLR_BG);

	// Draw grid tiles
	for (int i = 0; i < GRID_X * GRID_Y; i++)
		if (g->grid[i] != EMPTY)
			DrawRectangle(POS_X(i) * SQ_SIZE, POS_Y(i) * SQ_SIZE, SQ_SIZE,
						  SQ_SIZE, palette[g->grid[i] - 1]);

	// Draw grid lines
	for (int x = SQ_SIZE; x < SCRW; x += SQ_SIZE)
		DrawLine(x, 0, x, SCRH - SQ_SIZE, CLR_GRIDLINE);
	for (int y = SQ_SIZE; y <= SCRW; y += SQ_SIZE)
		DrawLine(0, y, SCRW, y, CLR_GRIDLINE);

	// Menu
	if (g->over) {
		DrawText(TITLE_TXT, (SCRW - MeasureText(TITLE_TXT, TITLE_FONT)) / 2,
				 SCRH * 0.35, TITLE_FONT, CLR_TXT);
		DrawText(SUB_TXT, (SCRW - MeasureText(SUB_TXT, SPACE_FONT)) / 2,
				 SCRH * 0.55, SPACE_FONT, CLR_TXT);
	}
	DrawText(SCORE_TXT, 20, SCRH - SCORE_FONT, SCORE_FONT, CLR_TXT);

	EndDrawing();
}
