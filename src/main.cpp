/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera mode
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <cmath>
#include <algorithm>
#include "raylib.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

glm::vec3 to(Vector3 v) {
	return { v.x, v.y, v.z };
}
Vector3 to(glm::vec3 v) {
	return { v.x, v.y, v.z };
}

//
void BlenderLikeCameraUpdate(Camera *camera) {
	Vector2 mousePositionDelta = { 0.0f, 0.0f };
	{
		Vector2 mousePosition = GetMousePosition();
		static Vector2 previousMousePosition = { 0.0f, 0.0f };
		mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
		mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
		previousMousePosition = mousePosition;
	}


	float wheel_sensitivity = 0.2f;
	float zoom_mouse_sensitivity = 0.002f;
	float rotate_sensitivity = 0.005f;
	float move_sensitivity = 0.0007f;

	bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
	
	int deltaWheel = GetMouseWheelMove();
	if (deltaWheel != 0) {
		auto target   = to(camera->target);
		auto position = to(camera->position);

		float d = glm::distance(target, position);
		d -= d * deltaWheel * wheel_sensitivity;
		d = std::max(0.1f, d);

		auto dir = glm::normalize(position - target);
		position = target + dir * d;
		camera->position = to(position);
	}
	else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
		if (mousePositionDelta.y != 0.0f) {
			auto target = to(camera->target);
			auto position = to(camera->position);

			float d = glm::distance(target, position);
			d -= d * mousePositionDelta.y * zoom_mouse_sensitivity;
			d = std::max(0.1f, d);

			auto dir = glm::normalize(position - target);
			position = target + dir * d;
			camera->position = to(position);
		}
	}

	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && shift == false) {
		if (mousePositionDelta.x != 0.0f) {
			auto target = to(camera->target);
			auto position = to(camera->position);
			auto up = to(camera->up);
			auto S = position - target;

			S  = glm::rotateY(S, -rotate_sensitivity * mousePositionDelta.x);
			up = glm::rotateY(up, -rotate_sensitivity * mousePositionDelta.x);

			camera->position = to(target + S);
			camera->up = to(up);
		}

		if (mousePositionDelta.y != 0.0f) {
			auto target = to(camera->target);
			auto position = to(camera->position);

			auto foward = position - target;

			auto up = to(camera->up);
			if (glm::length(up) <= glm::epsilon<float>()) {
				up = glm::vec3(0, 1, 0);
			} 
			auto right = glm::normalize(glm::cross(foward, up));
			up = glm::cross(right, foward);
			up = glm::normalize(up);

			auto S = position - target;
			S = glm::rotate(S, rotate_sensitivity * mousePositionDelta.y, right);
			camera->position = to(target + S);
			camera->up = to(up);
		}
	}

	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && shift) {
		auto target = to(camera->target);
		auto position = to(camera->position);

		auto foward = position - target;

		auto up = to(camera->up);
		if (glm::length(up) <= glm::epsilon<float>()) {
			up = glm::vec3(0, 1, 0);
		}
		auto right = glm::normalize(glm::cross(foward, up));
		up = glm::cross(right, foward);
		up = glm::normalize(up);

		float d = glm::distance(target, position);

		auto move = 
			d * up * move_sensitivity * mousePositionDelta.y +
			d * right * move_sensitivity * mousePositionDelta.x;

		target += move;
		position += move;

		camera->target = to(target);
		camera->position = to(position);
	}
}
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Blender Like Camera");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3 { 10.0f, 10.0f, 10.0f };  // Camera position
	camera.target = Vector3 { 0, 0, 0};      // Camera looking at point
	camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FREE);

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		BlenderLikeCameraUpdate(&camera);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
		DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

		DrawGrid(10, 1.0f);

		EndMode3D();

		DrawFPS(10, 10);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}