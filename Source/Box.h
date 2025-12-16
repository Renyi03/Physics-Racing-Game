#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
//#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, int width, int height, Module* _listener, Texture2D _texture, uint16 category, uint16 maskBits)
		: PhysicEntity(physics->CreateRectangle(_x, _y, width, height, category, maskBits), _listener)
		, texture(_texture)
	{
		body->body->SetFixedRotation(true);
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);

		float renderX = x + listener->App->renderer->camera.x;
		float renderY = y + listener->App->renderer->camera.y;

		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ renderX, renderY, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);	
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

	virtual void SetTexture(Texture2D newTexture)
	{
		texture = newTexture;
	}

protected:
	Texture2D texture;
};