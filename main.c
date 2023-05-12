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

	Texture tile_img;

	while (!WindowShouldClose()) {
		UpdateMusicStream(song);
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
					DrawTextureEx(tile_img, (Vector2){ j*30, i*30 }, 0.0f, 2.0f, RAYWHITE);
				}
			}
			DrawFPS(10, 10);
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

}

void UnloadTextures(void) {

}

void UpdateDrawFrame(void) {
	
}
