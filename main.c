#include <math.h>
#include "raylib.h"
#include "player.h"
#include "enemies.h"
#include "tiles.h"
#include "items.h"

int screenWidth = 800;
int screenHeight = 450;

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
	Texture slash = LoadTexture("data/images/slash_1.png");
	Texture tile_water = LoadTexture("data/images/tile_water.png");
	Texture tile_grass = LoadTexture("data/images/tile_grass.png");
	Texture tile_dirt = LoadTexture("data/images/tile_dirt.png");
	Texture tile_sand = LoadTexture("data/images/tile_sand.png");
	Texture tile_images[10] = { tile_water, tile_grass, tile_dirt, tile_sand };
	Music boomtown = LoadMusicStream("data/music/boomtown.ogg");
	Music music = boomtown;
	// PlayMusicStream(music);

	Sound james_voice = LoadSound("data/sfx/james.wav");

	float rotation;

	int melee_frame = 0;
	float melee_rotation;
	bool swinging_weapon = false;
	Rectangle sword_hitbox;

	while (!WindowShouldClose()) {
		rotation = atan2f(GetMouseY() - GetScreenHeight() / 2, GetMouseX() - GetScreenWidth() / 2) * 180 / PI + 45;
		UpdateMusicStream(music);
		// Get input
		player = CollidePlayerWithTile(map, player);
		player = MovePlayer(player);
		player = AnimatePlayer(player);
		if (IsKeyPressed(KEY_SPACE)) {
			SetSoundPitch(james_voice, GetRandomValue(0, 4));
			PlaySound(james_voice);
		}
		if (IsKeyPressed(KEY_E)) {
			AddItem(inventory, 5, item_log);
		}
		if (IsKeyPressed(KEY_Q)) {
			RemoveItem(inventory, 3, item_log);
		}
		if (IsKeyPressed(KEY_KP_1)) {
			CraftItem(inventory, sword_recipe, item_woodsword);
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && swinging_weapon == false) {
			melee_frame = 0;
			swinging_weapon = true;
		}
		if (swinging_weapon == true) {
			if (melee_frame < 135) {
				melee_frame += 15;
			} else {
				swinging_weapon = false;
			}
		}
		switch (player.facing) {
			case UP:
				melee_rotation = 240;
				break;
			case LEFT:
				melee_rotation = 150;
				break;
			case DOWN:
				melee_rotation = 60;
				break;
			case RIGHT:
				melee_rotation = 330;
				break;
		}
		// Update
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawMap(map, tile_images, player);
			DrawTexturePro(james, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, (Rectangle){ GetScreenWidth() / 2, GetScreenHeight() / 2, 37, 54 }, (Vector2){ 18.5f, 27.0f }, 0.0f, RAYWHITE);
			if (swinging_weapon == true) {
				DrawTexturePro(slash, (Rectangle){ 0, 0, 30, 30 }, (Rectangle){ GetScreenWidth() / 2, GetScreenHeight() / 2, 60, 60 }, (Vector2){ 0.0f, 60.0f }, melee_rotation + melee_frame, RAYWHITE);
				DrawTexturePro(bluesteel_sabre, (Rectangle){ 0, 0, 30, 30 }, (Rectangle){ GetScreenWidth() / 2, GetScreenHeight() / 2, 60, 60 }, (Vector2){ 0.0f, 60.0f }, melee_rotation + melee_frame, RAYWHITE);
				sword_hitbox = (Rectangle){ GetScreenWidth() / 2, GetScreenHeight() / 2, 60, 60 };
				DrawRectangleLinesEx(sword_hitbox, 1.0f, )
			}
			DrawTexture(hotbar, 1.0f, GetScreenHeight() - 31, RAYWHITE);
			DrawItems(inventory, item_images);
			player = CollidePlayerWithTile(map, player);
			DrawFPS(10, 10);
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