#include "WorldContactListener.h"
#include <iostream>

void WorldContactListener::BeginContact(b2Contact* contact)
{
    // Called when collision begins
    void* bodyUserDataA = (void*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    void* bodyUserDataB = (void*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    // Identify objects via user data
    std::cout << "Collision began!\n";
}

void WorldContactListener::EndContact(b2Contact* contact)
{
    std::cout << "Collision ended!\n";
}

void WorldContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    float force = impulse->normalImpulses[0];
    std::cout << "Collision force: " << force << "\n";
}
