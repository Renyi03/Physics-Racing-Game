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
	return true;
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