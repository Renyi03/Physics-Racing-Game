#include "EnhypenSnail.h"

bool EnhypenSnail::Start()
{
	enhypenSnailTexture = LoadTexture("Assets/Textures/Enhypen_Snail.png");
	if (enhypenSnailTexture.id == 0) {
		TraceLog(LOG_INFO, "Enhypen Texutre not lodaded");
		return false;
	}
	SetTexture(enhypenSnailTexture);
	return true;
}

bool EnhypenSnail::CleanUp()
{
	UnloadTexture(enhypenSnailTexture);
	return true;
}

void EnhypenSnail::Update()
{
	Snail::Update();
}
