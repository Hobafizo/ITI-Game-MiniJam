#include "WorldContactListener.h"
#include "BoxML.h"

WorldContactListener::WorldContactListener(BoxML* world) : _world(world)
{
}

void WorldContactListener::BeginContact(b2Contact* contact)
{
    _world->OnBeginContact(contact);
}

void WorldContactListener::EndContact(b2Contact* contact)
{
    _world->OnEndContact(contact);
}

void WorldContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    _world->OnPostSolve(contact, impulse);
}
