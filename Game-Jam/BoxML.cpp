#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include <random>

#define G_CONST               20.0f
#define SUN_SIZE              100.0f
#define SUN_MASS              500.0f

#define PLANET_SIZE_MIN       10.0f
#define PLANET_SIZE_MAX       25.0f
#define TRAIL_SIZE_MAX        2000

#define SUN_PLANET_RADIUS_MIN 15.0f
#define SUN_PLANET_RADIUS_MAX 60.0f

BoxML::BoxML(unsigned short screenWidth, unsigned short screenHeight, unsigned short screenPixelPerUnit,
	float timeStep, int32 velocityIterations, int32 positionIterations)
	: _gravity(0.0f, 0.0f), _world(_gravity),
	_screenWidth(screenWidth), _screenHeight(screenHeight), _screenPixelPerUnit(screenPixelPerUnit),
	_timeStep(timeStep), _frameRefreshRate((int)_timeStep * 1000),
	_velocityIterations(velocityIterations), _positionIterations(positionIterations)
{
}

BoxML::~BoxML(void)
{
	ClearObjects();
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

void BoxML::CreateWorld(void)
{
	_sun = CreateCircle(b2_staticBody,
		centerAround(pixelToMeter({ SUN_SIZE * 2, SUN_SIZE * 2 }), sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ (float)_screenWidth, (float)_screenHeight }),
		SUN_SIZE);

	_sun->setFillColor(sf::Color(253, 184, 19));

	bfCircle* circle = CreateCircle(b2_dynamicBody, b2Vec2{ 10, 10 }, 10);
	circle->Body()->ApplyForceToCenter({ 5, 2 }, true);
	circle->Body()->ApplyForceToCenter({ 5, 2 }, true);
	circle->Body()->ApplyForceToCenter({ 5, 2 }, true);
}

void BoxML::SpawnPlanetAt(const sf::Vector2f position)
{
	float radius = PLANET_SIZE_MIN;
	sf::Color color = sf::Color::Blue;

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> radiusDist(PLANET_SIZE_MIN, PLANET_SIZE_MAX);
	std::uniform_int_distribution<unsigned int> colorDist(0, 0xFFFFFFFF);

	radius = radiusDist(eng);
	color = sf::Color(colorDist(eng));
	color.a = 255;

	CreatePlanet(pixelToMeter(position), radius, color);
}

void BoxML::CreatePlanet(const b2Vec2 position, float radius, const sf::Color color)
{
	bfCircle* planet = CreateCircle(b2_dynamicBody, position, radius);
	planet->setFillColor(color);

	b2Vec2 toSun = _sun->getB2Position() - position;

	b2Vec2 normalizedSun = toSun;
	normalizedSun.Normalize();
	b2Vec2 sunNormal(-normalizedSun.y, normalizedSun.x);

	// For a circular orbit, v = sqrt(G * Mass_Sun / r)
	float distance = toSun.Length();
	float initialVelocity = sqrt(G_CONST * SUN_MASS / distance);

	planet->Body()->SetLinearVelocity(initialVelocity * sunNormal);

	_planets.push_back({ planet });
}

void BoxML::RemovePlanet(b2Body* body)
{
	for (auto it = _planets.begin(); it != _planets.end(); ++it)
	{
		if (it->Planet->Body() == body)
		{
			RemoveObject(it->Planet);

			_planets.erase(it);
			return;
		}
	}
}

void BoxML::ApplyGravity(void)
{
	if (!_sun)
		return;

	for (auto it = _planets.begin(); it != _planets.end();)
	{
		if (ApplyGravity(_sun, *it))
			++it;

		// went out of range or too close to sun
		else
		{
			RemoveObject(it->Planet);
			it = _planets.erase(it);
		}
	}
}

bool BoxML::ApplyGravity(bfObject* sun, PlanetInfo& planet)
{
	b2Body* bSun = sun->Body();
	b2Body* bPlanet = planet.Planet->Body();

	// 1. Get distance vector
	b2Vec2 distVec = bSun->GetPosition() - bPlanet->GetPosition();
	float distance = distVec.Length();

	if (distance <= SUN_PLANET_RADIUS_MIN || distance >= SUN_PLANET_RADIUS_MAX)
		return false;

	// 2. Normalize vector (direction only)
	distVec.Normalize();

	// 3. Calculate Force (F = G * m1 * m2 / r^2)
	// "G" is a magic constant you tweak until it feels fun. Try 50.0f first.
	float strength = (G_CONST * SUN_MASS * bPlanet->GetMass()) / (distance * distance);

	// 4. Apply Force
	b2Vec2 force = strength * distVec;
	bPlanet->ApplyForceToCenter(force, true);

	return true;
}

void BoxML::UpdatePlanetTrail(PlanetInfo& planet)
{
	sf::CircleShape* planetShape = (sf::CircleShape*)planet.Planet->Drawable();

	if (planet.Trail.getVertexCount() >= TRAIL_SIZE_MAX)
	{
		sf::VertexArray copyTrail;

		for (int i = TRAIL_SIZE_MAX / 2; i < TRAIL_SIZE_MAX; ++i)
			copyTrail.append(planet.Trail[i]);

		planet.Trail.clear();

		for (int i = 0; i < TRAIL_SIZE_MAX / 2; ++i)
			planet.Trail.append(copyTrail[i]);
	}

	planet.Trail.append(sf::Vertex(planet.Planet->getSfPosition(), planetShape ? planetShape->getFillColor() : sf::Color::Blue));
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

bfCircle* BoxML::CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density, float friction)
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

	body->CreateFixture(&fixtureDef);

	bfCircle* bfObj = new bfCircle(body, radius);

	AddObject(bfObj);
	return bfObj;
}

bfRectangle* BoxML::CreateRectangle(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction)
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

	body->CreateFixture(&fixtureDef);

	bfRectangle* bfObj = new bfRectangle(body, size);

	AddObject(bfObj);
	return bfObj;
}

void BoxML::Step(void)
{
	ApplyGravity();

	if (_timer.getElapsedTime().asMilliseconds() < _frameRefreshRate)
		return;

	_world.Step(_timeStep, _velocityIterations, _positionIterations);
	_timer.restart();
}

void BoxML::Render(sf::RenderWindow& mainWnd)
{
	bfObject* obj;

	mainWnd.clear();

	for (auto it = _planets.begin(); it != _planets.end(); ++it)
	{
		UpdatePlanetTrail(*it);
		mainWnd.draw(it->Trail);
	}

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
