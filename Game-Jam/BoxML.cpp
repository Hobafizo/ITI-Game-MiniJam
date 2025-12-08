#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "bfPlayer.h"
#include "bfWall.h"
#include "bfSpeedWall.h"
#include "ObjectCategory.h"
#include <iostream>

BoxML::BoxML(unsigned short screenWidth, unsigned short screenHeight, unsigned short screenPixelPerUnit,
	float timeStep, int32 velocityIterations, int32 positionIterations)
	: _gravity(0.0f, 0.0f), _world(_gravity),
	_screenWidth(screenWidth), _screenHeight(screenHeight), _screenPixelPerUnit(screenPixelPerUnit),
	_timeStep(timeStep), _frameRefreshRate((int)_timeStep * 1000),
	_velocityIterations(velocityIterations), _positionIterations(positionIterations),
	_contactListener(this), _player(nullptr)
{
	_world.SetContactListener(&_contactListener);
}

BoxML::~BoxML(void)
{
	ClearObjects();
}

void BoxML::CreateWorld(void)
{
	if (_player)
		RemoveObject(_player);

	_player = CreatePlayer(b2_dynamicBody, b2Vec2{ 10, 10 }, 10, 0.01f, 0.3f);

	_player->Body()->ApplyForceToCenter({ 5, 2 }, true);
	_player->Body()->ApplyForceToCenter({ 5, 4 }, true);
	_player->Body()->ApplyForceToCenter({ 5, 4 }, true);

	bfWall* wall = CreateWall(b2_staticBody, pixelToMeter({ 500, 384 }), { 30, 368 });
}

void BoxML::LoadPositions(void)
{
	bfObject* obj;

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj)
			continue;

		obj->setSfPosition(meterToPixel(obj->getB2Position()));
	}
}

void BoxML::AddObject(bfObject* obj)
{
	_objs.push_back(obj);
}

bool BoxML::RemoveObject(bfObject* obj)
{
	bfObject* cur;

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		cur = *it;
		if (!cur)
			continue;

		if (cur == obj)
		{
			if (cur->Body())
				_world.DestroyBody(cur->Body());
			delete cur;

			_objs.erase(it);
			return true;
		}
	}

	return false;
}

void BoxML::ClearObjects()
{
	bfObject* cur;

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		cur = *it;
		if (!cur)
			continue;

		if (cur->Body())
			_world.DestroyBody(cur->Body());
		delete cur;

		*it = NULL;
	}

	_objs.clear();
}

bfCircle* BoxML::CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density, float friction, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position = position;

	b2Body* body = _world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = radius / _screenPixelPerUnit;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	if (categoryBits)
		fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits)
		fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	bfCircle* bfObj = new bfCircle(body, radius);

	AddObject(bfObj);
	return bfObj;
}

bfRectangle* BoxML::CreateRectangle(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position = position;

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	b2Vec2 bSize = pixelToMeter(size);
	boxShape.SetAsBox(bSize.x / 2, bSize.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	if (categoryBits)
		fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits)
		fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	bfRectangle* bfObj = new bfRectangle(body, size);

	AddObject(bfObj);
	return bfObj;
}

bfPlayer* BoxML::CreatePlayer(const b2BodyType bodyType, const b2Vec2 position, float radius, float density, float friction)
{
	const uint16 categoryBits = (uint16)ObjectCategory::Player, maskBits = 0;

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position = position;

	b2Body* body = _world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = radius / _screenPixelPerUnit;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	if (categoryBits)
		fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits)
		fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	bfPlayer* bfObj = new bfPlayer(body, radius);

	AddObject(bfObj);
	return bfObj;
}

bfWall* BoxML::CreateWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction)
{
	const uint16 categoryBits = (uint16)ObjectCategory::Wall, maskBits = 0;

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position = position;

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	b2Vec2 bSize = pixelToMeter(size);
	boxShape.SetAsBox(bSize.x / 2, bSize.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	if (categoryBits)
		fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits)
		fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	bfWall* bfObj = new bfWall(body, size);

	AddObject(bfObj);
	return bfObj;
}

bfSpeedWall* BoxML::CreateSpeedWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction)
{
	const uint16 categoryBits = (uint16)ObjectCategory::SpeedWall, maskBits = 0;

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position = position;

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	b2Vec2 bSize = pixelToMeter(size);
	boxShape.SetAsBox(bSize.x / 2, bSize.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	if (categoryBits)
		fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits)
		fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	bfSpeedWall* bfObj = new bfSpeedWall(body, size);

	AddObject(bfObj);
	return bfObj;
}

void BoxML::Step(void)
{
	if (_timer.getElapsedTime().asMilliseconds() < _frameRefreshRate)
		return;

	_world.Step(_timeStep, _velocityIterations, _positionIterations);
	_timer.restart();
}

void BoxML::Render(sf::RenderWindow& mainWnd)
{
	bfObject* obj;

	mainWnd.clear();

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj)
			continue;

		obj->setSfPosition(meterToPixel(obj->getB2Position()));
		mainWnd.draw(*obj->Drawable());
	}

	mainWnd.display();
}

void BoxML::OnBeginContact(b2Contact* contact)
{
	// Identify objects via user data
	std::cout << "Collision began!\n";
}

void BoxML::OnEndContact(b2Contact* contact)
{
	std::cout << "Collision ended!\n";
}

void BoxML::OnPostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	float force = impulse->normalImpulses[0];
	std::cout << "Collision force: " << force << "\n";
}

b2Vec2 BoxML::pixelToMeter(const sf::Vector2f pixel) const
{
	return b2Vec2{ pixel.x / _screenPixelPerUnit, pixel.y / _screenPixelPerUnit };
}

sf::Vector2f BoxML::meterToPixel(const b2Vec2 meter) const
{
	return sf::Vector2f{ meter.x * _screenPixelPerUnit, meter.y * _screenPixelPerUnit };
}

b2Vec2 BoxML::centerAround(const b2Vec2 size, const sf::Vector2f targetPosition, const sf::Vector2f targetSize) const
{
	b2Vec2 tPos = pixelToMeter(targetPosition);
	b2Vec2 tSize = pixelToMeter(targetSize);

	tPos.x += tSize.x / 2 /*- size.x / 2*/;
	tPos.y += tSize.y / 2 /*- size.y / 2*/;

	return tPos;
}
