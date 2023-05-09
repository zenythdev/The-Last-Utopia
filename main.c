#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "tiles.h"

#define menu_selection 0
#define ambient_colour RAYWHITE

const int screenWidth = 800;
const int screenHeight = 450;

void LoadTextures(void);
void UnloadTextures(void);
void UpdateDrawFrame(void);
void AnimatePlayer(void);
Texture player_textures[4][8] = {};
Texture snake_textures[2][4] = {};
Texture menu_backgrounds[5] = {};
Texture tile_images[] = {};
int menu_fade = 0;
char * fade = "in";
char * menu = "nexxus";

typedef struct {
	int x;
	int y;
	float speed;
	int facing;
	int anim;
} Player;

int main(void) {

	Map map = LoadMap("data/maps/map1.txt");
	Player player = { 0, 0, 0.5, 2, 3 };

	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	LoadTextures();

	Music boomtown = LoadMusicStream("data/music/boomtown.ogg");
	Music song = boomtown;
	PlayMusicStream(song);

	Texture tile_grass = LoadTexture("data/images/tile_grass.png");
	Texture tile_water = LoadTexture("data/images/tile_water.png");
	Texture nexxus_games = LoadTexture("data/images/nexxus_games.png");

	while (!WindowShouldClose()) {
		UpdateMusicStream(song);
		BeginDrawing();
			ClearBackground(RAYWHITE);
			for (int i = 0; i < map.rows; i++) {
				for (int j = 0; j < map.cols; j++) {
					switch (map.data[i][j]) {
						case 1:
							DrawTextureEx(tile_grass, (Vector2){ j*30, i*30 }, 0.0f, 2.0f, RAYWHITE);
							break;
						case 0:
							DrawTextureEx(tile_water, (Vector2){ j*30, i*30 }, 0.0f, 2.0f, RAYWHITE);
							break;
					}
				}
			}
			DrawFPS(10, 10);
			DrawTexture(player_textures[player.facing][player.anim], 400, 225, RAYWHITE);
			//if (menu == "nexxus") {
			//	DrawTexture(nexxus_games, 0, 0, (Color){ menu_fade, menu_fade, menu_fade, 255});
			//	if (menu_fade < 255) {
			//		if (fade == "in") menu_fade += 1;
			//	} else fade = "out";
			//	if (menu_fade > 0) { 
			//		if (fade == "out") menu_fade -= 1;
			//	} else menu = "game";
			// }
		EndDrawing();
	}
	UnloadTextures();
	CloseWindow();
	return 0;
}

void LoadTextures(void) {
	char player_texture_names[4][8][15] = { { "ju1.png", "ju2.png", "ju3.png", "ju4.png", "ju5.png", "ju6.png", "ju7.png", "ju8.png" }, { "jl1.png", "jl2.png", "jl3.png", "jl4.png", "jl5.png", "jl6.png", "jl7.png", "jl8.png" }, { "jd1.png", "jd2.png", "jd3.png", "jd4.png", "jd5.png", "jd6.png", "jd7.png", "jd8.png" }, { "jr1.png", "jr2.png", "jr3.png", "jr4.png", "jr5.png", "jr6.png", "jr7.png", "jr8.png" } };
	char snake_texture_names[2][4][15] = { { "snakel1.png", "snakel2.png", "snakel3.png", "snakel4.png" }, { "snaker1.png", "snaker2.png", "snaker3.png", "snaker4.png" } };
	for (int i = 0; i < sizeof(player_texture_names) / sizeof(player_texture_names[0]); i++) {
		for (int j = 0; j < sizeof(player_texture_names[0]) / sizeof(player_texture_names[0][0]); j++) {
			char image_path[27] = "data/images/";
			strcat(image_path, player_texture_names[i][j]);
			player_textures[i][j] = LoadTexture(image_path);
		}
	}
	for (int i = 0; i < sizeof(snake_texture_names) / sizeof(snake_texture_names[0]); i++) {
		for (int j = 0; j < sizeof(snake_texture_names[0]) / sizeof(snake_texture_names[0][0]); j++) {
			char image_path[27] = "data/images/";
			strcat(image_path, snake_texture_names[i][j]);
			snake_textures[i][j] = LoadTexture(image_path);
		}
	}
	Texture menu_backgrounds[5] = { LoadTexture("data/images/menu_items.png"), LoadTexture("data/images/menu_equips.png"), LoadTexture("data/images/menu_journal.png"), LoadTexture("data/images/menu_crafting.png"), LoadTexture("data/images/menu_settings.png") };
}

void UnloadTextures(void) {
	for (int i = 0; i < sizeof(player_textures) / sizeof(player_textures[0]); i++) {
		for (int j = 0; j < sizeof(player_textures[0]) / sizeof(player_textures[0][0]); j++) {
			UnloadTexture(player_textures[i][j]);
		}
	}
	for (int i = 0; i < sizeof(menu_backgrounds) / sizeof(menu_backgrounds[0]); i++) UnloadTexture(menu_backgrounds[i]);
}

void UpdateDrawFrame(void) {
	
}
