#include "UIGameOver.h"
#include "Application.h"
#include "ModuleGame.h"
#include "Snail.h"
#include <string>

UIGameOver::UIGameOver(ModuleGame* g) : game(g) {}

void UIGameOver::UpdateGameOver()
{
    game->physicsActive = false;
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

        game->raceResults.clear();
        game->finishedCount = 0;

		game->ResetRace();
		game->gameState = GameState::START_SCREEN;

	}
}

void UIGameOver::DrawGameOver()
{
    // Title
    DrawText("RACE COMPLETE!",
        SCREEN_WIDTH / 2 - MeasureText("RACE COMPLETE!", 48) / 2,
        40, 48, GREEN);

    // Find player result
    const RaceResult* playerResult = nullptr;
    for (const auto& result : game->raceResults)
    {
        if (result.snail == game->playerSnail)
        {
            playerResult = &result;
            break;
        }
    }

    // Big player placement text
    if (playerResult)
    {
        const char* posText = "";

        switch (playerResult->position)
        {
        case 1: posText = "YOU GOT 1ST PLACE!"; break;
        case 2: posText = "YOU GOT 2ND PLACE"; break;
        case 3: posText = "YOU GOT 3RD PLACE"; break;
        default: posText = "YOU FINISHED LAST"; break;
        }

        DrawText(posText,
            SCREEN_WIDTH / 2 - MeasureText(posText, 56) / 2,
            120, 56, GOLD);
    }

    // Standings title
    DrawText("FINAL STANDINGS",
        SCREEN_WIDTH / 2 - MeasureText("FINAL STANDINGS", 32) / 2,
        220, 32, BLACK);

    int yPos = 280;

    for (const auto& result : game->raceResults)
    {
        Color color = (result.snail == game->playerSnail) ? GOLD : BLACK;

        std::string text = TextFormat(
            "%d. %s  -  %.2f s",
            result.position,
            game->GetSnailName(result.type).c_str(),
            result.finishTime
        );

        DrawText(text.c_str(),
            SCREEN_WIDTH / 2 - MeasureText(text.c_str(), 28) / 2,
            yPos, 28, color);

        yPos += 40;
    }

    DrawText(TextFormat("BEST TIME: %.2f", game->bestRoundTimer), SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT - 50, 30, GOLD);

    DrawText("PRESS ENTER TO RETURN TO MENU",
        SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RETURN TO MENU", 20) / 2,
        SCREEN_HEIGHT - 80, 20, LIGHTGRAY);
}

