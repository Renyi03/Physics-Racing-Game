#pragma once

#include "Globals.h"
#include "Module.h"
#include "Map.h"
#include <string>

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

class UIStartScreen;
class UISnailSelect;
class UIGameOver;

enum class SnailType;

enum class GameState
{
	START_SCREEN,
	SNAIL_SELECT,
	PLAYING,
	GAME_OVER
};

struct RaceResult {
	Snail* snail;
	SnailType type;
	float finishTime;
	int position;
	bool finished;
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
	void EndCollision(PhysBody* bodyA, PhysBody* bodyB);
	void UpdateCamera();
	void CheckpointManager(Snail* snail, int num);

	
	void DrawGameplay();
	void SpawnGameplay(SnailType chosenType);
	void UpdateGameplay();
	void ResetRace();

public:

	GameState gameState = GameState::START_SCREEN;
	std::vector<PhysicEntity*> entities;
	Snail* playerSnail = nullptr;

	std::vector<RaceResult> raceResults;  
	int finishedCount = 0;                

	void RecordFinish(Snail* snail, float time);  
	std::string GetSnailName(SnailType type);

	Map* map;

	EnhypenSnail* enhypenSnail;
	ChopinSnail* chopinSnail;
	AdoSnail* adoSnail;
	MikuSnail* mikuSnail;

	UISnailSelect* snailSelectUI = nullptr;
	UIStartScreen* startScreenUI = nullptr;
	UIGameOver* gameOverUI = nullptr;

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

	int nextCheckpoint = 0;
	int laps = 0;
	bool passedAllCheckpoints = false;
	float currentRoundTimer;
	float bestRoundTimer = 0.0f;
	bool hasBestRoundTime = false;
	bool roundOver = false;

	//For starting countdown
	float countdownTimer = 3.0f;
	bool raceStarted = false;

	bool physicsActive = true;
};
