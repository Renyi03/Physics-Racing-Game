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

	plane = LoadTexture("Assets/Plane.png");
	car = LoadTexture("Assets/Car.png");
	ship = LoadTexture("Assets/Ship.png");
	bike = LoadTexture("Assets/Bike.png");
	Texture2D enhypenSnailTexture = LoadTexture("Assets/Textures/Enhypen_Snail.png");
	Texture2D chopinSnailTexture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	Texture2D adoSnailTexture = LoadTexture("Assets/Textures/Ado_Snail.png");
	Texture2D mikuSnailTexture = LoadTexture("Assets/Textures/Miku_Snail.png");

	enhypenSnail = new EnhypenSnail(App->physics, SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.9f, this, enhypenSnailTexture);
	chopinSnail = new ChopinSnail(App->physics, SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.9f, this, chopinSnailTexture);
	adoSnail = new AdoSnail(App->physics, SCREEN_WIDTH * 0.55f, SCREEN_HEIGHT * 0.9f, this, adoSnailTexture);
	mikuSnail = new MikuSnail(App->physics, SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.9f, this, mikuSnailTexture);

	entities.push_back(enhypenSnail);
	entities.push_back(chopinSnail);
	entities.push_back(adoSnail);
	entities.push_back(mikuSnail);

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

	return true;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
}

// Update: draw background
update_status ModuleGame::Update()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}


	// ray -----------------
	if (ray_on == true)
	{
		vec2f destination((float)(mouse.x - ray.x), (float)(mouse.y - ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}


	//SELECT SNAIL
	if (enhypenSnail->body->body->GetFixtureList()->TestPoint(b2Vec2{ GetMousePosition().x, GetMousePosition().y }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		enhypenSnail->active = true;
		chopinSnail->active = false;
		adoSnail->active = false;
		mikuSnail->active = false;
	}
	if (chopinSnail->body->body->GetFixtureList()->TestPoint(b2Vec2{ GetMousePosition().x, GetMousePosition().y }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		enhypenSnail->active = false;
		chopinSnail->active = true;
		adoSnail->active = false;
		mikuSnail->active = false;
	}
	if (adoSnail->body->body->GetFixtureList()->TestPoint(b2Vec2{ GetMousePosition().x, GetMousePosition().y }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		enhypenSnail->active = false;
		chopinSnail->active = false;
		adoSnail->active = true;
		mikuSnail->active = false;
	}
	if (mikuSnail->body->body->GetFixtureList()->TestPoint(b2Vec2{ GetMousePosition().x, GetMousePosition().y}) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		enhypenSnail->active = false;
		chopinSnail->active = false;
		adoSnail->active = false;
		mikuSnail->active = true;
	}

	return UPDATE_CONTINUE;
}
