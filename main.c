// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "rlgl.h"

// definitions
#define MAX_HP 30
#define MAX_TOOLS 1
#define DANE "dane"
#define JAMES "james"
#define ALL "all"
#define NONE "none"

// global variables
const int screenWidth = 800;
const int screenHeight = 450;
int hp = 30;
int ammo = 0;
int max_ammo = 0;
float rotation = 0;
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, MENU } GameScreen;
typedef enum Movements { STILL, WALKING, ROLLING, RUNNING } Movements;
typedef enum Directions { UP, DOWN, LEFT, RIGHT } Directions;

// define textures
Texture tile_grass;
Texture tile_water;
Texture tile_img;
Texture nexxus_games;
Texture menu_images;
Texture james_images;
Texture player_images;
Texture snake_images;
Texture hotbar;
Texture healthbar;
Texture axe;
Texture shotgun;
Texture bullet;
Texture meat_bullet;

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

struct obj_enemy enemies[20];

struct Item {
	char* name;
	char* description;
	char* summary;
	int num;
	int max_num;
	int id;
	char* type;
	char* usable_for;
};

struct Item item_log = { "Log", "It's a log; what were you expecting?", "Basic crafting material.", 1, 30, 0, "material", NONE };
struct Item item_axe = { "Axe", "It has seen better days, but it still works.", "Can chop weak trees.", 1, MAX_TOOLS, 1, "axe", JAMES };
struct Item item_shotgun = { "Shotgun", "Good for defending against home invaders.", "Fires bullets in a slight spread.", 1, MAX_TOOLS, 2, "shotgun", ALL };
struct Item item_higgs_boson = { "The Higgs Boson", "Said to be able to separate molecules into atoms.", "WARNING: BE VERY CAREFUL WITH THIS.", 1, MAX_TOOLS, 600, "shotgun", DANE };
struct Item item_wood_sword = { "Wood Sword", "Looks a bit like a toy. Not very strong.", "I mean, it's a sword. It slashes. Kind of.", 1, MAX_TOOLS, 3, "shortsword", JAMES };

struct Item inventory[16];

typedef struct {
	int** data;
	int rows;
	int cols;
} Map;

struct SaveData {
	char* filename;
	char* alias;
};

struct Projectile {
	Vector2 velocity;
	Vector2 position;
	float rotation;
};

struct Projectile projectiles[200];
int projectile_number = 0;

// prototypes
void LoadTextures(void);
void UnloadTextures(void);
void UpdateDrawFrame(void);
Player AnimatePlayer(Player player);
struct obj_enemy AnimateEnemy(struct obj_enemy enemy);
Player CollidePlayerWithTile(const Rectangle entity, const Rectangle tile, Player player);
struct obj_enemy CollideEnemyWithTile(const Rectangle entity, const Rectangle tile, struct obj_enemy enemy);
Player CollidePlayerWithEnemy(const Rectangle entity, const Rectangle entity2, Player player, struct obj_enemy enemy);
int AnimateHP(int hp, int change, int segment);
Map LoadMap(const char* filename);
struct SaveData ReadSave(const char* filename);
struct SaveData WriteSave(const char* filename);

int main(void) {

	GameScreen currentScreen = GAMEPLAY;

	Player player = { 0, 0, 2.0f, {0.0f, 0.0f }, 2, 0, 0, STILL, (Rectangle){ 383, 170, 37, 54 }, 40 };

	for (int i = 0; i < 20; i++) {
		enemies[i] = (struct obj_enemy) { 60.0f + 15*i, 60.0f + 15*i, 45, 45, 0.0005, 0, 0, 0, (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f } };
		enemies[i].hitbox = (Rectangle){ enemies[i].x-player.x, enemies[i].y+22-player.y, 30, 8 };
	}

	int health_frame[3] = { 0, 0, 0 };
	int health_anim[3] = { 0, 0, 0 };
	bool animate_health[3] = { false, false, false };

	Map map = LoadMap("data/maps/map1.txt");

	Rectangle tile_hitboxes[map.rows][map.cols];

	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	Font font = LoadFont("data/Terminal.ttf");

	Music overworld = LoadMusicStream("data/music/overworld.ogg");
	Music music = overworld;
	PlayMusicStream(music);

	LoadTextures();

	float dx, dy, length;
	int random;
	int random2;

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);
		switch (currentScreen) {
			case GAMEPLAY:
				Rectangle tile_hitboxes[map.rows][map.cols];
				player.moving = STILL;
				if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { 
					player.moving = WALKING; 
					player.facing = 0; 
					player.velocity[1] = -player.speed; 
				}
				if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { 
					player.moving = WALKING; 
					player.facing = 1; 
					player.velocity[0] = -player.speed; 
				}
				if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { 
					player.moving = WALKING; 
					player.facing = 2; 
					player.velocity[1] = player.speed;
				}
				if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { 
					player.moving = WALKING; 
					player.facing = 3; 
					player.velocity[0] = player.speed; 
				}
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					for (int i = 0; i < 199; i++) {
						random = GetRandomValue(-90, 90);
						random2 = GetRandomValue(-90, 90);
						dx = GetMouseX() - 435 + random;
						dy = GetMouseY() - 215 + random2;
						length = sqrt(dx*dx + dy*dy);

						dx /= length;
						dy /= length;
						projectiles[projectile_number] = (struct Projectile){(Vector2){ dx * 15, dy * 15 }, (Vector2){ 435, 215 }, atan2f(215 - GetMouseY(), 435 - GetMouseX()) * 180 / PI + 225 + random};
						if (projectile_number < 199) projectile_number++;
						else {
							memset(projectiles, 0, sizeof(projectiles));
							projectile_number = 0;
						}
					}
				}
				if (player.velocity[0] >= player.speed) {
					player.velocity[0] = player.speed;
				}
				if (player.velocity[0] <= -player.speed) {
					player.velocity[0] = -player.speed;
				}
				if (player.velocity[1] >= player.speed) {
					player.velocity[1] = player.speed;
				}
				if (player.velocity[1] <= -player.speed) {
					player.velocity[1] = -player.speed;
				}
				if (player.velocity[0] <= 0) {
					player.velocity[0] += player.speed / 20;
				}
				if (player.velocity[0] >= 0) {
					player.velocity[0] -= player.speed / 20;
				}
				if (player.velocity[1] <= 0) {
					player.velocity[1] += player.speed / 20;
				}
				if (player.velocity[1] >= 0) {
					player.velocity[1] -= player.speed / 20;
				}

				for (int i = 0; i < map.rows; i++) {
					for (int j = 0; j < map.cols; j++) {
						if (map.data[i][j] == 0) {
							tile_hitboxes[i][j] = (Rectangle){ j*30-player.x, i*30-player.y, 30, 30};
						}
						for (int k = 0; k < 20; k++) {
							switch(enemies[k].facing) {
								case 0:
									enemies[k].x -= enemies[k].speed;
									break;
								case 1:
									enemies[k].x += enemies[k].speed;
									break;
							}
							enemies[k].hitbox = (Rectangle){ enemies[k].x - player.x, enemies[k].y+22 - player.y, 30, 8 };
							if (CheckCollisionRecs(enemies[k].hitbox, tile_hitboxes[i][j]) == true) {
								enemies[k] = CollideEnemyWithTile(enemies[k].hitbox, tile_hitboxes[i][j], enemies[k]);
							}
						}
						if (CheckCollisionRecs(player.hitbox, tile_hitboxes[i][j]) == true) {
							player = CollidePlayerWithTile(player.hitbox, tile_hitboxes[i][j], player);
						}
					}
				}
				player.x += player.velocity[0];
				player.y += player.velocity[1];
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
			rotation = atan2f(215 - GetMouseY(), 435 - GetMouseX()) * 180 / PI + 225;
			DrawTexturePro(james_images, (Rectangle){ 37 * player.sprite, 54 * player.facing, 37, 54 }, player.hitbox, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			for (int t = 0; t < 200; t++) {
				DrawTexturePro(meat_bullet, (Rectangle){ 0, 0, 30, 30 }, (Rectangle){ projectiles[t].position.x - 15, projectiles[t].position.y - 15, 30, 30 }, (Vector2){ 15, 15 }, projectiles[t].rotation, RAYWHITE);

				projectiles[t].position.x += projectiles[t].velocity.x;
				projectiles[t].position.y += projectiles[t].velocity.y;
			}
			DrawTexturePro(shotgun, (Rectangle){ 0, 0, 30, 30 }, (Rectangle){ 420, 200, 30, 30 }, (Vector2){ 15, 15 }, rotation, RAYWHITE);
			for (int i = 0; i < 20; i++) {
				DrawTexturePro(snake_images, (Rectangle){ 15 * enemies[i].sprite, 15 * enemies[i].facing, 15, 15 }, (Rectangle){ enemies[i].x - player.x, enemies[i].y - player.y, 30, 30 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
				enemies[i] = AnimateEnemy(enemies[i]);
			}
			for (int i = 0; i < 3; i++) {
				if (animate_health[i] == true) {
					health_anim[i]++;
					if (health_anim[i] == 2) {
						health_frame[i]++;
						health_anim[i] = 0;
						if (health_frame[i] > 2) {
							health_anim[i] = 0;
							health_frame[i] = 0;
							animate_health[i] = false;
						}
					}
				}
			}
			DrawTexturePro(healthbar, (Rectangle){ 0, 0, 85, 30 }, (Rectangle){ 2, 2, 170, 60 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			DrawTextEx(font, TextFormat("%d", hp/100%10), (Vector2){ 32, 17 }, 32, 0.0f, BLACK);
			DrawTextEx(font, TextFormat("%d", hp/10%10), (Vector2){ 56, 17}, 32, 0.0f, BLACK);
			DrawTextEx(font, TextFormat("%d", hp%10), (Vector2){ 80, 17}, 32, 0.0f, BLACK);
			if (IsKeyPressed(KEY_R)) {
				ammo = max_ammo;
			}
			if (IsKeyPressed(KEY_E)) {
				for (int i = 0; i < 3; i++) {
					int new_hp = hp + 25;
					if (new_hp/100%10 != hp/100%10) {
						animate_health[2] = true;
					}
					if (new_hp/10%10 != hp/10%10) {
						animate_health[1] = true;
					}
					if (new_hp%10 != hp%10) {
						animate_health[0] = true;
					}
				}
				hp += 25;
			}
			if (hp < 0) {
				hp = 0;
			}
			if (animate_health[0] == true) {
				DrawTexturePro(healthbar, (Rectangle){ 85+health_frame[0]*85, 0, 85, 30 }, (Rectangle){ 2.0f, 2.0f, 170, 60 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			}
			if (animate_health[1] == true) {
				DrawTexturePro(healthbar, (Rectangle){ 85+health_frame[1]*85, 0, 85, 30 }, (Rectangle){ -22.0f, 2.0f, 170, 60 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			}
			if (animate_health[2] == true) {
				DrawTexturePro(healthbar, (Rectangle){ 85+health_frame[2]*85, 0, 85, 30 }, (Rectangle){ -46.0f, 2.0f, 170, 60 }, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
			}
			DrawTexture(hotbar, 1.0f, GetScreenHeight() - 1, RAYWHITE);
			DrawFPS(780, 10);
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
	healthbar = LoadTexture("data/images/healthbar.png");
	axe = LoadTexture("data/images/item_axe.png");
	shotgun = LoadTexture("data/images/item_shotgun.png");
	bullet = LoadTexture("data/images/bullet.png");
	meat_bullet = LoadTexture("data/images/meat_bullet.png");
}

void UnloadTextures(void) {
	UnloadTexture(tile_grass);
	UnloadTexture(tile_water);
	UnloadTexture(tile_img);
	UnloadTexture(james_images);
	UnloadTexture(snake_images);
	UnloadTexture(menu_images);
	UnloadTexture(hotbar);
	UnloadTexture(healthbar);
	UnloadTexture(axe);
	UnloadTexture(shotgun);
	UnloadTexture(bullet);
	UnloadTexture(meat_bullet);
}

void UpdateDrawFrame(void) {
	
}

Map LoadMap (const char * filename) {
	FILE *file = fopen(filename, "r");

	if (file == NULL) {
		fprintf(stderr, "Could not find map '%s'; does it exist?", filename);
		exit(EXIT_FAILURE);
	}

	int rows, cols;
	fscanf(file, "%d %d", &rows, &cols);

	int ** array = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		array[i] = (int*)malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++) {
			fscanf(file, "%d", &array[i][j]);
		}
	}
	fclose(file);
	Map result = { array, rows, cols };
	return result;
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

Player CollidePlayerWithTile(const Rectangle entity, const Rectangle tile, Player player) {
	Rectangle up = { entity.x, entity.y, entity.width, 2 };
	Rectangle left = { entity.x, entity.y, 2, entity.height };
	Rectangle down = { entity.x, entity.y + entity.height, entity.width, 2 };
	Rectangle right = { entity.x + entity.width, entity.y, 2, entity.height };
	if (CheckCollisionRecs(up, tile) == true) {
		player.y += player.speed;
	}
	if (CheckCollisionRecs(down, tile) == true) {
		player.y -= player.speed;
	}
	if (CheckCollisionRecs(left, tile) == true) {
		player.x += player.speed;
	}
	if (CheckCollisionRecs(right, tile) == true) {
		player.x -= player.speed;
	}
	return player;
}

Player CollidePlayerWithEnemy(const Rectangle entity, const Rectangle entity2, Player player, struct obj_enemy enemy) {
	//Rectangle up = { entity.x, entity.y, entity.width, 2 };
	Rectangle left = { entity.x, entity.y, 2, entity.height };
	//Rectangle down = { entity.x, entity.y + entity.height, entity.width, 2 };
	//Rectangle right = { entity.x + entity.width, entity.y, 2, entity.height };
	if (CheckCollisionRecs(left, entity2) == true) {
		player.x += 2;
	}
	return player;
}

struct obj_enemy CollideEnemyWithTile(const Rectangle entity, const Rectangle tile, struct obj_enemy enemy) {
	// Rectangle up = { entity.x, entity.y, entity.width, 2 };
	Rectangle left = { entity.x, entity.y, 2, entity.height };
	// Rectangle down = { entity.x, entity.y + entity.height, entity.width, 2 };
	Rectangle right = { entity.x + entity.width, entity.y, 2, entity.height };
	if (CheckCollisionRecs(left, tile) == true) {
		enemy.x += enemy.speed * 2;
		enemy.facing = 1;
	}
	if (CheckCollisionRecs(right, tile) == true) {
		enemy.x -= enemy.speed * 2;
		enemy.facing = 0;
	}
	return enemy;
}

struct SaveData WriteSave(const char* filename) {
	
}
