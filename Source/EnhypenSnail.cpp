#include "EnhypenSnail.h"

bool EnhypenSnail::Start()
{
	texture = LoadTexture("Assets/Textures/Enhypen_Snail.png");
	if (texture.id == 0) {
		TraceLog(LOG_INFO, "Enhypen Texutre not lodaded");
		return false;
	}
	SetTexture(texture);
	return true;
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
