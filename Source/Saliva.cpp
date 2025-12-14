#include "Saliva.h"
#include "ModuleGame.h"
#include "Application.h"
#include "ModulePhysics.h"
#include <vector>

Saliva::~Saliva()
{
	// Only delete body if the body still exists
	if (body) {
		if (body->body) {
			body->listener->App->physics->DestroyBody(body);
			body->body = nullptr;
		}
		delete body;
		body = nullptr;
	}
}

void Saliva::Update()
{
	if (active) {
		timer += GetFrameTime();

		// Encoger el body gradualmente
		if (timer - lastShrinkTime >= shrinkInterval) {
			lastShrinkTime = timer;
			ShrinkBody();
		}

		if (timer >= lifeTime) {
			active = false;
		}
	}

	// Call to Box::Update() only if the body still exists
	if (body && body->body) {
		Box::Update();
	}
}

void Saliva::ShrinkBody()
{
	if (!body || !body->body) return;

	// Percentage of life created (1.0 = created now, 0.0 = expired)
	float lifePercent = 1.0f - (timer / lifeTime);

	// If it is almost expired, it completely destroys the body.
	if (lifePercent <= 0.15f) {
		body->listener->App->physics->DestroyBody(body);
		body->body = nullptr;
		return;
	}

	// Calculate new size based on percentage of life
	int newWidth = (int)(initialWidth * lifePercent);
	int newHeight = (int)(initialHeight * lifePercent);

	// Ensure minimum size
	if (newWidth < 4) newWidth = 4;
	if (newHeight < 4) newHeight = 4;

	// Save current position
	b2Vec2 currentPos = body->body->GetPosition();
	float currentAngle = body->body->GetAngle();
	b2Vec2 currentVel = body->body->GetLinearVelocity();
	float currentAngularVel = body->body->GetAngularVelocity();

	// Destroy current fixtures
	b2Fixture* fixture = body->body->GetFixtureList();
	while (fixture) {
		b2Fixture* next = fixture->GetNext();
		body->body->DestroyFixture(fixture);
		fixture = next;
	}

	// Create a new shape with a reduced size
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(newWidth) * 0.5f, PIXEL_TO_METERS(newHeight) * 0.5f);

	// Create new fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.isSensor = true; // Keep as sensor

	body->body->CreateFixture(&fixtureDef);

	// Restore speed
	body->body->SetLinearVelocity(currentVel);
	body->body->SetAngularVelocity(currentAngularVel);

	// Update dimensions in PhysBody
	body->width = newWidth;
	body->height = newHeight;
}

bool Saliva::IsExpired() const
{
	return timer >= lifeTime;
}

float Saliva::GetAlpha() const
{
	// Retorna un valor entre 0.0 (invisible) y 1.0 (totalmente visible)
	float alpha = 1.0f - (timer / lifeTime);
	return alpha < 0.0f ? 0.0f : alpha;
}