#include "ChopinSnail.h"

bool ChopinSnail::Start()
{
	chopinSnailTexture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	if (chopinSnailTexture.id == 0) {
		TraceLog(LOG_INFO, "Chopin Texutre not lodaded");
		return false;
	}
	SetTexture(chopinSnailTexture);
	return true;
}

bool ChopinSnail::CleanUp()
{
	UnloadTexture(chopinSnailTexture);
	return false;
}

void ChopinSnail::Update()
{
	Snail::Update();
}
