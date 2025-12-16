#pragma once
#include "raylib.h"
class ModuleGame;
class Snail;

class UIGameOver{
public:
	UIGameOver(ModuleGame* game);
	void DrawGameOver();
	void UpdateGameOver();
private:
	ModuleGame* game;
	Sound finishFx;
	bool hasPlayed = false;
};