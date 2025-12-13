#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "Snail.h"
#include "Box.h"
#include "PhysicEntity.h"
#include "EnhypenSnail.h"
#include "ChopinSnail.h"
#include "AdoSnail.h"
#include "MikuSnail.h"

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

	background = LoadTexture("Assets/Textures/Racing_Map.png");

	enhypenSnail = new EnhypenSnail(App->physics, SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.9f, this);
	chopinSnail = new ChopinSnail(App->physics, SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.9f, this);
	adoSnail = new AdoSnail(App->physics, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.9f, this);
	mikuSnail = new MikuSnail(App->physics, SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.9f, this);

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

	PhysBody* checkpoint1 = CreateCheckPoint(300, 500, 50, 10, 0);
	PhysBody* checkpoint2 = CreateCheckPoint(300, 300, 50, 10, 1);
	PhysBody* checkpoint3 = CreateCheckPoint(300, 100, 50, 10, 2);

	checkpoints.push_back(checkpoint1);
	checkpoints.push_back(checkpoint2);
	checkpoints.push_back(checkpoint3);

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
	if (bodyA->ctype == ColliderType::SNAIL && bodyB != nullptr) {
		switch (bodyB->ctype) {
		case ColliderType::CHECKPOINT:
			int index = bodyB->checkpointIndex;
			if (index == nextCheckpoint) {
				//If index = 0 and passed all checkpoints, lap completed
				if (index == 0 && nextCheckpoint == 0 && passedAllCheckpoints) {
					laps++;
					TraceLog(LOG_INFO, "Lap completed! Total laps = %d", laps);

					//Reset for next lap
					nextCheckpoint = 1;
					passedAllCheckpoints = false;
				}
				else {
					//Move to next checkpoint
					nextCheckpoint++;

					//Player passed all checkponts
					if (nextCheckpoint >= checkpoints.size()) {
						nextCheckpoint = 0;
						passedAllCheckpoints = true;
					}

					TraceLog(LOG_INFO, "Checkpoint %d reached", index);
				}
			}
			break;
		}
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

PhysBody* ModuleGame::CreateCheckPoint(float x, float y, float w, float h, int num)
{
	PhysBody* cp = App->physics->CreateRectangleSensor(x, y, w, h);
	cp->checkpointIndex = num;
	cp->ctype = ColliderType::CHECKPOINT;
	return cp;
}

// Update: draw background
update_status ModuleGame::Update()
{

	switch (gameState)
	{
	case GameState::START_SCREEN:
		DrawStartScreen();
		break;

	case GameState::SNAIL_SELECT:
		DrawSnailSelect();
		break;

	case GameState::PLAYING:
		DrawGameplay();
		break;

	case GameState::GAME_OVER:
		DrawGameOver();
		break;
	}

	switch (gameState){
	case GameState::START_SCREEN:
		UpdateStartScreen();
		break;

	case GameState::SNAIL_SELECT:
		UpdateSnailSelect();
		break;

	case GameState::PLAYING:
		UpdateGameplay();
		break;

	case GameState::GAME_OVER:
		UpdateGameOver();
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

void ModuleGame::UpdateStartScreen()
{
	if (IsKeyPressed(KEY_ENTER))
		gameState = GameState::SNAIL_SELECT;

	if (IsKeyPressed(KEY_ESCAPE))
		App->window->RequestClose();
}

void ModuleGame::DrawStartScreen()
{
	DrawText("S N A I L I E S T   R A C E",
		SCREEN_WIDTH / 2 - MeasureText("S N A I L I E S T   R A C E", 48) / 2,
		100, 48, DARKGREEN);

	DrawText("PRESS ENTER TO START",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO START", 20) / 2,
		300, 20, LIGHTGRAY);

	DrawText("PRESS ESC TO EXIT",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ESC TO EXIT", 20) / 2,
		340, 20, LIGHTGRAY);
}

void ModuleGame::UpdateSnailSelect()
{
	Vector2 mouse = GetMousePosition();

	for (int i = 0; i < 4; ++i)
	{
		if (CheckCollisionPointRec(mouse, selectRegions[i]) &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			switch (i)
			{
			case 0: ChooseSnail(enhypenSnail); break;
			case 1: ChooseSnail(chopinSnail); break;
			case 2: ChooseSnail(adoSnail); break;
			case 3: ChooseSnail(mikuSnail); break;
			}
		}
	}

	if (IsKeyPressed(KEY_ESCAPE))
		gameState = GameState::START_SCREEN;
}

void ModuleGame::DrawSnailSelect()
{
	DrawText("CHOOSE YOUR SNAIL",
		SCREEN_WIDTH / 2 - MeasureText("CHOOSE YOUR SNAIL", 32) / 2,
		80, 32, BLACK);

	Snail* snails[4] = {
		enhypenSnail,
		chopinSnail,
		adoSnail,
		mikuSnail
	};

	for (int i = 0; i < 4; ++i)
	{
		DrawRectangleRec(selectRegions[i], Color{ 30, 30, 30, 220 });

		Texture2D tex = snails[i]->GetTexture();

		if (tex.id != 0) // safety
		{
			DrawTexturePro(
				tex,
				{ 0, 0, (float)tex.width, (float)tex.height },
				selectRegions[i],
				{ 0, 0 },
				0.0f,
				WHITE
			);
		}
	}
}

void ModuleGame::ChooseSnail(Snail* chosen)
{
	// deactivate all
	enhypenSnail->active = false;
	chopinSnail->active = false;
	adoSnail->active = false;
	mikuSnail->active = false;

	// activate chosen
	chosen->active = true;
	playerSnail = chosen;
	snailChosen = true;

	// center camera on chosen snail immediately
	Vector2 p = playerSnail->GetPosition();
	App->renderer->camera.x = -p.x + (SCREEN_WIDTH / 2.0f);
	App->renderer->camera.y = -p.y + (SCREEN_HEIGHT / 2.0f);

	// ensure housekeeping
	nextCheckpoint = 0;
	passedAllCheckpoints = false;
	laps = 0;
	roundOver = false;
	currentRoundTimer = 0.0f;

	// go to gameplay
	gameState = GameState::PLAYING;
}

void ModuleGame::DrawGameplay()
{
	// Draw background FIRST
	if (background.id != 0)
	{
		DrawTextureEx(
			background,
			Vector2{ (float)App->renderer->camera.x, (float)App->renderer->camera.y },
			0.0f,
			1.0f,
			WHITE
		);
	}
}

void ModuleGame::UpdateGameplay()
{
	for (PhysicEntity* e : entities)
		if (e->active)
			e->Update();

	UpdateCamera();
}


void ModuleGame::UpdateGameOver()
{
	if (IsKeyPressed(KEY_ENTER))
	{
		ResetRace();
		gameState = GameState::START_SCREEN;
	}
}

void ModuleGame::DrawGameOver()
{
	DrawText("FINISH!",
		SCREEN_WIDTH / 2 - MeasureText("FINISH!", 48) / 2,
		200, 48, RAYWHITE);

	DrawText("PRESS ENTER TO RETURN",
		SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RETURN", 20) / 2,
		300, 20, LIGHTGRAY);
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
