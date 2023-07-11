#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "player.h"
#include "tiles.h"

Map LoadMap(const char* filename) {
	// Open map file for reading
	FILE *file = fopen(filename, "r");

	// Print debug output if file is missing
	if (file == NULL) {
		fprintf(stderr, "Could not find map '%s'; does it exist?", filename);
		exit(EXIT_FAILURE);
	}

	// Read first two integers on line 1 in map file to the dimensions of the map
	int rows, cols;
	fscanf(file, "%d %d", &cols, &rows);

	// Read the rest of the file into a 2D array of integers
	int** array = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		array[i] = (int*)malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++) {
			fscanf(file, "%d", &array[i][j]);
		}
	}

	// Close map file and return the map
	fclose(file);
	Map result = { array, rows, cols };
	return result;
}

void DrawMap(Map map, Texture* tiles, Player player) {
	for (int i = 0; i < map.rows; i++) {
		for (int j = 0; j < map.cols; j++) {
			DrawTextureEx(tiles[map.data[i][j]], (Vector2){ j * 30 - player.position.x, i * 30 - player.position.y }, 0.0f, 2.0f, RAYWHITE);
		}
	}
}