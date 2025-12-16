#include "ChopinSnail.h"

bool ChopinSnail::Start()
{
	salivaTexture = LoadTexture("Assets/Textures/Car.png");
	texture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Chopin Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	ApplySnailStats();
	return true;
}

void ChopinSnail::ApplySnailStats()
{
	// CHOPIN: Fastest, worst turning
	moveForce = 1.7f;                // Faster (default: 1.7)
	rotation_base_rate = 2.5f;       // Worse turning (default: 3.0)
	staticFrictionCoeff = 0.4f;      // Less grip
	dynamicFrictionCoeff = 0.25f;
	lateralDamping = 0.18f;          // More drift
}

bool ChopinSnail::CleanUp()
{
	UnloadTexture(texture);
	return false;
}

void ChopinSnail::Update()
{
	Snail::Update();
	if (active) {
		UseAbility();
	}
}

void ChopinSnail::UseAbility() {
	/*if (abilityTimer <= 0.0f) {
		abilityTimer = 9.0f;
		if (flyingDuration > 0.0f) {
			
			uint16 flyingMask = PhysicCategory::DEFAULT;
			SetMaskBits(flyingMask);
			TraceLog(LOG_INFO, "Chopin is flying");
		}
		else {
			flyingDuration = 3.0f;
			RestoreMaskBits();
			TraceLog(LOG_INFO, "Chopin is landing");
		}
	}*/

	if (abilityTimer > 0.0f) {
		abilityTimer -= GetFrameTime();
		if (abilityTimer < 0.0f) {
			abilityTimer = 0.0f;
		}
	}
	if (isFlying) {
		flyingDuration -= GetFrameTime();
		if (flyingDuration <= 0.0f) {
			isFlying = false;
			RestoreMaskBits();

			TraceLog(LOG_INFO, "Chopin is landing");
		}
	}
	if (IsKeyPressed(KEY_E) && abilityTimer <= 0 && !isFlying) {
		isFlying = true;
		flyingDuration = 10.0f;
		abilityTimer = 12.0;
		uint16 flyingMask = PhysicCategory::ALL - PhysicCategory::SNAIL_CATEGORY;
		SetMaskBits(flyingMask);
		TraceLog(LOG_INFO, "Chopin is flying");
	}
}