#include "Saliva.h"
#include "ModuleGame.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Snail.h"
#include <vector>
#include <algorithm>

Saliva::~Saliva()
{
	// Clean the vector of snails
	snailsInContact.clear();
	
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

		// Apply friction to all snails in contact
		ApplyFrictionToSnails();
		
		// Shrink the body gradually
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

void Saliva::ApplyFrictionToSnails() {
	if (!body || !body->body) return;

	// Clean up any snails that are no longer in contact (just in case)
	snailsInContact.erase(
		std::remove_if(snailsInContact.begin(), snailsInContact.end(),
			[](Snail* s) {return s == nullptr || s->pendingToDelete; }),
		snailsInContact.end()
	);

	// Apply friction to each snail
	for (Snail* snail : snailsInContact) {
		if (!snail || !snail->body || !snail->body->body) {
			continue;
		}

		for (Snail* snail : snailsInContact) {
			if (snail) {
				snail->isOnSaliva = true;
			}
		}

		
		////if (snail == owner) continue;
		//
		//// Get current speed of the snail
		//b2Vec2 velocity = snail->body->body->GetLinearVelocity();

		//// Calculate the frictional force opposing the velocity
		//float speed = velocity.Length();
		//if (speed > 0.1f) {
		//	// Normalize speed direction
		//	b2Vec2 frictionDir = velocity;
		//	frictionDir.Normalize();
		//	frictionDir *= -1.0f; // Opposite to speed

		//	// Calculate the magnitude of friction based on the speed
		//	float frictionMagnitude = frictionCoefficient * speed * snail->body->body->GetMass() * 50.0f;

		//	// Apply friction force
		//	b2Vec2 frictionForce(frictionDir.x * frictionMagnitude, frictionDir.y * frictionMagnitude);
		//	snail->body->body->ApplyForceToCenter(frictionForce, true);
		//}
	}
}

void Saliva::OnCollision(PhysBody* otherBody)
{
	
	printf("=== SALIVA OnCollision ===\n");

	if (!otherBody) {
		printf("otherBody es NULL\n"); 
		return;
	}

	printf("otherBody->ctype = %d, SNAIL enum = %d\n", otherBody->ctype, ColliderType::SNAIL);

	// Verify if is a snail
	if (otherBody->ctype == ColliderType::SNAIL) {

		printf("¡SI ES UN SNAIL!\n");

		// Find the corresponding Snail object
		// Assuming listener is the game module that has access to the snails
		ModuleGame* game = dynamic_cast<ModuleGame*>(listener);
		if (game) {

			

			// This assumes the PhysBody listener is pointing to the Snail
			Snail* snail = dynamic_cast<Snail*>(otherBody->listener);
			if (snail) {

				printf("Cast exitoso!\n");

				// Agregar caracol si no está ya en la lista
				if (std::find(snailsInContact.begin(), snailsInContact.end(), snail) == snailsInContact.end()) {
					snailsInContact.push_back(snail);

					printf("Caracol agregado! Total: %d\n", snailsInContact.size());
				}
				else {
					printf("Caracol ya estaba en la lista\n");
				}
			}
			else {
				printf("ERROR: Cast a Snail FALLÓ\n");
			}
		}
		else {
			printf("NO es un SNAIL\n");
		}
	}
}

void Saliva::EndCollision(PhysBody* otherBody)
{
	if (!otherBody) return;

	// Verify if is a snail
	if (otherBody->ctype == ColliderType::SNAIL) {
		Snail* snail = dynamic_cast<Snail*>(otherBody->listener);
		if (snail) {
			snail->isOnSaliva = false;

			// Remove snail from the list
			snailsInContact.erase(
				std::remove(snailsInContact.begin(), snailsInContact.end(), snail),
				snailsInContact.end()
			);
		}
	}
}

void Saliva::ShrinkBody()
{
	if (!body || !body->body) return;

	// Percentage of life created (1.0 = created now, 0.0 = expired)
	float lifePercent = 1.0f - (timer / lifeTime);

	// If it is almost expired, it completely destroys the body.
	if (lifePercent <= 0.15f) {
		// Clean snail list in contact
		snailsInContact.clear();

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
	// Returns a value between 0.0 (invisible) and 1.0 (totally visible)
	float alpha = 1.0f - (timer / lifeTime);
	return alpha < 0.0f ? 0.0f : alpha;
}