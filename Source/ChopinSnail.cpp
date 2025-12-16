#include "ChopinSnail.h"

bool ChopinSnail::Start()
{
	flyFx = LoadSound("Assets/Audio/Fly.wav");
	flyTexture = LoadTexture("Assets/Textures/Chopin_Snail_Ability.png");
	texture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	salivaTexture = LoadTexture("Assets/Textures/ChopinBaba.png");
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
	moveForce = 1.7f; 
	rotation_base_rate = 2.5f; 
	staticFrictionCoeff = 0.4f; 
	dynamicFrictionCoeff = 0.25f;
	lateralDamping = 0.18f;
}

bool ChopinSnail::CleanUp()
{
	if (texture.id != 0) {
		UnloadTexture(texture);
		texture = Texture2D{};
	}
	if (salivaTexture.id != 0) {
		UnloadTexture(salivaTexture);
	}
	if (flyTexture.id != 0) {
		UnloadTexture(flyTexture);
	}

	if (flyFx.frameCount > 0) {
		UnloadSound(flyFx);
	}

	return true;
}

void ChopinSnail::Update()
{
	Snail::Update();
	if (active) {
		UseAbility();
		if (isFlying) {
			SetTexture(flyTexture);
		}
		else {
			SetTexture(texture);
		}
	}
}

void ChopinSnail::UseAbility() {
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
		PlaySound(flyFx);
		isFlying = true;
		flyingDuration = 10.0f;
		abilityTimer = 12.0;
		uint16 flyingMask = PhysicCategory::ALL - PhysicCategory::SNAIL_CATEGORY;
		SetMaskBits(flyingMask);

		TraceLog(LOG_INFO, "Chopin is flying");
	}
}