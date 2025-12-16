#include "MikuSnail.h"
#include "ModulePhysics.h"
#include "Box.h"
#include "Snail.h"
#include "PhysicEntity.h"

void MikuSnail::UseAbility() {
	if (abilityTimer <= 0.0f) {
		body->body->ApplyLinearImpulseToCenter(2 * forward, true);
		abilityTimer = 9.0f;
		TraceLog(LOG_INFO, "Used ability");
		PlaySound(sprintFx);
	}
}

bool MikuSnail::Start()
{
	sprintFx = LoadSound("Assets/Audio/Sprint.wav");
	salivaTexture = LoadTexture("Assets/Textures/Car.png");
	texture = LoadTexture("Assets/Textures/Miku_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Miku Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	ApplySnailStats();

	return true;
}

void MikuSnail::ApplySnailStats()
{
	// MIKU: High acceleration, average speed
	moveForce = 1.65f;                // Good acceleration
	rotation_base_rate = 3.2f;       // Good turning
	staticFrictionCoeff = 0.5f;      // Low friction = quick start
	dynamicFrictionCoeff = 0.3f;     // High friction = lower top speed
	lateralDamping = 0.22f;
}

bool MikuSnail::CleanUp()
{
	if (texture.id != 0) {
		UnloadTexture(texture);
		texture = Texture2D{};
	}

	if (sprintFx.frameCount > 0) {
		UnloadSound(sprintFx);
	}
	return true;
}

void MikuSnail::Update()
{
	Snail::Update();
	if (active) {
		abilityTimer -= GetFrameTime();
		if (IsKeyPressed(KEY_E)) {
			UseAbility();
		}
	}
}