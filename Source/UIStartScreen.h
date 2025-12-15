#pragma once
class ModuleGame;
class Snail;

class UIStartScreen {
public:
	UIStartScreen(ModuleGame* game);
	void DrawStartScreen();
	void UpdateStartScreen();
private:
	ModuleGame* game;
};
