#include "MikuSnail.h"
#include "ModulePhysics.h"
#include "Box.h"
#include "Snail.h"
#include "PhysicEntity.h"

void MikuSnail::UseAbility() {
	body->body->ApplyLinearImpulseToCenter( 2*forward, true);
}

bool MikuSnail::Start()
{
	mikuSnailTexture = LoadTexture("Assets/Textures/Miku_Snail.png");
	if (mikuSnailTexture.id == 0) {
		TraceLog(LOG_INFO, "Miku Texutre not lodaded");
		return false;
	}
	SetTexture(mikuSnailTexture);
	return true;
}

bool MikuSnail::CleanUp()
{
	UnloadTexture(mikuSnailTexture);
	return true;
}

void MikuSnail::Update()
{
	Snail::Update();

	if (IsKeyPressed(KEY_E)) {
		UseAbility();
	}
}