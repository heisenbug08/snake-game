#include "game.h"
#include "icon_data.h"
#include "raylib.h"

int main() {
	InitWindow(SCRW, SCRH, "Snake Game");
	SetTargetFPS(120);

	Image icon = {ICON_DATA, ICON_DATA_WIDTH, ICON_DATA_HEIGHT, 1,
				  ICON_DATA_FORMAT};
	SetWindowIcon(icon);

	Game game = NewGame();

	while (!WindowShouldClose()) {
		UpdateGame(&game);
		DrawGame(&game);
	}

	CloseWindow();
	return 0;
}
