#pragma once
class ModuleGame;
class Snail;

class UISnailSelect {
public:
	UISnailSelect(ModuleGame* game);
	void DrawSnailSelect();
	void UpdateSnailSelect();
	void ChooseSnail(Snail* chosen);
private:
	ModuleGame* game;
};