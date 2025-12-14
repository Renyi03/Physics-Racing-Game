#include "SnailAI.h"
#include "Snail.h"

SnailAI::SnailAI(Snail* owner) : snail(owner) {}

void SnailAI::SetWaypoints(const std::vector<Vector2>& points) { 
	waypoints = points; 
}

void SnailAI::Update() {
    if (!snail || waypoints.empty())
        return;

    Vector2 pos = snail->GetPosition();
    Vector2 target = waypoints[currentWaypoint];

    // Calculate distance to current waypoint
    Vector2 toTarget = {
         target.x - pos.x,
         target.y - pos.y
    };
    float distance = sqrtf(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    // Debug: print occasionally
    static int frameCount = 0;
    if (frameCount++ % 60 == 0) {
        TraceLog(LOG_INFO, "AI pos: (%.1f, %.1f), target wp%d: (%.1f, %.1f), dist: %.1f",
            pos.x, pos.y, currentWaypoint, target.x, target.y, distance);
    }

    // SMALLER THRESHOLD - only advance when very close (checkpoint sensor size)
    if (distance < 50.0f)  // ? Changed from 100 to 50
    {
        TraceLog(LOG_INFO, "AI reached waypoint %d, advancing to %d",
            currentWaypoint, (currentWaypoint + 1) % waypoints.size());
        currentWaypoint = (currentWaypoint + 1) % waypoints.size();
        stuckTimer = 0.0f;

        // Update to new target
        target = waypoints[currentWaypoint];
        toTarget = {
            target.x - pos.x,
            target.y - pos.y
        };
        distance = sqrtf(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    }

    // Simple direct aiming - no look-ahead for now
    // Just aim straight at the current waypoint
    if (distance > 0.01f)
    {
        toTarget.x /= distance;
        toTarget.y /= distance;
    }

    // Set input direction - Snail::Move() handles everything else
    snail->SetAIInput(b2Vec2(toTarget.x, toTarget.y));

    // STUCK RECOVERY
    b2Vec2 vel = snail->body->body->GetLinearVelocity();
    float speed = vel.Length();

    if (speed < 0.3f)
    {
        stuckTimer += GetFrameTime();
        if (stuckTimer > 2.0f)  // ? Increased timeout
        {
            TraceLog(LOG_INFO, "AI snail stuck at (%.1f, %.1f), skipping waypoint %d",
                pos.x, pos.y, currentWaypoint);
            currentWaypoint = (currentWaypoint + 1) % waypoints.size();
            stuckTimer = 0.0f;
        }
    }
    else
    {
        stuckTimer = 0.0f;
    }
}