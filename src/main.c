#include "../header/controls.h"
#include "../assets/playerSprite.h"
#include <math.h>

float speed = 300;
Vector2 getMovementVector() {
	Vector2 movement = { 0.0f, 0.0f };

	if (getControls().left) {
		movement.x -= 1.0f;
	}

	if (getControls().right) {
		movement.x += 1.0f;
	}

	if (movement.x != 0 || movement.y != 0) {
		float length = sqrtf(movement.x * movement.x + movement.y * movement.y);
		movement.x /= length;
		movement.y /= length;
	}

	return movement;
}

float normalX = 0;
float normalY = 0;
void handleMovement(Rectangle* player, float deltaTime) {
	Vector2 movement = getMovementVector();

	normalX = movement.x;
	normalY = movement.y;

	player->x += movement.x * speed * deltaTime;
	player->y += movement.y * speed * deltaTime;
}

void ResolveCollision(Rectangle* player, Rectangle* wall) {
	// Calculate the intersection rectangle
	float overlapX = fminf(player->x + player->width, wall->x + wall->width) - fmaxf(player->x, wall->x);
	float overlapY = fminf(player->y + player->height, wall->y + wall->height) - fmaxf(player->y, wall->y);

	// Determine which direction has the smallest overlap
	if (overlapX < overlapY) {
		// Resolve collision along the X axis
		if (player->x < wall->x) {
			// Player is to the left of the wall
			player->x = wall->x - player->width;
		}
		if (player->x > wall->x) {
			// Player is to the right of the wall
			player->x = wall->x + wall->width;
		}
	}
	if (overlapX > overlapY) {
		// Resolve collision along the Y axis
		if (player->y < wall->y) {
			// Player is above the wall
			player->y = wall->y - player->height;
		}
		if (player->y > wall->y) {
			// Player is below the wall
			player->y = wall->y + wall->height;
		}
	}
}

static bool canJump = true;
void playerHop(float deltaTime, Rectangle *player) {
	const float jumpHeight = 100.0f;
	static bool isJumping = false;
	static float startY = 0.0f;
	float targetY = startY - jumpHeight;
	float deltaY = targetY - player->y;
	
	// Check if the space bar is pressed and the player is not already jumping
 	if (getControls().space && canJump) {
		isJumping = true;
		canJump = false;
		startY = player->y;
	}

	// If the player is jumping
	if (!isJumping) {
		// player falls
		player->y += 250.f * deltaTime;
	}

	if (isJumping) {
		targetY = startY - jumpHeight;
		deltaY = targetY - player->y;
		if ((player->y - 1) <= targetY) {
			isJumping = false;
			startY = player->y;
		}

		player-> y += deltaY * deltaTime * 18.0f;
	}

}

int main() {
	int screenWidth = 800;
	int screenHeight = 700;
	int targetFPS = 60;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Sound Test");
	SetTargetFPS(targetFPS);
	
	InitAudioDevice();
	Sound quack = LoadSound("./assets/sound.wav");

	while (!WindowShouldClose()) {
		BeginDrawing();
		
			ClearBackground((Color) { 0x17, 0x17, 0x17, 0xFF });
			if (IsKeyPressed(KEY_SPACE)) {
				PlaySound(quack);
				// SetSoundVolume(quack, 100);
			}
			DrawText("Press space to make a sound", 0, 0, 20, WHITE);
		EndDrawing();
	}

	UnloadSound(quack);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

