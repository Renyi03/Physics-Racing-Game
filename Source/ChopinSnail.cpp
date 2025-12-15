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
}
