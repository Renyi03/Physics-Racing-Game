#include "UISnailSelect.h"
#include "raylib.h"
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
			case 0: ChooseSnail(SnailType::ENHYPEN); break;
			case 1: ChooseSnail(SnailType::CHOPIN); break;
			case 2: ChooseSnail(SnailType::ADO);     break;
			case 3: ChooseSnail(SnailType::MIKU);    break;
			}
		}
	}

	if (IsKeyPressed(KEY_ESCAPE))
		game->gameState = GameState::START_SCREEN;
}

void UISnailSelect::DrawSnailSelect()
{
	if (!texturesLoaded)
	{
		previewEnhypen = LoadTexture("Assets/Textures/Enhypen_Snail.png");
		previewChopin = LoadTexture("Assets/Textures/Chopin_Snail.png");
		previewAdo = LoadTexture("Assets/Textures/Ado_Snail.png");
		previewMiku = LoadTexture("Assets/Textures/Miku_Snail.png");
		texturesLoaded = true;
	}

	DrawText("CHOOSE YOUR SNAIL",
		SCREEN_WIDTH / 2 - MeasureText("CHOOSE YOUR SNAIL", 32) / 2,
		80, 32, BLACK);

	Texture2D previews[4] = {
		previewEnhypen,
		previewChopin,
		previewAdo,
		previewMiku
	};

	for (int i = 0; i < 4; ++i)
	{
		DrawRectangleRec(game->selectRegions[i], Color{ 30, 30, 30, 220 });

		if (previews[i].id != 0) // safety
		{
			DrawTexturePro(
				previews[i],
				{ 0, 0, (float)previews[i].width, (float)previews[i].height },
				game->selectRegions[i],
				{ 0, 0 },
				0.0f,
				WHITE
			);
		}
	}
}

void UISnailSelect::ChooseSnail(SnailType type)
{
	game->SpawnGameplay(type);

	// Center camera after spawn
	Vector2 p = game->playerSnail->GetPosition();
	game->App->renderer->camera.x = -p.x + (SCREEN_WIDTH / 2.0f);
	game->App->renderer->camera.y = -p.y + (SCREEN_HEIGHT / 2.0f);

	TraceLog(LOG_INFO, "GAMESTATE PLAYING CALLED");
	game->gameState = GameState::PLAYING;
}
