#include "raylib.h"

typedef enum Directions { UP, LEFT, DOWN, RIGHT } Directions;

typedef struct {
	Vector2 position;
	Vector2 velocity;
	float speed;
	int facing;
	int frame;
	int sprite;
} Player;

Player MovePlayer(Player player);
Player AnimatePlayer(Player player);