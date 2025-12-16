#include "EnhypenSnail.h"

bool EnhypenSnail::Start()
{
	salivaTexture = LoadTexture("Assets/Textures/Car.png");
	texture = LoadTexture("Assets/Textures/Enhypen_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Enhypen Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	ApplySnailStats();
	return true;
}

void EnhypenSnail::ApplySnailStats()
{
	// ENHYPEN: Best turning, slower speed
	moveForce = 1.6f;                // Slower (default: 1.7)
	rotation_base_rate = 3.5f;       // Better turning (default: 3.0)
	staticFrictionCoeff = 0.5f;      // More grip
	dynamicFrictionCoeff = 0.3f;
	lateralDamping = 0.25f;          // Better cornering
}

bool EnhypenSnail::CleanUp()
{
	UnloadTexture(texture);
	return true;
}

void EnhypenSnail::Update()
{
	if (active) {
		UseAbility();
	}
	Snail::Update();
}

void EnhypenSnail::UseAbility() {

	abilityCooldown -= GetFrameTime();
	if (IsKeyPressed(KEY_E) && abilityCooldown <= 0.0f) {
		projectile = new Projectile(listener->App->physics, this);
		projectile->Start();

		projectile->body->body->ApplyLinearImpulseToCenter(25.0f * forward, true);
		abilityCooldown = 10.0f;
		TraceLog(LOG_INFO, "ENHYPEN HAS THROWN A PROJECTILE!!!");
	}
	if (projectile) {
		projectile->Update();
		if (!projectile->active) {
			delete projectile;
			projectile = nullptr;
		}
	}
}