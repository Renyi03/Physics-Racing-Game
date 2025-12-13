#include "ChopinSnail.h"

bool ChopinSnail::Start()
{
	texture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Chopin Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	return true;
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
