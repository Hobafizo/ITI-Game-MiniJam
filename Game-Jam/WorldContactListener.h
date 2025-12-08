#pragma once
#include "bfObject.h"

class WorldContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};