#include "UIGameOver.h"
#include "ModuleGame.h"

UIGameOver::UIGameOver(ModuleGame* g) : game(g) {}

void UIGameOver::UpdateGameOver()
{
	if (IsKeyPressed(KEY_ENTER))
	{
		for (auto* e : game->entities)
		{
			if (e)
				delete e;  // This will delete the snail and its PhysBody
		}
		game->entities.clear();
		game->enhypenSnail = nullptr;
		game->chopinSnail = nullptr;
		game->adoSnail = nullptr;
		game->mikuSnail = nullptr;
		game->playerSnail = nullptr;
		game->ResetRace();
		game->gameState = GameState::START_SCREEN;
	}
}

void UIGameOver::DrawGameOver()
{
	DrawText("FINISH!",
		SCREEN_WIDTH / 2 - MeasureText("FINISH!", 48) / 2,
		200, 48, BLACK);

	DrawText("PRESS ENTER TO RETURN",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RETURN", 20) / 2,
		300, 20, LIGHTGRAY);
}
