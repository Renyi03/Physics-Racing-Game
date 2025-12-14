#pragma once
#include "raylib.h"
class ModuleGame;
enum class SnailType;

class UISnailSelect {
public:
	UISnailSelect(ModuleGame* game);
	void DrawSnailSelect();
	void UpdateSnailSelect();
	void ChooseSnail(SnailType type);
private:
	ModuleGame* game;
	// Snail preview textures (UI only)
	Texture2D previewEnhypen;
	Texture2D previewChopin;
	Texture2D previewAdo;
	Texture2D previewMiku;

	bool texturesLoaded = false;
};
