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

	Item sword_recipe[2] = { item_log, item_log };

	Player player = { (Vector2){ 0.0f, 0.0f }, (Vector2){ 0.0f, 0.0f }, 0.05f, 0, 0, 0 };
	Item inventory[16];
	for (int i = 0; i < 16; i++) {
		inventory[i] = item_air;
	}
	Texture james = LoadTexture("data/images/james.png");
	Texture hotbar = LoadTexture("data/images/hotbar.png");
	Texture log = LoadTexture("data/images/item_log2.png");
	Texture rock = LoadTexture("data/images/item_rock.png");
	Texture bluesteel_sabre = LoadTexture("data/images/item_bluesteel_sabre.png");
	Texture item_images[2] = { log, rock };
	Texture tile_water = LoadTexture("data/images/tile_water.png");
	Texture tile_grass = LoadTexture("data/images/tile_grass.png");
	Texture tile_dirt = LoadTexture("data/images/tile_dirt.png");
	Texture tile_sand = LoadTexture("data/images/tile_sand.png");
	Texture tile_images[10] = { tile_water, tile_grass, tile_dirt, tile_sand };
	Music boomtown = LoadMusicStream("data/music/boomtown.ogg");
	Music music = boomtown;
	PlayMusicStream(music);

	Sound james_voice = LoadSound("data/sfx/james.ogg");

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);
		// Get input
		player = MovePlayer(player);
		player = AnimatePlayer(player);
		if (IsKeyPressed(KEY_SPACE)) {
			PlaySound(james_voice);
		}
		if (IsKeyPressed(KEY_E)) {
			AddItem(inventory, 5, item_log);
		}
		if (IsKeyPressed(KEY_Q)) {
			RemoveItem(inventory, 3, item_log);
		}
		if (IsKeyPressed(KEY_KP_1)) {
			CraftItem(inventory, sword_recipe, item_rock);
		}
		// Update
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawMap(map, tile_images, player);
			DrawTexturePro(james, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, (Rectangle){ GetScreenWidth() / 2 - 18.5, GetScreenHeight() / 2 - 27, 37, 54 }, (Vector2){ 0.0f, 0.0f }, 0.0f, RAYWHITE);
			DrawTexture(hotbar, 1.0f, GetScreenHeight() - 31, RAYWHITE);
			DrawItems(inventory, item_images);
		EndDrawing();
	}
	CloseWindow();
	UnloadMusicStream(boomtown);
	UnloadMusicStream(music);
	UnloadSound(james_voice);
	UnloadTexture(hotbar);
	UnloadTexture(james);
	UnloadTexture(tile_water);
	UnloadTexture(tile_grass);
	UnloadTexture(tile_dirt);
	UnloadTexture(tile_sand);
	UnloadTexture(log);
	UnloadTexture(rock);
	UnloadTexture(bluesteel_sabre);
	return 0;
}