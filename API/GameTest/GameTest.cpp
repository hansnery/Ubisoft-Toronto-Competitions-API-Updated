//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
const float CHARACTER_SPEED = 5.0f;
const float GROUND_HEIGHT = 400.f;
const float JUMP_STRENGTH = 15.0f;
const float GRAVITY = 1.0f;
const float MAX_VERTICAL_VELOCITY = 15.0f;
const float HERO_ACCELERATION = 0.12f;
const float HERO_DECELERATION = 0.1f;
const float LANDING_DURATION = 2.0f;
bool isJumping = false;
float verticalVelocity = 0.0f;
float landingTimer = 0.0f;
//------------------------------------------------------------------------
// Animations
//------------------------------------------------------------------------
CSimpleSprite* hero;
enum
{
	ANIM_IDLE,
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
	ANIM_JUMP,
	ANIM_LAND,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Create hero's sprite
	hero = App::CreateSprite(".\\TestData\\player-spritemap-v9.png", 8, 4);
	hero->SetPosition(400.0f, GROUND_HEIGHT);
	// Create hero's animations
	float speed = 1.0f / 15.0f; // Default: 1.0f / 15.0f;
	hero->CreateAnimation(ANIM_IDLE, speed, { 0 });
	//hero->CreateAnimation(ANIM_BACKWARDS, speed, { 8,9,10,11 });
	hero->CreateAnimation(ANIM_LEFT, speed, { 56,57,58,59,60,61,62,63 });
	hero->CreateAnimation(ANIM_RIGHT, speed, { 24,25,26,27,28,29,30,31 });
	hero->CreateAnimation(ANIM_FORWARDS, speed, { 13 });
	//hero->CreateAnimation(ANIM_JUMP, speed, { 13,13,13,13,13,13,1, });
	hero->CreateAnimation(ANIM_JUMP, speed, { 13 });
	hero->CreateAnimation(ANIM_LAND, speed, { 1 });
	hero->SetScale(1.5f);
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Update the sprite
	hero->Update(deltaTime);

	// Get position of sprite
	float x, y;
	hero->GetPosition(x, y);

	// Static variable to maintain horizontal speed across frames
	static float horizontalSpeed = 0.0f;
	const float acceleration = HERO_ACCELERATION;
	const float deceleration = HERO_DECELERATION;

	// Handle horizontal movement
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		// Gradual increase in speed to the right
		horizontalSpeed += acceleration;
		if (horizontalSpeed > CHARACTER_SPEED) {
			horizontalSpeed = CHARACTER_SPEED;
		}
		if (!isJumping) {
			hero->SetAnimation(ANIM_RIGHT);
			hero->SetFlip(false, false);
		}
	}
	else if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		// Gradual increase in speed to the left
		horizontalSpeed -= acceleration;
		if (horizontalSpeed < -CHARACTER_SPEED) {
			horizontalSpeed = -CHARACTER_SPEED;
		}
		if (!isJumping) {
			hero->SetAnimation(ANIM_RIGHT);
			hero->SetFlip(true, false);
		}
	}
	else
	{
		// Gradual reduction in speed (deceleration)
		if (horizontalSpeed != 0) {
			if (horizontalSpeed > 0) {
				horizontalSpeed -= deceleration;
				if (horizontalSpeed < 0) horizontalSpeed = 0;
			}
			else {
				horizontalSpeed += deceleration;
				if (horizontalSpeed > 0) horizontalSpeed = 0;
			}
		}

		if (!isJumping && horizontalSpeed == 0) {
			hero->SetAnimation(ANIM_IDLE);
		}
	}

	// Handle jump initiation
	if (App::GetController().GetLeftThumbStickY() > 0.5f && !isJumping)
	{
		isJumping = true;
		verticalVelocity = JUMP_STRENGTH;
		hero->SetAnimation(ANIM_JUMP);
	}

	// Handle jumping mechanics
	if (isJumping)
	{
		y += verticalVelocity;
		verticalVelocity -= GRAVITY;

		if (verticalVelocity < -MAX_VERTICAL_VELOCITY) {
			verticalVelocity = -MAX_VERTICAL_VELOCITY;
		}

		if (y <= GROUND_HEIGHT) {
			y = GROUND_HEIGHT;
			isJumping = false;
			verticalVelocity = 0.0f;
			hero->SetAnimation(ANIM_LAND);
		}
	}

	// Apply horizontal movement
	x += horizontalSpeed;

	// Update hero's position
	hero->SetPosition(x, y);
	
	/*if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		testSprite->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		testSprite->GetPosition(x, y);
		y -= CHARACTER_SPEED;
		testSprite->SetPosition(x, y);
	}*/
	//// Up Arrow
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	//{
	//	hero->SetScale(hero->GetScale() + 0.1f);
	//}
	//// Down Arrow
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	//{
	//	hero->SetScale(hero->GetScale() - 0.1f);
	//}
	//// Left Arrow
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	//{
	//	hero->SetAngle(hero->GetAngle() + 0.1f);
	//}
	//// Right Arrow
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	//{
	//	hero->SetAngle(hero->GetAngle() - 0.1f);
	//}
	//// .
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	//{
	//	hero->SetAnimation(-1);
	//}
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	// Number 1
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	hero->Draw();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(840, 740, "1. Kill all alien insects.");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	/*static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey,r,g,b);
	}*/
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	//------------------------------------------------------------------------
	delete hero;
	//------------------------------------------------------------------------
}