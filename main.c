#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "tiles.h"

const int screenWidth = 800;
const int screenHeight = 450;
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, MENU } GameScreen;
typedef enum Movements { STILL = 0, WALKING, ROLLING, RUNNING } Movements;

// ensure that all resources are pre-defined so that gcc doesn't have a mental breakdown
Texture tile_grass;
Texture tile_water;
Texture tile_img;
Texture nexxus_games;
Texture menu_images;
Texture james_images;
Texture player_images;
Texture hotbar;

typedef struct {
	int x;
	int y;
	float speed;
	float velocity[2];
	int facing;
	int anim;
	int sprite;
	Movements moving;
} Player;

typedef struct {
	int age;
	int lifespan;
	char * name;
} obj_plant;

obj_plant obj_appletree = { 84, 18500, "Apple Tree" };

struct Item {
	char * name;
	char * description;
	char * summary;
	int id;
	char * type;
};

struct Item item_log = { "Log", "It's a log; what were you expecting?", "Basic crafting material.", 0, "material" };

struct Item inventory[16];

void LoadTextures(void);
void UnloadTextures(void);
void UpdateDrawFrame(void);
Player AnimatePlayer(Player player);

int main(void) {

	GameScreen currentScreen = GAMEPLAY;

	Map map = LoadMap("data/maps/map1.txt");
	Player player = { 0, 0, 1, 1, 0, 0, STILL };

	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	LoadTextures();

	while (!WindowShouldClose()) {
		switch (currentScreen) {
			case GAMEPLAY:
				if (IsKeyDown(KEY_W)) { player.facing = 0; player.velocity[1] -= 0.2; }
				if (IsKeyDown(KEY_A)) { player.facing = 1; player.velocity[0] -= 0.2; }
				if (IsKeyDown(KEY_S)) { player.facing = 2; player.velocity[1] += 0.2; }
				if (IsKeyDown(KEY_D)) { player.facing = 3; player.velocity[0] += 0.2; }
				if (player.velocity[0] > 1.5) player.velocity[0] = 1.5;
				if (player.velocity[0] < -1.5) player.velocity[0] = -1.5;
				if (player.velocity[1] > 1.5) player.velocity[1] = 1.5;
				if (player.velocity[1] < -1.5) player.velocity[1] = -1.5;
				player.x += player.velocity[0];
				player.y += player.velocity[1];
				if (player.velocity[0] <= 0) player.velocity[0] += 0.1;
				if (player.velocity[0] >= 0) player.velocity[0] -= 0.1;
				if (player.velocity[1] <= 0) player.velocity[1] += 0.1;
				if (player.velocity[1] >= 0) player.velocity[1] -= 0.1;
				break;
			default:
				break;
		}
		BeginDrawing();
			ClearBackground(RAYWHITE);
			for (int i = 0; i < map.rows; i++) {
				for (int j = 0; j < map.cols; j++) {
					switch (map.data[i][j]) {
						case 0:
							tile_img = tile_water;
							break;
						case 1:
							tile_img = tile_grass;
							break;
					}
					DrawTextureEx(tile_img, (Vector2){ j*30 - player.x, i*30 - player.y }, 0.0f, 2.0f, RAYWHITE);
				}
			}
			player = AnimatePlayer(player);
			DrawTexturePro(james_images, (Rectangle){37 * player.sprite, 54 * player.facing, 37, 54 }, (Rectangle){ 383, 170, 37, 54 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			DrawTexture(hotbar, 1.0f, 419.0f, RAYWHITE);
			DrawFPS(10, 10);
		EndDrawing();
	}
	UnloadTextures();
	CloseWindow();
	return 0;
}

void LoadTextures(void) {
	tile_grass = LoadTexture("data/images/tile_grass.png");
	tile_water = LoadTexture("data/images/tile_water.png");
	james_images = LoadTexture("data/images/james.png");
	menu_images = LoadTexture("data/images/menu.png");
	hotbar = LoadTexture("data/images/hotbar.png");
}

void UnloadTextures(void) {
	UnloadTexture(tile_grass);
	UnloadTexture(tile_water);
	UnloadTexture(tile_img);
	UnloadTexture(james_images);
	UnloadTexture(menu_images);
	UnloadTexture(hotbar);
}

void UpdateDrawFrame(void) {
	
}

Player AnimatePlayer(Player player) {
	player.anim++;
	if (player.anim > 7) {
		player.anim = 0;
		player.sprite++;
		if (player.sprite >= 8) {
			player.sprite = 0;
		}
	}
	return player;
}