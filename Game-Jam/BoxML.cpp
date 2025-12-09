#include "BoxML.h"
#include "def.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "bfPlayer.h"
#include "bfMonster.h"
#include "bfWall.h"
#include "bfSpeedWall.h"
#include "bfKey.h"
#include "bfDoor.h"

#include<SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <cmath>

#define WALL_SPEED_INC_MULTIPLIER 2.5f
#define WALL_SPEED_INC_TIME       5.0f
#define PI           (22 / 7.0f)

#define PLAYGROUND_MARGIN_LEFT   323
#define PLAYGROUND_MARGIN_RIGHT  323
#define PLAYGROUND_MARGIN_TOP    175
#define PLAYGROUND_MARGIN_BOTTOM 185

#define WALL_VERTICAL_WIDTH  103
#define WALL_VERTICAL_HEIGHT 246

static std::random_device randomDevice;
static std::mt19937 randGenerator(randomDevice());

BoxML* BoxML::_instance = nullptr;

// --- HELPER: ROTATE VISUALS ---
void ApplyRotation(bfObject* obj, float angleInRadians)
{
	if (!obj) return;
	float angleDeg = angleInRadians * 180.0f / PI;
	sf::Transformable* trans = dynamic_cast<sf::Transformable*>(obj->Drawable());
	if (trans) trans->setRotation(angleDeg);
}

BoxML::BoxML(unsigned short screenWidth, unsigned short screenHeight, unsigned short screenPixelPerUnit,
	float timeStep, int32 velocityIterations, int32 positionIterations)
	: _gravity(0.0f, 0.0f), _world(_gravity),
	_screenWidth(screenWidth), _screenHeight(screenHeight), _screenPixelPerUnit(screenPixelPerUnit),
	_timeStep(timeStep), _frameRefreshRate((int)_timeStep * 1000),
	_velocityIterations(velocityIterations), _positionIterations(positionIterations),
	_contactListener(this), _player(nullptr)
{
	_world.SetContactListener(&_contactListener);
	_placedWall = nullptr;
	_placedSpeedWall = nullptr;
	_placedMonster = nullptr;
	_previewRotation = 0.0f;
	_loseBuffer.loadFromFile("Assets/Audio/Creepy Bell Sound Effect - KiiroKarol.wav");
	_loseSound.setBuffer(_loseBuffer);
	_winBuffer.loadFromFile("Assets/Audio/Won.wav");
	_winSound.setBuffer(_winBuffer);
	_levelMusic.openFromFile("Assets/Audio/Forbidden Friends.wav");
	_levelMusic.setLoop(true);
	_wallBuffer.loadFromFile("Assets/Audio/Ocean Wave - Sound Effect - RazendeGijs.wav");
	_wallSound.setBuffer(_wallBuffer);
}

BoxML::~BoxML(void)
{
	ClearObjects();
}

void BoxML::CreateWorld(void)
{
	ClearObjects();

	LoadBackground2("Assets/Enviroment/Water_Filter.png", sf::Color(255, 255, 255, 150));
	LoadBackground("Assets/background/sea.png");

	if (_player)
		RemoveObject(_player);

	_player = CreatePlayer(b2_dynamicBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT + 50, PLAYGROUND_MARGIN_TOP + 50 }), { 165, 192 }, 0.01f, 0.3f);
	_player->Body()->SetLinearVelocity({ PLAYER_SPEED_X, PLAYER_SPEED_Y });

	bfWall* wall;

	wall = CreateWall(b2_staticBody, pixelToMeter({ 800, 500 }), { WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT }, 0.01f, 0.3f, (uint16)ObjectCategory::SpeedWall_Vertical);

	wall = CreateWall(b2_staticBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT - WALL_VERTICAL_WIDTH, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ (float)_screenWidth - PLAYGROUND_MARGIN_RIGHT, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ 0, PLAYGROUND_MARGIN_TOP - WALL_VERTICAL_WIDTH }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ 0, (float)_screenHeight - PLAYGROUND_MARGIN_BOTTOM }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);

	// 4. RESTORED: MONSTER
	bfMonster* monster = CreateMonster(b2_dynamicBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT + 600, PLAYGROUND_MARGIN_TOP + 50 }), { 108, 76 }, 0.01f, 0.3f, 1);
	monster->setMovePattern(Monster_MovePattern::Down);

	monster = CreateMonster(b2_dynamicBody, pixelToMeter({ (float)_screenWidth - 600, PLAYGROUND_MARGIN_TOP + 50 }), { 83, 87 }, 0.01f, 0.3f, 2);
	monster->setMovePattern(Monster_MovePattern::Down);

	monster = CreateMonster(b2_dynamicBody, pixelToMeter({ (float)_screenWidth - 600, PLAYGROUND_MARGIN_TOP + 50}), { 94, 56 }, 0.01f, 0.3f, 3);
	monster->setMovePattern(Monster_MovePattern::Right);

	CreateKey(b2_staticBody, pixelToMeter({ 500, 500 }), { 71, 82 }, 0.01f, 0.3f);
	CreateDoor(b2_staticBody, pixelToMeter({ 500, 800 }), { 202, 298 }, 0.01f, 0.3f);
}


void BoxML::HandleInput(sf::RenderWindow& window, sf::Event& event)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

	UpdatePreviewObject(worldPos);

	if (event.type == sf::Event::KeyPressed) {
		HandleKeyPress(event.key.code);
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			PlacePreviewObject();
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			HandleRightClick(worldPos);
		}
	}
}

void BoxML::LoadPositions(void)
{
	bfObject* obj;
	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj) continue;
		obj->setSfPosition(meterToPixel(obj->getB2Position()));
		if (obj->Body()) ApplyRotation(obj, obj->Body()->GetAngle());
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
		if (!cur) continue;
		if (cur == obj)
		{
			//if (cur->Body()) _world.DestroyBody(cur->Body());
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
		if (!cur) continue;
		//if (cur->Body()) _world.DestroyBody(cur->Body());
		delete cur;
		*it = NULL;
	}
	_objs.clear();
}

bfCircle* BoxML::CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density, float friction, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	// Convert pixel radius to meters
	float rMeters = radius / _screenPixelPerUnit;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(rMeters, rMeters);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = rMeters;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	if (categoryBits) fixtureDef.filter.categoryBits = categoryBits;
	if (maskBits) fixtureDef.filter.maskBits = maskBits;
	body->CreateFixture(&fixtureDef);

	bfCircle* bfObj = new bfCircle(body, radius);
	body->GetUserData().pointer = (uintptr_t)bfObj;
	AddObject(bfObj);
	return bfObj;
}

bfRectangle* BoxML::CreateRectangle(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, uint16 categoryBits, uint16 maskBits, bool addToWorld)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	// Get full size in meters
	b2Vec2 bSize = pixelToMeter(size);
	// Calculate half-extents
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

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
	body->GetUserData().pointer = (uintptr_t)bfObj;
	if (addToWorld) AddObject(bfObj);
	return bfObj;
}

bfPlayer* BoxML::CreatePlayer(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, bool loadSprite)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 1.0f;
	fixtureDef.filter.categoryBits = (uint16)ObjectCategory::Player;
	body->CreateFixture(&fixtureDef);

	bfPlayer* bfObj = new bfPlayer(body, size);
	body->GetUserData().pointer = (uintptr_t)bfObj;
	AddObject(bfObj);

	if (loadSprite)
		bfObj->loadSpriteSheet("Assets/characters/frog.png", size.x, size.y, 0, 0, 5, 5, _timer.getElapsedTime().asSeconds(), 0.17f);

	bfObj->setScale({ 0.7f, 0.7f });

	return bfObj;
}

bfMonster* BoxML::CreateMonster(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, unsigned char spirteIndex)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.filter.categoryBits = (uint16)ObjectCategory::Monster;
	body->CreateFixture(&fixtureDef);

	bfMonster* bfObj = new bfMonster(body, size);
	AddObject(bfObj);

	switch (spirteIndex)
	{
	case 1:
		bfObj->loadSpriteSheet("Assets/monsters/1.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.17);
		bfObj->setScale({ 0.8f, 0.8f });
		break;

	case 2:
		bfObj->loadSpriteSheet("Assets/monsters/2.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.15);
		break;

	case 3:
		bfObj->loadSpriteSheet("Assets/monsters/3.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.17);
		break;
	}	

	return bfObj;
}

bfWall* BoxML::CreateWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, uint16 categoryBits, uint16 maskBits, bool addToWorld, bool loadSprite, bool invisible)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

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
	if (addToWorld)
		AddObject(bfObj);

	if (invisible)
		bfObj->setFillColor(sf::Color::Transparent);

	else if (loadSprite)
	{
		if (isObject((ObjectCategory)categoryBits,
			(ObjectCategory)((uint16)ObjectCategory::Wall_Vertical | (uint16)ObjectCategory::SpeedWall_Vertical)
		))
		{
			std::uniform_int_distribution<int> typeNum(1, 2);

			switch (typeNum(randGenerator))
			{
			case 1:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_1.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 2:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_2.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;
			}
		}

		else if (isObject((ObjectCategory)categoryBits,
			(ObjectCategory)((uint16)ObjectCategory::Wall_Horizontal | (uint16)ObjectCategory::SpeedWall_Horizontal)
		))
		{
			std::uniform_int_distribution<int> typeNum(1, 3);

			switch (typeNum(randGenerator))
			{
			case 1:
				bfObj->loadSpriteSheet("Assets/walls/wall_h_1.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 2:
				bfObj->loadSpriteSheet("Assets/walls/wall_h_2.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 3:
				bfObj->loadSpriteSheet("Assets/walls/wall_h_3.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;
			}
		}
	}

	return bfObj;
}

bfKey* BoxML::CreateKey(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, bool loadSprite)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.filter.categoryBits = (uint16)ObjectCategory::Key;
	body->CreateFixture(&fixtureDef);

	bfKey* bfObj = new bfKey(body, size);
	AddObject(bfObj);

	if (loadSprite)
		bfObj->loadSpriteSheet("Assets/Enviroment/key.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);

	return bfObj;
}

bfDoor* BoxML::CreateDoor(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, bool loadSprite)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	// --- ADJUSTMENT START ---
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	// Shift origin from Top-Left to Center
	bodyDef.position = position + b2Vec2(hx, hy);
	// --- ADJUSTMENT END ---

	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.filter.categoryBits = (uint16)ObjectCategory::Gate;
	body->CreateFixture(&fixtureDef);

	bfDoor* bfObj = new bfDoor(body, size);
	AddObject(bfObj);

	if (loadSprite)
		bfObj->loadSpriteSheet("Assets/Enviroment/Gate_Closed.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);

	return bfObj;
}

bool BoxML::LoadBackground(const std::string& imagePath, sf::Color color)
{
	if (!_backgroundTexture.loadFromFile(imagePath, {}))
		return false;

	_background.setTexture(_backgroundTexture);
	_background.setColor(color);
	return true;
}

bool BoxML::LoadBackground2(const std::string& imagePath, sf::Color color)
{
	if (!_backgroundTexture2.loadFromFile(imagePath, {}))
		return false;

	_background2.setTexture(_backgroundTexture2);
	_background2.setColor(color);
	return true;
}

void BoxML::Step(void)
{
	_levelMusic.play();
	if (_frameTimer.getElapsedTime().asMilliseconds() < _frameRefreshRate)
		return;

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
		(*it)->updatePosition(_timer.getElapsedTime().asSeconds());

	_world.Step(_timeStep, _velocityIterations, _positionIterations);
	_frameTimer.restart();
}

void BoxML::Render(sf::RenderWindow& mainWnd)
{
	bfObject* obj;

	mainWnd.clear();

	mainWnd.draw(_background);
	mainWnd.draw(_background2);

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj)
			continue;

		obj->setSfPosition(meterToPixel(obj->getB2Position()));

		// --- FIX START ---
		// Apply rotation to ALL objects that have a body, not just the player.
		if (obj->Body())
		{
			ApplyRotation(obj, obj->Body()->GetAngle());
		}
	

		mainWnd.draw(*obj->Drawable());
	}

	if (_previewObject)
	{
		_previewObject->setSfPosition(meterToPixel(_previewObject->getB2Position()));

		// Rotation for Preview
		if (_previewObject->Body()) ApplyRotation(_previewObject, _previewObject->Body()->GetAngle());
		else ApplyRotation(_previewObject, _previewRotation);

		mainWnd.draw(*_previewObject->Drawable());
	}
	mainWnd.display();
}


void BoxML::OnBeginContact(b2Contact* contact)
{

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (isObject(ObjectCategory::Player, fixtureA))
		OnPlayerContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Player, fixtureB))
		OnPlayerContact(fixtureB, fixtureA);

	if (isObject(ObjectCategory::Monster, fixtureA))
		OnMonsterContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Monster, fixtureB))
		OnMonsterContact(fixtureB, fixtureA);

	if (isObject(ObjectCategory::Key, fixtureA))
		OnKeyContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Key, fixtureB))
		OnKeyContact(fixtureB, fixtureA);

	if (isObject(ObjectCategory::Gate, fixtureA))
		OnDoorContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Gate, fixtureB))
		OnDoorContact(fixtureB, fixtureA);
}

void BoxML::OnEndContact(b2Contact* contact)
{
}

void BoxML::OnPostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void BoxML::OnPlayerContact(b2Fixture* player, b2Fixture* object)
{
	if (!player || !object) return;
	uint16 objCategory = object->GetFilterData().categoryBits;

	if (isObject((ObjectCategory)objCategory,
		(ObjectCategory)((uint16)ObjectCategory::Wall_Vertical | (uint16)ObjectCategory::Wall_Horizontal | (uint16)ObjectCategory::SpeedWall_Vertical | (uint16)ObjectCategory::SpeedWall_Horizontal)))
	{

		_wallSound.play();
		OnPlayerWallContact(player, object, objCategory);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Monster))
	{
		_levelMusic.stop();
		_loseSound.play();
		printf("Player hit monster\n");
	}
}

void BoxML::OnPlayerWallContact(b2Fixture* player, b2Fixture* wall, uint16 objCategory)
{
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * PI);

	b2Body* playerBody = player->GetBody();
	b2Vec2 currentVelocity = playerBody->GetLinearVelocity();
	float currentSpeed = currentVelocity.Length();
	if (currentSpeed < 0.1f) currentSpeed = 5.0f;

	float randomAngle = angleDist(randGenerator);
	b2Vec2 randomDir(cos(randomAngle), sin(randomAngle));

	// Generalized bounce
	if (isObject((ObjectCategory)objCategory, ObjectCategory::Wall_Vertical))
	{
		if (currentVelocity.x > 0 && randomDir.x > 0 || currentVelocity.x < 0 && randomDir.x < 0) randomDir.x = -randomDir.x;
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Wall_Horizontal))
	{
		if (currentVelocity.y > 0 && randomDir.y > 0 || currentVelocity.y < 0 && randomDir.y < 0) randomDir.y = -randomDir.y;
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::SpeedWall_Vertical))
	{
		if (currentVelocity.x > 0 && randomDir.x > 0 || currentVelocity.x < 0 && randomDir.x < 0) randomDir.x = -randomDir.x;
		_player->setBoostMultiplier(WALL_SPEED_INC_MULTIPLIER);
		_player->setSpeedBoostTimer(_timer.getElapsedTime().asSeconds() + WALL_SPEED_INC_TIME);
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::SpeedWall_Horizontal))
	{
		if (currentVelocity.y > 0 && randomDir.y > 0 || currentVelocity.y < 0 && randomDir.y < 0) randomDir.y = -randomDir.y;
		_player->setBoostMultiplier(WALL_SPEED_INC_MULTIPLIER);
		_player->setSpeedBoostTimer(_timer.getElapsedTime().asSeconds() + WALL_SPEED_INC_TIME);
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
}

void BoxML::OnMonsterContact(b2Fixture* monster, b2Fixture* object)
{
	if (!monster || !object) return;
	b2Body* monsterBody = monster->GetBody();
	bfMonster* monsterObj = findObjectByBody<bfMonster>(monsterBody);
	if (!monsterBody || !monsterObj) return;

	uint16 objCategory = object->GetFilterData().categoryBits;

	if (isObject((ObjectCategory)objCategory,
		(ObjectCategory)((uint16)ObjectCategory::Wall_Vertical
			| (uint16)ObjectCategory::Wall_Horizontal
			| (uint16)ObjectCategory::SpeedWall_Vertical
			| (uint16)ObjectCategory::SpeedWall_Horizontal
			| (uint16)ObjectCategory::Wall
			| (uint16)ObjectCategory::SpeedWall
			)))
	{
		monsterObj->setMovePattern(monsterObj->toggleMovementPattern());
	}
}

void BoxML::OnKeyContact(b2Fixture* key, b2Fixture* object)
{
	bfKey* keyObj = findObjectByBody<bfKey>(key->GetBody());
	if (!keyObj)
		return;

	bfDoor* doorObj = findObjectByBody<bfDoor>(ObjectCategory::Gate);
	if (!doorObj)
		return;

	RemoveObject(keyObj);

	doorObj->loadSpriteSheet("Assets/Enviroment/Gate_Open.png", 202, 298, 0, 0, 1, 1, 0, 0, true);
}

void BoxML::OnDoorContact(b2Fixture* door, b2Fixture* object)
{
	bfKey* keyObj = findObjectByBody<bfKey>(ObjectCategory::Key);
	if (keyObj)
		return;

	std::cout << "YOU WINNNNNNNN!" << std::endl;

	_winSound.play();
	_levelMusic.stop();

	//CreateWorld();
}

sf::Vector2u BoxML::Resolution(void) const
{
	return { _screenWidth, _screenHeight };
}

b2Vec2 BoxML::pixelToMeter(const sf::Vector2f pixel) const
{
	return b2Vec2{ pixel.x / _screenPixelPerUnit, pixel.y / _screenPixelPerUnit };
}

sf::Vector2f BoxML::meterToPixel(const b2Vec2 meter) const
{
	return sf::Vector2f{ meter.x * _screenPixelPerUnit, meter.y * _screenPixelPerUnit };
}

template<typename T>
T* BoxML::findObjectByBody(b2Body* body)
{
	bfObject* obj;
	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj) continue;
		if (obj->Body() == body) return (T*)obj;
	}
	return nullptr;
}

template<typename T>
T* BoxML::findObjectByBody(const ObjectCategory category)
{
	bfObject* obj;
	b2Body* body;

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		obj = *it;
		if (!obj)
			continue;

		body = obj->Body();
		if (!body)
			continue;

		b2Fixture* fixture = body->GetFixtureList();
		if (!fixture)
			continue;

		if (isObject(category, fixture))
			return (T*)obj;
	}
	return nullptr;
}

b2Vec2 BoxML::centerAround(const b2Vec2 size, const sf::Vector2f targetPosition, const sf::Vector2f targetSize) const
{
	b2Vec2 tPos = pixelToMeter(targetPosition);
	b2Vec2 tSize = pixelToMeter(targetSize);
	tPos.x += tSize.x / 2;
	tPos.y += tSize.y / 2;
	return tPos;
}

bool BoxML::isObject(const ObjectCategory category, const ObjectCategory object) const
{
	return ((uint16)object & (uint16)category);
}

bool BoxML::isObject(const ObjectCategory category, b2Fixture* fixture) const
{
	if (!_player || !fixture) return false;
	return isObject(category, (ObjectCategory)fixture->GetFilterData().categoryBits);
}



void BoxML::HandleKeyPress(sf::Keyboard::Key key)
{
	bool typeChanged = false;

	if (key == sf::Keyboard::Num1) { _currentPreviewType = ObjectCategory::Wall; typeChanged = true; }
	else if (key == sf::Keyboard::Num2) { _currentPreviewType = ObjectCategory::SpeedWall; typeChanged = true; }
	//else if (key == sf::Keyboard::Num3) { _currentPreviewType = ObjectCategory::Monster; typeChanged = true; }

	if (key == sf::Keyboard::R) {
		_previewRotation += 90.0f * (PI / 180.0f);
		if (_previewObject && _previewObject->Body()) {
			_previewObject->Body()->SetTransform(_previewObject->Body()->GetPosition(), _previewRotation);
		}
	}

	if (typeChanged) {
		_previewRotation = 0.0f;
		if (_previewObject) {
			//if (_previewObject->Body()) _world.DestroyBody(_previewObject->Body());
			delete _previewObject;
			_previewObject = nullptr;
		}
	}
}

void BoxML::UpdatePreviewObject(const sf::Vector2f& pixelMousePos)
{
	bool alreadyHasThisType = false;
	if (_currentPreviewType == ObjectCategory::Wall && _placedWall != nullptr) alreadyHasThisType = true;
	if (_currentPreviewType == ObjectCategory::SpeedWall && _placedSpeedWall != nullptr) alreadyHasThisType = true;
	if (_currentPreviewType == ObjectCategory::Monster && _placedMonster != nullptr) alreadyHasThisType = true;

	if (alreadyHasThisType) {
		if (_previewObject) {
			//if (_previewObject->Body()) _world.DestroyBody(_previewObject->Body());
			delete _previewObject;
			_previewObject = nullptr;
		}
		return;
	}

	b2Vec2 mouseMeters = pixelToMeter(pixelMousePos);

	if (_previewObject == nullptr)
	{
		// FIX: Increased size from 50x20 to 250x60 so it's visible
		sf::Vector2f size(250.f, 60.f);

		switch (_currentPreviewType)
		{
		case ObjectCategory::Wall:
		{
			// Load Horizontal Wall Sprite
			bfWall* w = CreateWall(b2_staticBody, mouseMeters, size, 0.0f, 0.0f,
				(uint16)ObjectCategory::Wall_Horizontal, 0, false, true, false);

			// REMOVED: w->setColor(...) -> Now shows full opaque sprite
			_previewObject = w;
		}
		break;
		case ObjectCategory::SpeedWall:
		{
			// Load Horizontal Speed Wall Sprite
			bfWall* w = CreateWall(b2_staticBody, mouseMeters, size, 0.0f, 0.0f,
				(uint16)ObjectCategory::SpeedWall_Horizontal, 0, false, true, false);

			_previewObject = w;
		}
		break;
		case ObjectCategory::Monster:
		{
			// FIX: Increased monster size from 15x15 to 80x80
			bfMonster* m = CreateMonster(b2_dynamicBody, mouseMeters, { 80.0f, 80.0f }, 0.0f, 0.0f, 1);

			// Remove from main list so it doesn't get standard updates
			bool removed = false;
			for (auto it = _objs.begin(); it != _objs.end(); ++it) {
				if (*it == m) { _objs.erase(it); removed = true; break; }
			}

			_previewObject = m;
		}
		break;
		}

		if (_previewObject && _previewObject->Body()) {
			_previewObject->Body()->SetGravityScale(0.0f);
			b2Fixture* fixture = _previewObject->Body()->GetFixtureList();
			fixture->SetSensor(true);
			b2Filter filter;
			filter.categoryBits = 0;
			filter.maskBits = 0;
			fixture->SetFilterData(filter);
		}
	}

	if (_previewObject && _previewObject->Body())
	{
		_previewObject->Body()->SetTransform(mouseMeters, _previewRotation);
		_previewObject->setSfPosition(pixelMousePos);
		ApplyRotation(_previewObject, _previewRotation);
		_previewObject->Body()->SetLinearVelocity(b2Vec2(0, 0));
		_previewObject->Body()->SetAngularVelocity(0);
	}
}

void BoxML::PlacePreviewObject()
{
	if (!_previewObject) return;
	b2Vec2 pos = _previewObject->getB2Position();

	// FIX: Make sure the placed object uses the same larger size
	sf::Vector2f size(250.f, 60.f);

	bfObject* createdObj = nullptr;

	switch (_currentPreviewType)
	{
	case ObjectCategory::Wall:
		if (_placedWall == nullptr) {
			createdObj = CreateWall(b2_staticBody, pos, size, 0.01f, 0.3f,
				(uint16)ObjectCategory::Wall_Horizontal, 0, true, true, false);
			_placedWall = createdObj;
		}
		break;
	case ObjectCategory::SpeedWall:
		if (_placedSpeedWall == nullptr) {
			createdObj = CreateWall(b2_staticBody, pos, size, 0.01f, 0.3f,
				(uint16)ObjectCategory::SpeedWall_Horizontal, 0, true, true, false);
			_placedSpeedWall = createdObj;
		}
		break;
	case ObjectCategory::Monster:
		if (_placedMonster == nullptr) {
			// FIX: Use 80x80 size for the placed monster too
			bfMonster* m = CreateMonster(b2_dynamicBody, pos, { 80.0f, 80.0f }, 0.01f, 0.3f, 1);
			createdObj = m;
			_placedMonster = m;
		}
		break;
	}

	if (createdObj && createdObj->Body()) {
		createdObj->Body()->SetTransform(pos, _previewRotation);
	}
}
void BoxML::HandleRightClick(const sf::Vector2f& pixelMousePos)
{
	b2Vec2 mouseMeters = pixelToMeter(pixelMousePos);

	bfObject* objectsToCheck[] = { _placedWall, _placedSpeedWall, _placedMonster };
	for (bfObject* obj : objectsToCheck) {
		if (obj && obj->Body()) {
			for (b2Fixture* f = obj->Body()->GetFixtureList(); f; f = f->GetNext()) {
				if (f->TestPoint(mouseMeters)) {
					// Found the object! Clear the specific pointer.
					if (obj == _placedWall) _placedWall = nullptr;
					if (obj == _placedSpeedWall) _placedSpeedWall = nullptr;
					if (obj == _placedMonster) _placedMonster = nullptr;

					RemoveObject(obj);
					return;
				}
			}
		}
	}
}

void BoxML::setInstance(BoxML* world)
{
	_instance = world;
}

BoxML* BoxML::Instance(void)
{
	return _instance;
}
