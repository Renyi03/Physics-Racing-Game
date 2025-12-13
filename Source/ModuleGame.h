#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class Snail;
class EnhypenSnail;
class ChopinSnail;
class AdoSnail;
class MikuSnail;

class PhysBody;
class PhysicEntity;
class Snail;

enum class GameState
{
	START_SCREEN,
	SNAIL_SELECT,
	PLAYING,
	GAME_OVER
};

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void UpdateCamera();
	PhysBody* CreateCheckPoint(float x, float y, float w, float h, int num);

	void DrawStartScreen();
	void DrawSnailSelect();
	void DrawGameplay();
	void DrawGameOver();

	void UpdateStartScreen();
	void UpdateSnailSelect();
	void ChooseSnail(Snail* chosen);
	void UpdateGameplay();
	void UpdateGameOver();
	void ResetRace();

public:

	GameState gameState = GameState::START_SCREEN;
	std::vector<PhysicEntity*> entities;
	Snail* playerSnail = nullptr;

	Texture2D plane;
	Texture2D car;
	Texture2D ship;
	Texture2D bike;
	Texture2D background;

	EnhypenSnail* enhypenSnail;
	ChopinSnail* chopinSnail;
	AdoSnail* adoSnail;
	MikuSnail* mikuSnail;

	vec2<int> ray;
	bool ray_on;

	// selection UI
	int selection_index = 0;
	bool snailChosen = false;
	// selection UI tuning
	Rectangle selectRegions[4];

	int map_width = 2000;
	int map_height = 1500;
	
	std::vector<float> m_staticFrictions = { 0.0f, 0.1f, 0.3f, 0.5f };
	std::vector<float> m_dynamicFrictions = { 0.0f, 0.1f, 0.3f, 0.5f };

	std::vector<PhysBody*> checkpoints;
	int nextCheckpoint = 0;
	int laps = 0;
	bool passedAllCheckpoints = false;
	float currentRoundTimer;
	float bestRoundTimer = 10000000.0f;
	bool roundOver = false;
};
