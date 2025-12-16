#include "UIStartScreen.h"
#include "ModuleGame.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

UIStartScreen::UIStartScreen(ModuleGame* g) : game(g) {}

void UIStartScreen::UpdateStartScreen()
{
	if (IsKeyPressed(KEY_ENTER))
		game->gameState = GameState::SNAIL_SELECT;

	if (IsKeyPressed(KEY_ESCAPE))
		game->App->window->RequestClose();
}

void UIStartScreen::DrawStartScreen()
{
	DrawText("T H E   S N A I L E S T   R A C E   2",
		SCREEN_WIDTH / 2 - MeasureText("T H E   S N A I L E S T   R A C E   2", 48) / 2,
		100, 48, DARKGREEN);

	DrawText("PRESS ENTER TO START",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO START", 20) / 2,
		300, 20, LIGHTGRAY);

	DrawText("PRESS ESC TO EXIT",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ESC TO EXIT", 20) / 2,
		340, 20, LIGHTGRAY);
}