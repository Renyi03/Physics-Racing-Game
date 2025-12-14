#include "UIGameOver.h"
#include "ModuleGame.h"

UIGameOver::UIGameOver(ModuleGame* g) : game(g) {}

void UIGameOver::UpdateGameOver()
{
	if (IsKeyPressed(KEY_ENTER))
	{
		game->ResetRace();
		game->gameState = GameState::START_SCREEN;
	}
}

void UIGameOver::DrawGameOver()
{
	DrawText("FINISH!",
		SCREEN_WIDTH / 2 - MeasureText("FINISH!", 48) / 2,
		200, 48, RAYWHITE);

	DrawText("PRESS ENTER TO RETURN",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RETURN", 20) / 2,
		300, 20, LIGHTGRAY);
}