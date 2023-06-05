#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "tiles.h"

const int screenWidth = 800;
const int screenHeight = 450;
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, MENU } GameScreen;
typedef enum Movements { STILL, WALKING, ROLLING, RUNNING } Movements;
typedef enum Directions { UP, DOWN, LEFT, RIGHT } Directions;

// ensure that all resources are pre-defined so that gcc doesn't have a mental breakdown
Texture tile_grass;
Texture tile_water;
Texture tile_img;
Texture nexxus_games;
Texture menu_images;
Texture james_images;
Texture player_images;
Texture snake_images;
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
	Rectangle hitbox;
	int iframes;
} Player;

struct obj_enemy {
	float x;
	float y;
	int maxhp;
	int hp;
	float speed;
	int facing;
	int anim;
	int sprite;
	Rectangle hitbox;
};

struct obj_enemy obj_tattlesnake = { 0.0f, 0.0f, 45, 45, 0.4, 0, 0, 0, (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f } };

struct obj_enemy enemies[20];

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
struct obj_enemy AnimateEnemy(struct obj_enemy enemy);
bool * CollideEntityWithTile(const Rectangle entity, const Rectangle tile, bool collisions[4]);

int main(void) {

	GameScreen currentScreen = GAMEPLAY;

	for (int i = 0; i < 20; i++) {
		enemies[i] = obj_tattlesnake;
	}

	Map map = LoadMap("data/maps/map1.txt");
	Player player = { 0, 0, 2.0f, {0.0f, 0.0f }, 2, 0, 0, STILL, (Rectangle){ 383, 170, 37, 54 }, 40 };

	Rectangle tile_hitboxes[map.rows][map.cols];

	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	LoadTextures();

	while (!WindowShouldClose()) {
		switch (currentScreen) {
			case GAMEPLAY:
				Rectangle tile_hitboxes[map.rows][map.cols];
				player.moving = STILL;
				if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { player.moving = WALKING; player.facing = 0; player.velocity[1] -= player.speed / 10; }
				if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { player.moving = WALKING; player.facing = 1; player.velocity[0] -= player.speed / 10; }
				if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { player.moving = WALKING; player.facing = 2; player.velocity[1] += player.speed / 10; }
				if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { player.moving = WALKING; player.facing = 3; player.velocity[0] += player.speed / 10; }
				if (player.velocity[0] >= player.speed) player.velocity[0] = player.speed;
				if (player.velocity[0] <= -player.speed) player.velocity[0] = -player.speed;
				if (player.velocity[1] >= player.speed) player.velocity[1] = player.speed;
				if (player.velocity[1] <= -player.speed) player.velocity[1] = -player.speed;
				if (player.velocity[0] <= 0) player.velocity[0] += player.speed / 20;
				if (player.velocity[0] >= 0) player.velocity[0] -= player.speed / 20;
				if (player.velocity[1] <= 0) player.velocity[1] += player.speed / 20;
				if (player.velocity[1] >= 0) player.velocity[1] -= player.speed / 20;
				for (int i = 0; i < map.rows; i++) {
					for (int j = 0; j < map.cols; j++) {
						if (map.data[i][j] == 0) tile_hitboxes[i][j] = (Rectangle){ j*30-player.x, i*30-player.y, 30, 30};
						if (CheckCollisionRecs(player.hitbox, tile_hitboxes[i][j]) == true) {
							bool collisions2[4];
							bool collisions[4] = CollideEntityWithTile(player.hitbox, tile_hitboxes[i][j], collisions2);
							if (collisions[UP] == true) player.y += player.speed;
							if (collisions[LEFT] == true) player.x += player.speed;
							if (collisions[DOWN] == true) player.y -= player.speed;
							if (collisions[RIGHT] == true) player.x -= player.speed;
						for (int k = 0; k < 20; k++) {
							if (CheckCollisionRecs(enemies[k].hitbox, tile_hitboxes[i][j]) == true) {
								bool collisions[4];
							}
						}
						}
					}
				}
				player.x += player.velocity[0];
				player.y += player.velocity[1];
				for (int i = 0; i < 20; i++) {
					if (enemies[i].facing == 1) enemies[i].x += enemies[i].speed;
					if (enemies[i].facing == 0) enemies[i].x -= enemies[i].speed;
					enemies[i].hitbox = (Rectangle){ enemies[i].x + 4 - player.x, enemies[i].y + 22 - player.y, 24, 8 };
					if (CheckCollisionRecs(player.hitbox, enemies[i].hitbox) == true) CloseWindow();
				}
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
					DrawRectangleLinesEx(tile_hitboxes[i][j], 1.0f, BLUE);
				}
			}
			player = AnimatePlayer(player);
			DrawTexturePro(james_images, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, player.hitbox, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			DrawRectangleLinesEx(player.hitbox, 1.0f, SKYBLUE);
			for (int i = 0; i < 20; i++) {
				DrawTexturePro(snake_images, (Rectangle){ 15 * enemies[i].sprite, 15 * enemies[i].facing, 15, 15 }, (Rectangle){ enemies[i].x - player.x, enemies[i].y - player.y, 30, 30 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
				enemies[i] = AnimateEnemy(enemies[i]);
				DrawRectangleLinesEx(enemies[i].hitbox, 1.0f, RED);
			}
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
	snake_images = LoadTexture("data/images/snake.png");
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
	if (player.moving != STILL) {
		player.anim++;
		if (player.anim > 7) {
			player.anim = 0;
			player.sprite++;
			if (player.sprite >= 8) {
				player.sprite = 0;
			}
		}
	} else {
		player.anim = 0;
		player.sprite = 0;
	}
	return player;
}

struct obj_enemy AnimateEnemy(struct obj_enemy enemy) {
	enemy.anim++;
	if (enemy.anim > 7) {
		enemy.anim = 0;
		enemy.sprite++;
		if (enemy.sprite >= 4) {
			enemy.sprite = 0;
		}
	}
	return enemy;
}

bool * CollideEntityWithTile(const Rectangle entity, const Rectangle tile, bool collisions[4]) {
	Rectangle up = { entity.x, entity.y, entity.width, 2 };
	Rectangle left = { entity.x, entity.y, 2, entity.height };
	Rectangle down = { entity.x, entity.y + entity.height, entity.width, 2 };
	Rectangle right = { entity.x + entity.width, entity.y, 2, entity.height };
	if (CheckCollisionRecs(up, tile) == true) collisions[UP] = true;
	if (CheckCollisionRecs(down, tile) == true) collisions[DOWN] = true;
	if (CheckCollisionRecs(left, tile) == true) collisions[LEFT] = true;
	if (CheckCollisionRecs(right, tile) == true) collisions[RIGHT] = true;
	return collisions;
}