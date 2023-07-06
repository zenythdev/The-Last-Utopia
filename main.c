#include "raylib.h"
#include "player.h"
#include "tiles.h"
#include "items.h"

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
	// Initialize game
	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	Map map = LoadMap("data/maps/map1.txt");

	Player player = { (Vector2){ 0.0f, 0.0f }, (Vector2){ 0.0f, 0.0f }, 0.05f, 0, 0, 0 };
	Item inventory[16];
	for (int i = 0; i < 16; i++) {
		inventory[i] = item_air;
	}
	Texture james = LoadTexture("data/images/james.png");
	Texture hotbar = LoadTexture("data/images/hotbar.png");
	Texture log = LoadTexture("data/images/item_log2.png");
	Texture bluesteel_sabre = LoadTexture("data/images/item_bluesteel_sabre.png");
	Texture tile_water = LoadTexture("data/images/tile_water.png");
	Sound james_voice = LoadSound("data/sfx/james.ogg");

	while (!WindowShouldClose()) {
		// Get input
		player = MovePlayer(player);
		player = AnimatePlayer(player);
		if (IsKeyPressed(KEY_SPACE)) {
			PlaySound(james_voice);
		}
		if (IsKeyPressed(KEY_E)) {
			AddItem(inventory, 1, item_bluesteel_sabre);
		}
		// Update
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawMap(map, tile_water, player);
			DrawTexturePro(james, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, (Rectangle){ GetScreenWidth() / 2 - 18.5, GetScreenHeight() / 2 - 27, 37, 54 }, (Vector2){ 0.0f, 0.0f }, 0.0f, RAYWHITE);
			DrawTexture(hotbar, 1.0f, GetScreenHeight() - 31, RAYWHITE);
			for (int i = 0; i < 16; i++) {
				if (inventory[i].id == 35) {
					DrawTexture(bluesteel_sabre, 1.0f + 31 * i, GetScreenHeight() - 31, RAYWHITE);
					if (inventory[i].count > 1) {
						DrawText(TextFormat("%d", inventory[i].count), 1.0f + 31 * i + 19, GetScreenHeight() - 13, 10, BLACK);
					}
				}
			}
		EndDrawing();
	}
	CloseWindow();
	UnloadSound(james_voice);
	UnloadTexture(hotbar);
	UnloadTexture(james);
	UnloadTexture(tile_water);
	UnloadTexture(log);
	UnloadTexture(bluesteel_sabre);
	return 0;
}