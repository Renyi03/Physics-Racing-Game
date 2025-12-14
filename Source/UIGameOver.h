#pragma once
class ModuleGame;
class Snail;

class UIGameOver{
public:
	UIGameOver(ModuleGame* game);
	void DrawGameOver();
	void UpdateGameOver();
private:
	ModuleGame* game;
};