#include "raylib.h"

typedef struct {
	int** data;
	int rows;
	int cols;
} Map;

Map LoadMap(const char* filename);
Player CollidePlayerWithTile(Map map, Player player);
void DrawMap(Map map, Texture* tiles, Player player);