#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "tiles.h"

// define miscellaneous things vital to the game's functionality
#define inventory_size 16
#define menu_selection 0
#define menu "nexxus"
#define fade "in"
#define fade_frame 0
#define ambient_colour RAYWHITE

// screen settings
const int screenWidth = 800;
const int screenHeight = 450;

// ensure that all resources are pre-defined so that gcc doesn't have a mental breakdown
Texture tile_grass; // BAD! Rendering each tile one by one will hurt raylib's performance!
Texture tile_water; // TODO: Make a texture atlas of each tile image.
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
	int facing;
	int anim;
	int sprite;
	bool running;
	int stats[10]; 		 // hp, max hp, hunger, max hunger, thirst, max thirst, stamina, max stamina, action points, max action points
	int combat_stats[8]; // melee damage, melee defense, ranged damage, ranged defense, magic damage, magic defense, explosive damage, explosive defense
} Player;

typedef struct {
	int age; 	  // in-game days
	int * stages; // points at which the plant will age
	int lifespan; // point at which it will die if its age reaches this
	char * name;  // self-explanatory
} obj_plant;

// obj_plant obj_appletree = { 84, { 28, 56, 84 }, 18500, "Apple Tree" };

typedef struct {
	char * name;
	char * description;
	char * summary;
	int img;
	char * type;
} Item;

// Item log = { "Log", "It's a log; what were you expecting?", "Basic crafting material.", 0, "material" };

void LoadTextures(void);
void UnloadTextures(void);
void UpdateDrawFrame(void);
void AnimatePlayer(Player player);

char * displaymsg = "Log";
char * displaymsg2 = "It's a log; what were you expecting?";
char * displaymsg3 = "Basic crafting material.";

int main(void) {

	Map map = LoadMap("data/maps/map1.txt");
	Player player = { 0, 0, 0.5, 2, 0, 0, false, { 90, 90, 72, 72, 72, 72, 75, 75, 10, 10 }, {1.5, 1.25, 1.0, 0.75, 0.75, 1.0}};

	InitWindow(screenWidth, screenHeight, "The Last Utopia");
	InitAudioDevice();
	SetTargetFPS(60);

	LoadTextures();

	Music boomtown = LoadMusicStream("data/music/boomtown.ogg");
	Music song = boomtown;
	PlayMusicStream(song);

	Font font1 = LoadFont("data/terminal.ttf");

	int text_frames = 0;

	while (!WindowShouldClose()) {
		text_frames++;
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
			// DrawTextureEx(james_images, (Vector2){ 0, 0 }, 0.0f, 1.0f, RAYWHITE);
			DrawTexture(hotbar, 1.0f, 419.0f, RAYWHITE);
			DrawTexturePro(menu_images, (Rectangle){ 0.0f + 400.0f * menu_selection, 0.0f, 400.0f, 225.0f }, (Rectangle){ 0.0f, 0.0f, 800.0f, 450.0f }, (Vector2){ 0.0f, 0.0f }, 0.0f, RAYWHITE);
			DrawFPS(10, 10);
			// DrawTextEx(font1, TextSubtext(displaymsg, 0, text_frames/10), (Vector2){ 25.0f, 100.0f }, 16, 0, RAYWHITE);
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
}

void UpdateDrawFrame(void) {
	
}

void AnimatePlayer(Player player) {
	player.anim++;
	if (player.anim > 7) {
		player.anim = 0;
		player.sprite++;
		if (player.sprite >= 8) {
			player.sprite = 0;
		}
	}
}