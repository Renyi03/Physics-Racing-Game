#include "AdoSnail.h"

bool AdoSnail::Start()
{
	salivaTexture = LoadTexture("Assets/Textures/Car.png");
	texture = LoadTexture("Assets/Textures/Ado_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Ado Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	ApplySnailStats();
	return true;
}

void AdoSnail::ApplySnailStats()
{
	// ADO: Balanced (default stats)
	moveForce = 1.7f;
	rotation_base_rate = 3.0f;
	staticFrictionCoeff = 0.5f;
	dynamicFrictionCoeff = 0.3f;
	lateralDamping = 0.2f;
}

bool AdoSnail::CleanUp()
{
	UnloadTexture(texture);
	return false;
}

void AdoSnail::Update()
{
	Snail::Update();
}