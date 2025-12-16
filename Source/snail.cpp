#include "Snail.h"
#include "SnailAI.h"
#include "ModuleGame.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Map.h"
#include "Saliva.h"
#include "AdoYell.h"
#include "AdoSnail.h"

#include <vector>

void Snail::Update()
{
	if (active)
	{
		aiInputDir = b2Vec2(0.0f, 0.0f);
		Move();            // player
	}
	else if (isAI && ai)
	{
		ai->Update();      // AI drives input
		Move();
		aiInputDir = b2Vec2(0.0f, 0.0f); // SAME movement

		if (aiLaps < 3) {
			aiRaceTime += GetFrameTime();
		}
	}


	if (active) {
		if (IsKeyPressed(KEY_SPACE) && !isSlobber) {
			isSlobber = true;
		}
		Hability();
	}

	for (auto s : salives) {
		s->Update();
	}

	Box::Update();
}

void Snail::Move()
{
	b2Vec2 inputDir(0.0f, 0.0f);

	if (active) {
		if (IsKeyDown(KEY_W)) {
			inputDir.y -= 1.0f;
		}
		if (IsKeyDown(KEY_S)) {
			inputDir.y += 1.0f;
		}
		if (IsKeyDown(KEY_A)) {
			inputDir.x -= 1.0f;
		}
		if (IsKeyDown(KEY_D)) {
			inputDir.x += 1.0f;
		}
	}
	else if (isAI)
	{
		// AI input (set by SnailAI::Update via SetAIInput)
		inputDir = aiInputDir;
	}

	bool has_input = inputDir.LengthSquared() > 0.0f;

	// normalize input direction if going in diagonal
	if (has_input) {
		inputDir.Normalize();
	}

	// snail's current forward direction
	float angle = body->body->GetAngle() - 90 * DEG2RAD;
	forward = { float(cos(angle)), float(sin(angle)) };

	// apply force to the "forward" direction (direction snail is facing)
	b2Vec2 force(0.0f, 0.0f);
	if (has_input) {
		float forwardInput = b2Dot(inputDir, forward); //checking if player input is same as snail is facing 
		float forceMagnitude = moveForce * fmax(0.3f, fabs(forwardInput)); // minimum 30% force when snail is not facing the input direction. slow when not facing "correct" direction
		force = b2Vec2(forward.x * forceMagnitude, forward.y * forceMagnitude);
	}

	//Apply friction
	ApplyFriction(staticFrictionCoeff, dynamicFrictionCoeff);
	//Apply forward force
	body->body->ApplyForceToCenter(force, true);

	Rotate(inputDir, forward, has_input);

	b2Vec2 right(-forward.y, forward.x);
	ApplyLateralFriction(right);
}

void Snail::ApplyLateralFriction(const b2Vec2& right)
{
	//little bit of drifting!!!

	b2Vec2 vel = body->body->GetLinearVelocity();
	float lateralVel = b2Dot(vel, right);
	b2Vec2 lateralImpulse = b2Vec2(
		-lateralVel * lateralDamping * body->body->GetMass() * right.x,
		-lateralVel * lateralDamping * body->body->GetMass() * right.y
	);

	body->body->ApplyLinearImpulseToCenter(lateralImpulse, true);
}


void Snail::Rotate(b2Vec2 inputDir, const b2Vec2& forward, bool has_input) {
	b2Vec2 vel = body->body->GetLinearVelocity();
	float currentAngle = body->body->GetAngle();

	float targetAngle = currentAngle;
	bool shouldRotate = false;

	// if there's input, rotate towards input direction
	if (has_input) {
		targetAngle = atan2(inputDir.y, inputDir.x) + 90.0f * DEG2RAD;
		shouldRotate = true;
	}
	// if moving but no input, rotate towards velocity
	else if (vel.LengthSquared() > 0.1f) {
		targetAngle = atan2(vel.y, vel.x) + 90.0f * DEG2RAD;
		shouldRotate = true;
	}

	if (shouldRotate)
	{
		//rotates efficiently (takes shortest path to rotate)
		float diff = targetAngle - currentAngle;
		diff = atan2(sinf(diff), cosf(diff));

		// speed-dependent turn rate: slower when moving slowly
		float speed = vel.Length();
		float speed_factor = fminf(1.0f, speed / rotation_speed_scale); // adjust 50.0f based on max speed
		float turnRate = rotation_base_rate * (0.5f + speed_factor * 0.5f) * GetFrameTime();

		//low speed circular movement instead of just rotating in place
		if (speed < low_speed_threshold && has_input)
		{
			// Get current forward direction
			float fAngle = currentAngle - 90 * DEG2RAD;
			b2Vec2 forward(cos(fAngle), sin(fAngle));

			// Small forward push so turning makes a circle instead of rotating in place
			b2Vec2 tiny_push(forward.x * tiny_push_strength, forward.y * tiny_push_strength);
			body->body->ApplyForceToCenter(tiny_push, true);
		}

		//if snail is almost facing the target angle, snap to it, otherwise gradually move toward it
		if (fabs(diff) < turnRate)
			currentAngle = targetAngle;
		else
			currentAngle += turnRate * (diff > 0 ? 1.0f : -1.0f);
		
		// apply to body
		body->body->SetTransform(body->body->GetPosition(), currentAngle);
	}
}

void Snail::ApplyFriction(float staticFriction, float dynamicFriction)
{
	b2Vec2 velocity = body->body->GetLinearVelocity();
	float speedSquared = velocity.LengthSquared();

	//If the snail is moving
	if (speedSquared > 0.001f) {
		//Calculate friction force opposite to velocity direction
		b2Vec2 frictionDir = velocity;
		frictionDir.Normalize();
		frictionDir *= -1.0f;

		//Dynamic friction when moving
		float N = mass * 9.8f;
		float frictionMagnitude = N * dynamicFriction;

		b2Vec2 frictionForce = frictionDir;
		frictionForce *= frictionMagnitude;

		body->body->ApplyForceToCenter(frictionForce, true);
	}
	else if (speedSquared < 0.001f){
		//Step speed if too slow
		body->body->SetLinearVelocity({ 0.0f, 0.0f });
	}
}

void Snail::Trail() {
	b2Vec2 vel = body->body->GetLinearVelocity();
	float speed = vel.Length();

	// Only leave trail when moving
	if (speed > 0.5f)
	{
		trailTimer += GetFrameTime(); // raylib-style frame delta time

		if (trailTimer >= trailInterval)
		{
			trailTimer = 0.0f;
			trail.push_back(GetPosition());
		}
	}

	for (const auto& pos : trail)
	{
		float renderX = pos.x + listener->App->renderer->camera.x;
		float renderY = pos.y + listener->App->renderer->camera.y;
		DrawRectangle((int)renderX - 2, (int)renderY - 2, 4, 4, GRAY);
	}
}

void Snail::Hability()
{
	if (!isSlobber) return;

	salivaTimer += GetFrameTime();
	slobberTimer += GetFrameTime();

	if (salivaTimer >= salivaInterval)
	{
		salivaTimer = 0.0f;
		saliva = new Saliva(
			body->listener->App->physics,
			GetPosition().x,
			GetPosition().y,
			body->listener,
			salivaTexture,
			this
		);

		salives.push_back(saliva);

	}

	if (slobberTimer >= 5.0f) {
		isSlobber = false;
		slobberTimer = 0.0f;
	}

	for (auto it = salives.begin(); it != salives.end();) {
		if (!(*it)->active) {
			delete* it;
			it = salives.erase(it);
		}
		else {
			++it;
		}
	}

}

void Snail::SetMaskBits(uint16 newMaskBits)
{
	b2Fixture* fixture = body->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.maskBits = newMaskBits;
	fixture->SetFilterData(filter);
}

void Snail::RestoreMaskBits()
{
	SetMaskBits(originalMaskBits);
}

Vector2 Snail::GetPosition() const
{
	if (!body)
		return { 0.0f, 0.0f };
    int x = 0, y = 0;
	body->GetPhysicPosition(x, y);
    return { (float)x, (float)y };
}

void Snail::OnCollisionWithMap(PhysBody* mapObject)
{
	//Checks for the type of map object the snail collided with
	if (mapObject == nullptr) {
		return;
	}
	switch (mapObject->ctype) {
		case ColliderType::CHECKPOINT: {
			int checkpointNum = mapObject->checkpointIndex;
			ModuleGame* game = dynamic_cast<ModuleGame*>(listener);
			game->CheckpointManager(this, checkpointNum);
			break;
		}

		case ColliderType::ICE: {
			//dynamicFrictionCoeff = 0.1f;
			dynamicFrictionCoeff = 2.0f;
			break;
		}

		case ColliderType::MUD: {
			//dynamicFrictionCoeff = 1.0f;
			dynamicFrictionCoeff = 2.0f;
			break;
		}

		case ColliderType::GRASS: {
			//dynamicFrictionCoeff = 0.7f;
			dynamicFrictionCoeff = 2.0f;
			break;
		}

		case ColliderType::SALIVA: {
			bool isOwnSaliva = false;
			for (auto s : salives) {
				if (s->body == mapObject) {
					isOwnSaliva = true;
					break;
				}
			}
			if (!isOwnSaliva) {
				dynamicFrictionCoeff = 1.5f;
			}
			break;
		}

		case ColliderType::ADO_YELL: {
			AdoSnail* adoSnail = dynamic_cast<AdoSnail*>(this);
			if (adoSnail) {
				break;
			}
			dynamicFrictionCoeff = 2.0;
			break;
		}
	}
}

void Snail::EndCollisionWithMap(PhysBody* mapObject)
{
	//Checks for the type of map object the snail collided with
	if (mapObject == nullptr) {
		return;
	}
	switch (mapObject->ctype) {
		case ColliderType::ICE: {
			dynamicFrictionCoeff = 0.3f;
			break;
		}

		case ColliderType::MUD: {
			dynamicFrictionCoeff = 0.3f;
			break;
		}

		case ColliderType::GRASS: {
			dynamicFrictionCoeff = 0.3f;
			break;
		}

		case ColliderType::SALIVA: {
			dynamicFrictionCoeff = 0.3f;
			break;
		}

		case ColliderType::ADO_YELL: {
			dynamicFrictionCoeff = 0.3f;
			break;
		}
	}
}




