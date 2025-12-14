#include "UISnailSelect.h"
#include "ModuleGame.h"
#include "Snail.h"
#include "EnhypenSnail.h"
#include "ChopinSnail.h"
#include "AdoSnail.h"
#include "MikuSnail.h"

UISnailSelect::UISnailSelect(ModuleGame* g) : game(g) {}

void UISnailSelect::UpdateSnailSelect()
{
	Vector2 mouse = GetMousePosition();

	for (int i = 0; i < 4; ++i)
	{
		if (CheckCollisionPointRec(mouse, game->selectRegions[i]) &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			switch (i)
			{
			case 0: ChooseSnail(game->enhypenSnail); break;
			case 1: ChooseSnail(game->chopinSnail); break;
			case 2: ChooseSnail(game->adoSnail); break;
			case 3: ChooseSnail(game->mikuSnail); break;
			}
		}
	}

	if (IsKeyPressed(KEY_ESCAPE))
		game->gameState = GameState::START_SCREEN;
}

void UISnailSelect::DrawSnailSelect()
{
	DrawText("CHOOSE YOUR SNAIL",
		SCREEN_WIDTH / 2 - MeasureText("CHOOSE YOUR SNAIL", 32) / 2,
		80, 32, BLACK);

	Snail* snails[4] = {
		game->enhypenSnail,
		game->chopinSnail,
		game->adoSnail,
		game->mikuSnail
	};

	for (int i = 0; i < 4; ++i)
	{
		DrawRectangleRec(game->selectRegions[i], Color{ 30, 30, 30, 220 });

		Texture2D tex = snails[i]->GetTexture();

		if (tex.id != 0) // safety
		{
			DrawTexturePro(
				tex,
				{ 0, 0, (float)tex.width, (float)tex.height },
				game->selectRegions[i],
				{ 0, 0 },
				0.0f,
				WHITE
			);
		}
	}
}

void UISnailSelect::ChooseSnail(Snail* chosen)
{
	// deactivate all
	game->enhypenSnail->active = false;
	game->chopinSnail->active = false;
	game->adoSnail->active = false;
	game->mikuSnail->active = false;

	// activate chosen
	chosen->active = true;
	game->playerSnail = chosen;
	game->snailChosen = true;

	// center camera on chosen snail immediately
	Vector2 p = game->playerSnail->GetPosition();
	game->App->renderer->camera.x = -p.x + (SCREEN_WIDTH / 2.0f);
	game->App->renderer->camera.y = -p.y + (SCREEN_HEIGHT / 2.0f);

	// ensure housekeeping
	game->nextCheckpoint = 0;
	game->passedAllCheckpoints = false;
	game->laps = 0;
	game->roundOver = false;
	game->currentRoundTimer = 0.0f;

	// go to gameplay
	game->gameState = GameState::PLAYING;
}