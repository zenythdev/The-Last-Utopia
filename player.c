#include "raylib.h"
#include "player.h"

Player AnimatePlayer(Player player) {
	// Check if player is moving
	if (IsKeyDown(KEY_W)) {
		if (player.velocity.y <= 0) {
			player.velocity.y -= player.speed;
		} else {
			player.velocity.y = 0;
		}
		player.facing = UP;
	}
	if (IsKeyDown(KEY_A)) {
		if (player.velocity.x <= 0) {
			player.velocity.x -= player.speed;
		} else {
			player.velocity.x = 0;
		}
		player.facing = LEFT;
	}
	if (IsKeyDown(KEY_S)) {
		if (player.velocity.y >= 0) {
			player.velocity.y += player.speed;
		} else {
			player.velocity.y = 0;
		}
		player.facing = DOWN;
	}
	if (IsKeyDown(KEY_D)) {
		if (player.velocity.x >= 0) {
			player.velocity.x += player.speed;
		} else {
			player.velocity.x = 0;
		}
		player.facing = RIGHT;
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
		player.frame++;
	} else {
		player.sprite = 0;
		player.velocity.x = 0;
		player.velocity.y = 0;
	}

	// Increment player sprite
	if (player.frame >= 7) {
		player.frame = 0;
		player.sprite++;
	}
	if (player.sprite >= 8) {
		player.sprite = 0;
	}

	// Move player
	player.position.x += player.velocity.x;
	player.position.y += player.velocity.y;

	return player;
}