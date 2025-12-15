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
	Snail::Update();
}
