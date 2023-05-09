#ifndef TILES_H
#define TILES_H

typedef struct {
	int ** data;
	int rows;
	int cols;
} Map;

Map LoadMap (const char * filename);

#endif
