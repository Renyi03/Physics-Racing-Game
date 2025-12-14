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
	}
	
}

bool MikuSnail::Start()
{
	salivaTexture = LoadTexture("Assets/Textures/Car.png");
	texture = LoadTexture("Assets/Textures/Miku_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Miku Texutre not lodaded");
		return false;
	}
	SetTexture(texture);

	return true;
}

bool MikuSnail::CleanUp()
{
	UnloadTexture(texture);
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