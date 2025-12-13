#include "AdoSnail.h"

bool AdoSnail::Start()
{
	adoSnailTexture = LoadTexture("Assets/Textures/Ado_Snail.png");
	if (adoSnailTexture.id == 0) {
		TraceLog(LOG_INFO, "Ado Texutre not lodaded");
		return false;
	}
	SetTexture(adoSnailTexture);
	return true;
}

bool AdoSnail::CleanUp()
{
	UnloadTexture(adoSnailTexture);
	return false;
}

void AdoSnail::Update()
{
	Snail::Update();
}