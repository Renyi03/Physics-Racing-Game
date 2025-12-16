#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Snail.h"
#include "SnailAI.h"
#include "Box.h"
#include "PhysicEntity.h"
#include "EnhypenSnail.h"
#include "ChopinSnail.h"
#include "AdoSnail.h"
#include "MikuSnail.h"
#include "UIStartScreen.h"
#include "UISnailSelect.h"
#include "UIGameOver.h"
#include <string>

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	currentRoundTimer = 0.0f;

	map = new Map(App, this);
	map->Start();

	startScreenUI = new UIStartScreen(this);
	snailSelectUI = new UISnailSelect(this);
	gameOverUI = new UIGameOver(this);

	// Selection boxes
	for (int i = 0; i < 4; ++i)
	{
		selectRegions[i] = {
			200.0f + i * 200.0f,
			200.0f,
			150.0f,
			150.0f
		};
	}

	ResetRace();

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	if (IsMusicStreamPlaying(bgm)) {
		StopMusicStream(bgm);
	}
	UnloadMusicStream(bgm);

	if (finishFx.frameCount > 0) {
		UnloadSound(finishFx);
	}

	if(startScreenUI) {
		delete startScreenUI;
		startScreenUI = nullptr;
	}
	if (snailSelectUI) {
		delete snailSelectUI;
		snailSelectUI = nullptr;
	}
	if (gameOverUI) {
		delete gameOverUI;
		gameOverUI = nullptr;
	}

	if (map) {
		map->CleanUp();
		delete map;
		map = nullptr;
	}

	for (PhysicEntity* entity : entities) {
		Snail* snail = dynamic_cast<Snail*>(entity);
		if (snail) {
			snail->body = nullptr;
			snail->CleanUp();
		}
		delete entity;
	}
	entities.clear();

	raceResults.clear();

	playerSnail = nullptr;
	enhypenSnail = nullptr;
	chopinSnail = nullptr;
	adoSnail = nullptr;
	mikuSnail = nullptr;

	return true;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == nullptr || bodyB == nullptr) {
		return;
	}

	//Check if the body colliding is a snail
	if (bodyA->ctype == ColliderType::SNAIL) {
		Snail* snailA = nullptr;
		for (PhysicEntity* entity : entities) {
			Snail* snail = dynamic_cast<Snail*>(entity);
			if (snail && snail->body == bodyA) {
				snailA = snail;
				break;
			}
		}

		if (!snailA) return;

		snailA->OnCollisionWithMap(bodyB);
	}

	//...or if it's a map object colliding with a snail :0
	else if (bodyB->ctype == ColliderType::SNAIL) {
		Snail* snailB = nullptr;
		for (PhysicEntity* entity : entities) {
			Snail* snail = dynamic_cast<Snail*>(entity);
			if (snail && snail->body == bodyB) {
				snailB = snail;
				break;
			}
		}

		if (!snailB) return;

		snailB->OnCollisionWithMap(bodyA);
	}
}

void ModuleGame::EndCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (!physicsActive) return;

	if (bodyA == nullptr || bodyB == nullptr) {
		return;
	}
	if (bodyA->body == nullptr || bodyB->body == nullptr) {
		return;
	}

	//Check if the body colliding is a snail
	if (bodyA->ctype == ColliderType::SNAIL) {
		Snail* snailA = nullptr;
		for (PhysicEntity* entity : entities) {
			if (entity == nullptr || entity->body == nullptr) {
				continue;
			}

			if (entity->body != nullptr) {
				Snail* snail = dynamic_cast<Snail*>(entity);
				if (snail && snail->body == bodyA) {
					snailA = snail;
					break;
				}
			}
		}

		if (!snailA) return;

		snailA->EndCollisionWithMap(bodyB);
	}

	//...or if it's a map object colliding with a snail :0
	else if (bodyB->ctype == ColliderType::SNAIL) {
		Snail* snailB = nullptr;
		for (PhysicEntity* entity : entities) {
			Snail* snail = dynamic_cast<Snail*>(entity);
			if (snail && snail->body == bodyB) {
				snailB = snail;
				break;
			}
		}

		if (!snailB) return;

		snailB->EndCollisionWithMap(bodyA);
	}
}

void ModuleGame::UpdateCamera()
{
	if (!playerSnail || !playerSnail->body)
		return;
	//Get player position
	Vector2 playerPos = playerSnail->GetPosition();
	
	//Camera target centers on player
	float targetX = -playerPos.x + (SCREEN_WIDTH / 2.0f);
	float targetY = -playerPos.y + (SCREEN_HEIGHT / 2.0f);

	//Camera follows player
	float smoothSpeed = 5.0f;
	float deltaTime = GetFrameTime();

	App->renderer->camera.x += (targetX - App->renderer->camera.x) * smoothSpeed * deltaTime;
	App->renderer->camera.y += (targetY - App->renderer->camera.y) * smoothSpeed * deltaTime;

	//Camera doesn't get out of limits
	float mapWidth = 2000.0f;
	float mapHeight = 1500.0f;
	float minX = -mapWidth + SCREEN_WIDTH;
	float maxX = 0;
	float minY = -mapHeight + SCREEN_HEIGHT;
	float maxY = 0;

	App->renderer->camera.x = fmax(minX, fmin(maxX, App->renderer->camera.x));
	App->renderer->camera.y = fmax(minY, fmin(maxY, App->renderer->camera.y));
}

void ModuleGame::CheckpointManager(Snail* snail, int num)
{
	//Check if this is the correct next checkpoint
	if (snail->active && num == nextCheckpoint) {
		//Only checks active snail
		if (snail->active == false) {
			return;
		}
		if (num == 0 && nextCheckpoint == 0 && passedAllCheckpoints) {
			laps++;
			TraceLog(LOG_INFO, "Lap completed! Total laps = %d", laps);

			//Reset for next lap
			nextCheckpoint = 1;
			passedAllCheckpoints = false;

			//Check if race is finished (3 laps)
			if (laps >= 3) {
				roundOver = true;
				//gameState = GameState::GAME_OVER;
				RecordFinish(snail, currentRoundTimer);
				//Update best time
				if (!hasBestRoundTime || currentRoundTimer < bestRoundTimer)
				{
					bestRoundTimer = currentRoundTimer;
					hasBestRoundTime = true;
				}
				TraceLog(LOG_INFO, "Race finished! Current round time: %.2f & Best time: %.2f",
					currentRoundTimer, bestRoundTimer);
			}
		}
		else {
			//Go to next checkpoint
			nextCheckpoint++;

			//Check if all checkpoints passed
			if (nextCheckpoint >= map->checkpoints.size()) {
				nextCheckpoint = 0;
				passedAllCheckpoints = true;
			}

			TraceLog(LOG_INFO, "Checkpoint %d reached", num);
		}
	}
	if (snail->isAI) {
		if (num == 0 && snail->aiNextCheckpoint == 0 && snail->aiPassedAllCheckpoints) {
			snail->aiLaps++;
			TraceLog(LOG_INFO, "AI lap completed! Total laps = %d", snail->aiLaps);
			snail->aiNextCheckpoint = 1;
			snail->aiPassedAllCheckpoints = false;

			if (snail->aiLaps == 3) {
				snail->finished = true;
				snail->finishTime = currentRoundTimer;
				RecordFinish(snail, snail->finishTime);
			}
		}
		else if (num == snail->aiNextCheckpoint) {
			snail->aiNextCheckpoint++;
			if (snail->aiNextCheckpoint >= map->checkpoints.size()) {
				snail->aiNextCheckpoint = 0;
				snail->aiPassedAllCheckpoints = true;
			}
		}
	}
}

update_status ModuleGame::Update()
{
	switch (gameState)
	{
	case GameState::START_SCREEN:
		startScreenUI->DrawStartScreen();
		startScreenUI->UpdateStartScreen();
		break;

	case GameState::SNAIL_SELECT:
		snailSelectUI->DrawSnailSelect();
		snailSelectUI->UpdateSnailSelect();
		break;

	case GameState::PLAYING:
		DrawGameplay();
		UpdateGameplay();
		UpdateMusicStream(bgm);
		break;

	case GameState::GAME_OVER:
		gameOverUI->DrawGameOver();
		gameOverUI->UpdateGameOver();
		break;
	}

	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	return UPDATE_CONTINUE;
}

void ModuleGame::DrawGameplay()
{	
	map->DrawMapTexture();
	DrawText(TextFormat("CURRENT TIME: %.2f", currentRoundTimer), SCREEN_WIDTH / 2 - 140, 10, 30, WHITE);
	if (countdownTimer <= 3.0f && countdownTimer > 2.0f) {
		DrawText("3", SCREEN_WIDTH / 2, 50, 60, WHITE);
	}
	else if (countdownTimer <= 2.0f && countdownTimer > 1.0f) {
		DrawText("2", SCREEN_WIDTH / 2, 50, 60, WHITE);
	}
	else if (countdownTimer <= 1.0f && countdownTimer > 0.0f) {
		DrawText("1", SCREEN_WIDTH / 2, 50, 60, WHITE);
	}
	if (playerSnail) {
		float cooldown = playerSnail->GetAbilityCooldown();
		if (cooldown > 0) {
			DrawText(TextFormat("ABILITY COOLDOWN: %.1f", cooldown), SCREEN_WIDTH - 300, 10, 24, WHITE);
		}
		else {
			DrawText(TextFormat("ABILITY COOLDOWN: 0.0"), SCREEN_WIDTH - 300, 10, 24, WHITE);
		}
	}
}

void ModuleGame::SpawnGameplay(SnailType chosenType)
{
	Vector2 firstCheckpoint = map->GetWaypoints()[0];

	float startX = firstCheckpoint.x - 50.0f;  // 150 pixels left of checkpoint
	float startY = firstCheckpoint.y;
	float spacing = 40.0f;  // Space between snails
	
	// Spawn snails
	enhypenSnail = new EnhypenSnail(App->physics, startX, startY - spacing * 1.5f, this);
	chopinSnail = new ChopinSnail(App->physics, startX, startY - spacing * 0.5f, this);
	adoSnail = new AdoSnail(App->physics, startX, startY + spacing * 0.5f, this);
	mikuSnail = new MikuSnail(App->physics, startX, startY + spacing * 1.5f, this);

	entities = {
		enhypenSnail,
		chopinSnail,
		adoSnail,
		mikuSnail
	};

	for (auto* e : entities)
	{
		Snail* snail = dynamic_cast<Snail*>(e);
		snail->Start();
		snail->body->body->SetTransform(
			snail->body->body->GetPosition(),
			90.0f * DEG2RAD   // face right
		);
	}

	for (auto* e : entities)
		e->active = false;

	// Activate chosen
	switch (chosenType)
	{
	case SnailType::ENHYPEN: 
		playerSnail = enhypenSnail; 
		TraceLog(LOG_INFO, "Selected ENHYPEN snail"); 
		bgm = LoadMusicStream("Assets/Audio/Highway1009.wav");
		break;
	case SnailType::CHOPIN:  
		playerSnail = chopinSnail;
		TraceLog(LOG_INFO, "Selected CHOPIN snail"); 
		bgm = LoadMusicStream("Assets/Audio/BalladeNo1inGMinor.wav");
		break;
	case SnailType::ADO:     
		playerSnail = adoSnail; 
		TraceLog(LOG_INFO, "Selected ADO snail");     
		bgm = LoadMusicStream("Assets/Audio/AshuraChan.wav");
		break;
	case SnailType::MIKU:    
		playerSnail = mikuSnail; 
		TraceLog(LOG_INFO, "Selected MIKU snail");  
		bgm = LoadMusicStream("Assets/Audio/WorldIsMine.wav");
		break;
	}

	PlayMusicStream(bgm);
	SetMusicVolume(bgm, 0.4f);

	if (playerSnail) {
		TraceLog(LOG_INFO, "SETTING PLAYER SNAIL AS ACTIVE");
		playerSnail->active = true;
	}

	for (auto* e : entities)
	{
		Snail* snail = dynamic_cast<Snail*>(e);

		if (snail != playerSnail)
		{
			snail->isAI = true;
			snail->ai = new SnailAI(snail);
			snail->ai->SetWaypoints(map->GetWaypoints());
		}
	}

	ResetRace();
}

void ModuleGame::UpdateGameplay()
{
	physicsActive = true;
	float dt = GetFrameTime();

	if (!raceStarted) {
		countdownTimer -= dt;
		if (countdownTimer <= 0.0f) {
			countdownTimer = 0.0f;
			raceStarted = true;
		}

		UpdateCamera();
		map->Update();
		for (auto* e : entities)
		{
			Snail* snail = dynamic_cast<Snail*>(e);
			if (!snail) continue;

			int x, y;
			snail->body->GetPhysicPosition(x, y);

			float renderX = x + snail->listener->App->renderer->camera.x;
			float renderY = y + snail->listener->App->renderer->camera.y;

			Texture2D texture = snail->GetTexture();

			DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
				Rectangle{ renderX, renderY, (float)texture.width, (float)texture.height },
				Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, snail->body->GetRotation() * RAD2DEG, WHITE);
		}
		return;
	}

	for (auto* e : entities)
	{
		Snail* snail = dynamic_cast<Snail*>(e);
		if (!snail) continue;

		snail->Update();

		if (snail->isAI && !snail->finished)
		{
			snail->aiRaceTime += dt;
		}
	}
	UpdateCamera();
	map->Update();
	currentRoundTimer += dt;
	if (laps == 3) {
		for (auto* e : entities)
		{
			Snail* snail = dynamic_cast<Snail*>(e);
			if (!snail) continue;

		}
		gameState = GameState::GAME_OVER;
		
	}
}

void ModuleGame::ResetRace()
{
	map = new Map(App, this);
	map->Start();

	// Reset timers and lap counters
	laps = 0;
	nextCheckpoint = 0;
	passedAllCheckpoints = false;
	roundOver = false;
	currentRoundTimer = 0.0f;

	// reset camera
	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	//Reset countdown timer
	countdownTimer = 3.0f;
	raceStarted = false;
}

void ModuleGame::RecordFinish(Snail* snail, float time)
{
	// Check if already recorded
	for (auto& result : raceResults) {
		if (result.snail == snail && result.finished)
			return;
	}

	// Record finish
	finishedCount++;

	SnailType type = SnailType::ADO;
	if (snail == enhypenSnail) type = SnailType::ENHYPEN;
	else if (snail == chopinSnail) type = SnailType::CHOPIN;
	else if (snail == adoSnail) type = SnailType::ADO;
	else if (snail == mikuSnail) type = SnailType::MIKU;

	raceResults.push_back({ snail, type, time, finishedCount, true });

	TraceLog(LOG_INFO, "%s finished in position %d with time %.2f",
		GetSnailName(type).c_str(), finishedCount, time);

	// Check if all snails finished
	if (finishedCount >= 4) {
		gameState = GameState::GAME_OVER;
	}
}

std::string ModuleGame::GetSnailName(SnailType type)
{
	switch (type) {
	case SnailType::ENHYPEN: return "ENHYPEN";
	case SnailType::CHOPIN: return "CHOPIN";
	case SnailType::ADO: return "ADO";
	case SnailType::MIKU: return "MIKU";
	default: return "UNKNOWN";
	}
}
