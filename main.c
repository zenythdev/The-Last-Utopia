#include "raylib.h"
#include "player.h"
#include "tiles.h"

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
	// Initialize game
	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	Map map = LoadMap("data/maps/map1.txt");

	Player player = { (Vector2){ 0.0f, 0.0f }, (Vector2){ 0.0f, 0.0f }, 0.05f, 0, 0, 0 };

	Texture james = LoadTexture("data/images/james.png");
	Texture tile_water = LoadTexture("data/images/tile_water.png");
	Sound james_voice = LoadSound("data/sfx/james.ogg");

	while (!WindowShouldClose()) {
		// Get input
		player = MovePlayer(player);
		player = AnimatePlayer(player);
		if (IsKeyPressed(KEY_SPACE)) {
			PlaySound(james_voice);
		}
		// Update
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawMap(map, tile_water, player);
			DrawTexturePro(james, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, (Rectangle){ GetScreenWidth() / 2 - 18.5, GetScreenHeight() / 2 - 27, 37, 54 }, (Vector2){ 0.0f, 0.0f }, 0.0f, RAYWHITE);
		EndDrawing();
	}
	CloseWindow();
	UnloadSound(james_voice);
	UnloadTexture(james);
	UnloadTexture(tile_water);
	return 0;
}