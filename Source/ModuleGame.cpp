#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Snail.h"
#include "Box.h"
#include "PhysicEntity.h"
#include "EnhypenSnail.h"
#include "ChopinSnail.h"
#include "AdoSnail.h"
#include "MikuSnail.h"
#include "UIStartScreen.h"
#include "UISnailSelect.h"
#include "UIGameOver.h"

//class Plane : public Box {
//public:
//	Plane(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 232, 121, _listener, _texture, PhysicCategory::PLANE, PhysicCategory::DEFAULT) {
//		body->body->ApplyForce(b2Vec2(0.0f, -1000.f), body->body->GetWorldCenter(), true);
//	}
//};
//
//class Bike : public Box {
//public:
//	Bike(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 18, 35, _listener, _texture, PhysicCategory::BIKE, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
//	}
//};

//class Ship : public Box {
//public:
//	Ship(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 215, 138, _listener, _texture, PhysicCategory::SHIP, PhysicCategory::DEFAULT | PhysicCategory::SHIP)
//	{
//	}
//};

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

	//background = LoadTexture("Assets/Textures/Racing_Map2.png");
	map = new Map(App, this);
	map->Start();
	//background = LoadTexture("Assets/Textures/Racing_Map.png");

	enhypenSnail = new EnhypenSnail(App->physics, SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.9f, this);
	chopinSnail = new ChopinSnail(App->physics, SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.9f, this);
	adoSnail = new AdoSnail(App->physics, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.9f, this);
	mikuSnail = new MikuSnail(App->physics, SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.9f, this);

	startScreenUI = new UIStartScreen(this);
	snailSelectUI = new UISnailSelect(this);
	gameOverUI = new UIGameOver(this);

	entities.push_back(enhypenSnail);
	entities.push_back(chopinSnail);
	entities.push_back(adoSnail);
	entities.push_back(mikuSnail);

	enhypenSnail->active = false;
	chopinSnail->active = false;
	adoSnail->active = false;
	mikuSnail->active = false;

	for (PhysicEntity* entity : entities)
	{
		Snail* snail = dynamic_cast<Snail*>(entity);
		if (snail) {
			snail->Start();
		}
	}

	/*PhysBody* checkpoint1 = CreateCheckPoint(300, 500, 50, 10, 0);
	PhysBody* checkpoint2 = CreateCheckPoint(300, 300, 50, 10, 1);
	PhysBody* checkpoint3 = CreateCheckPoint(300, 100, 50, 10, 2);*/

	/*checkpoints.push_back(checkpoint1);
	checkpoints.push_back(checkpoint2);
	checkpoints.push_back(checkpoint3);*/

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

	//for (int i = 0; i < 2; ++i) {
	//	entities.push_back(new Ship(App->physics, i * 300 + SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.5f, this, ship));
	//}

	//for (int i = 0; i < 6; ++i) {
	//	entities.push_back(new Bike(App->physics, i * 100 + SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, this, bike));
	//}

	//for (int i = 0; i < 3; ++i) {
	//	entities.push_back(new Plane(App->physics, i * 300 + SCREEN_WIDTH * 0.25f, 600, this, plane));
	//}

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	for (PhysicEntity* entity : entities) {
		Snail* snail = dynamic_cast<Snail*>(entity);
		if (snail) {
			snail->CleanUp();
		}
	}

	return true;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
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

void ModuleGame::UpdateCamera()
{
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
	//Only checks active snail
	if (snail->active == false) {
		return;
	}

	//Check if this is the correct next checkpoint
	if (num == nextCheckpoint) {
		if (num == 0 && nextCheckpoint == 0 && passedAllCheckpoints) {
			laps++;
			TraceLog(LOG_INFO, "Lap completed! Total laps = %d", laps);

			//Reset for next lap
			nextCheckpoint = 1;
			passedAllCheckpoints = false;

			//Check if race is finished (3 laps)
			if (laps >= 3) {
				roundOver = true;
				gameState = GameState::GAME_OVER;

				//Update best time
				if (currentRoundTimer < bestRoundTimer) {
					bestRoundTimer = currentRoundTimer;
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
}

// Update: draw background
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
}

void ModuleGame::UpdateGameplay()
{
	for (PhysicEntity* e : entities)
		if (e->active)
			e->Update();

	UpdateCamera();
	map->Update();
	currentRoundTimer += GetFrameTime();
}

void ModuleGame::ResetRace()
{
	// Reset timers and lap counters
	laps = 0;
	nextCheckpoint = 0;
	passedAllCheckpoints = false;
	roundOver = false;
	currentRoundTimer = 0.0f;

	// reset camera
	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	// deactivate player
	snailChosen = false;
	playerSnail = nullptr;

	// Reset snail positions to spawn line (recreate or reposition as desired)
	// For simplicity, reposition their bodies to original spawn points if you have a reposition helper.
	// (You can implement repositioning in your Snail or PhysBody classes.)
}
