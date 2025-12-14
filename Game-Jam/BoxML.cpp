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

#define PI (22 / 7.0f)

static std::random_device randomDevice;
static std::mt19937 randGenerator(randomDevice());

BoxML* BoxML::_instance = nullptr;


void ApplyRotation(bfObject* obj, float angleInRadians)
{
	if (!obj)
		return;

	b2Body* body = obj->Body();
	body->GetLinearVelocity();
	body->GetAngularVelocity();

	float angleDeg = angleInRadians * 180.0f / PI;
	sf::Transformable* trans = dynamic_cast<sf::Transformable*>(obj->Drawable());
	if (trans)
		trans->setRotation(angleDeg);
}

void ApplyRotation(bfObject* obj)
{
	if (!obj)
		return;

	b2Body* body = obj->Body();
	if (!body)
		return;

	b2Vec2 vel = body->GetLinearVelocity();

	if (vel.LengthSquared() < 0.0001f)
		return;

	float angleRad = atan2(vel.y, vel.x);
	float angleDeg = angleRad * 180.0f / b2_pi;

	sf::Transformable* trans = dynamic_cast<sf::Transformable*>(obj->Drawable());
	if (trans)
		trans->setRotation(angleDeg + 90.0f);
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
	_wallBuffer.loadFromFile("Assets/Audio/Ocean Wave - Sound Effect - RazendeGijs.wav");
	_wallSound.setBuffer(_wallBuffer);
	_levelMusic.openFromFile("Assets/Audio/Forbidden Friends.wav");
	_LoserMusic.openFromFile("Assets/Audio/YOU DIED (HD) - iwanPlays.wav");
	_winBuffer.loadFromFile("Assets/Audio/Won.wav");
	_winSound.setBuffer(_winBuffer);
	KeypickUpBuffer.loadFromFile("Assets/Audio/Pick Up Keys  Sound Effect.wav");
	KeypickUp.setBuffer(KeypickUpBuffer);

}

BoxML::~BoxML(void)
{
	ClearObjects();
}

void BoxML::PrepareWorld(void)
{
	LoadBackground2("Assets/Enviroment/Water_Filter.png", sf::Color(255, 255, 255, 150));
	LoadBackground("Assets/background/sea.png");
	LoadWinBackground("Assets/background/win.png");
	LoadPauseBackground("Assets/background/pause.png");


	// Default map walls
	bfWall* wall;

	wall = CreateWall(b2_staticBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT - WALL_VERTICAL_WIDTH, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ (float)_screenWidth - PLAYGROUND_MARGIN_RIGHT, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ 0, PLAYGROUND_MARGIN_TOP - WALL_VERTICAL_WIDTH }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);
	wall = CreateWall(b2_staticBody, pixelToMeter({ 0, (float)_screenHeight - PLAYGROUND_MARGIN_BOTTOM }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);

}

void BoxML::CreateWorld(void)
{
	//ClearObjects();
	//PrepareWorld();

	//_player = CreatePlayer(b2_dynamicBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT + 50, PLAYGROUND_MARGIN_TOP + 50 }), { PLAYER_WIDTH, PLAYER_HEIGHT }, 0.01f, 0.3f);
	//_player->Body()->SetLinearVelocity({ PLAYER_SPEED_X, PLAYER_SPEED_Y });

	//bfWall* wall = CreateWall(b2_staticBody, pixelToMeter({ 800, 500 }), { WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT }, 0.01f, 0.3f, (uint16)ObjectCategory::SpeedWall_Vertical);

	// 4. RESTORED: MONSTER
	//bfMonster* monster;

	//monster = CreateMonster(b2_dynamicBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT + 600, PLAYGROUND_MARGIN_TOP + 50 }), { 108, 76 }, 0.01f, 0.3f, 1);
	//monster->setMovePattern(Monster_MovePattern::Down);

	//monster = CreateMonster(b2_dynamicBody, pixelToMeter({ (float)_screenWidth - 600, PLAYGROUND_MARGIN_TOP + 50 }), { 83, 87 }, 0.01f, 0.3f, 2);
	//monster->setMovePattern(Monster_MovePattern::Down);

	//monster = CreateMonster(b2_dynamicBody, pixelToMeter({ (float)_screenWidth - 600, PLAYGROUND_MARGIN_TOP + 100}), { 94, 56 }, 0.01f, 0.3f, 3);
	//monster->setMovePattern(Monster_MovePattern::Right);

	//CreateKey(b2_staticBody, pixelToMeter({ 500, 500 }), { KEY_WIDTH, KEY_HEIGHT }, 0.01f, 0.3f);
	//CreateDoor(b2_staticBody, pixelToMeter({ 500, 800 }), { GATE_WIDTH, GATE_HEIGHT }, 0.01f, 0.3f);
}

void BoxML::StartLevelMusic()
{
	if (_levelMusic.getStatus() != sf::SoundSource::Playing)
	{
		_levelMusic.setLoop(true);
		_levelMusic.play();
	}

}
void BoxML::StartLoserMusic()
{
	if (_LoserMusic.getStatus() != sf::SoundSource::Playing)
	{
		_LoserMusic.setLoop(true);
		_LoserMusic.play();
	}
}
void BoxML::StartWinMusic()
{

	if (_winSound.getStatus() != sf::SoundSource::Playing)
	{


		_winSound.setLoop(true);
		_winSound.play();
	}
}
void BoxML::StopLevelMusic() {
	if (_levelMusic.getStatus() == sf::SoundSource::Playing) {
		_levelMusic.stop();
		_levelMusic.setLoop(false);
	}

}
void::BoxML::StopWinMusic() {
	if (_winSound.getStatus() == sf::SoundSource::Playing) {
		_winSound.stop();
		_winSound.setLoop(false);
	}
}
void::BoxML::StopLoserMusic() {
	if (_LoserMusic.getStatus() == sf::SoundSource::Playing) {
		_LoserMusic.stop();
		_LoserMusic.setLoop(false);
	}
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

		if (obj->Body())
			ApplyRotation(obj, obj->Body()->GetAngle());
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
			if (cur->Body())
				_objsToDelete.push_back(cur->Body());

			delete cur;
			_objs.erase(it);
			return true;
		}
	}
	return false;
}

void BoxML::ClearObjects(void)
{
	bfObject* cur;
	for (auto it = _objs.begin(); it != _objs.end(); ++it)
	{
		cur = *it;
		if (!cur)
			continue;

		if (cur->Body())
			_objsToDelete.push_back(cur->Body());

		delete cur;
		*it = NULL;
	}

	_objs.clear();

	_player = nullptr;
	_placedWall = nullptr;
	_placedSpeedWall = nullptr;
	_placedMonster = nullptr;
}

bool BoxML::addContactObjects(b2Body* bodyA, b2Body* bodyB)
{
	if (inContactObjects(bodyA, bodyB))
		return false;

	_objsInContact.push_back({ bodyA, bodyB });
	return true;
}

bool BoxML::removeContactObjects(b2Body* bodyA, b2Body* bodyB)
{
	for (auto it = _objsInContact.begin(); it != _objsInContact.end(); ++it)
	{
		if (
			it->first == bodyA && it->second == bodyB
			|| it->first == bodyB && it->second == bodyA
			)
		{
			_objsInContact.erase(it);
			return true;
		}
	}

	return false;
}

void BoxML::DispatchDestroyBody(void)
{
	for (auto it = _objsToDelete.begin(); it != _objsToDelete.end(); ++it)
	{
		_world.DestroyBody(*it);
	}

	_objsToDelete.clear();
}

void BoxML::SetRenderState(const WorldRenderState state)
{
	_state = state;
}

void BoxML::SetPlayer(bfPlayer* player)
{
	_player = player;
}

bfCircle* BoxML::CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density, float friction, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	float rMeters = radius / _screenPixelPerUnit;


	bodyDef.position = position + b2Vec2(rMeters, rMeters);

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
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	bodyDef.position = position + b2Vec2(hx, hy);

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

	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	bodyDef.position = position + b2Vec2(hx, hy);

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
	{
		bfObj->loadSpriteSheet("Assets/characters/frog.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.25f);
		//bfObj->loadSpriteSheet("Assets/characters/frog4.png", 165, 192, 0, 0, 4, 4, _timer.getElapsedTime().asSeconds(), 0.17f);
		//bfObj->loadSpriteSheet("Assets/characters/frog4_fixed.png", 150, 192, 0, 0, 4, 4, _timer.getElapsedTime().asSeconds(), 0.17f);
	}

	bfObj->setScale({ 0.6f, 0.6f });

	return bfObj;
}

bfMonster* BoxML::CreateMonster(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, unsigned char spirteIndex)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	bodyDef.position = position + b2Vec2(hx, hy);

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
		bfObj->setMovePattern(Monster_MovePattern::Up);
		break;

	case 2:
		bfObj->loadSpriteSheet("Assets/monsters/2.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.15);
		bfObj->setMovePattern(Monster_MovePattern::Left);

		break;

	case 3:
		bfObj->loadSpriteSheet("Assets/monsters/3.png", size.x, size.y, 0, 0, 3, 3, _timer.getElapsedTime().asSeconds(), 0.17);
		bfObj->setMovePattern(Monster_MovePattern::Right);
		break;
	}

	return bfObj;
}

bfWall* BoxML::CreateWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, uint16 categoryBits, uint16 maskBits, bool addToWorld, bool loadSprite, bool invisible, unsigned char spriteIndex)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	bodyDef.position = position + b2Vec2(hx, hy);
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
			(ObjectCategory)((uint16)ObjectCategory::Wall_Vertical
				| (uint16)ObjectCategory::SpeedWall_Vertical
				| (uint16)ObjectCategory::Wall
				| (uint16)ObjectCategory::SpeedWall
				)))
		{
			if (!spriteIndex)
			{
				std::uniform_int_distribution<int> typeNum(1, 2);
				spriteIndex = typeNum(randGenerator);
			}

			switch (spriteIndex)
			{
			case 1:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_1.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 2:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_2.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 3:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_1.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;

			case 4:
				bfObj->loadSpriteSheet("Assets/walls/wall_v_2.png", size.x, size.y, 0, 0, 1, 1, 0, 0, true);
				break;
			}
		}

		else if (isObject((ObjectCategory)categoryBits,
			(ObjectCategory)((uint16)ObjectCategory::Wall_Horizontal
				| (uint16)ObjectCategory::SpeedWall_Horizontal)
		))
		{
			if (!spriteIndex)
			{
				std::uniform_int_distribution<int> typeNum(1, 3);
				spriteIndex = typeNum(randGenerator);
			}

			switch (spriteIndex)
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
	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;


	bodyDef.position = position + b2Vec2(hx, hy);


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

	bfObj->setScale({ 0.7f, 0.7f });

	return bfObj;
}

bfDoor* BoxML::CreateDoor(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density, float friction, bool loadSprite)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	b2Vec2 bSize = pixelToMeter(size);
	float hx = bSize.x / 2.0f;
	float hy = bSize.y / 2.0f;

	bodyDef.position = position + b2Vec2(hx, hy);

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

	bfObj->setScale({ 0.5f, 0.5f });

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

bool BoxML::LoadWinBackground(const std::string& imagePath, sf::Color color)
{
	/*if (!_winTexture.loadFromFile(imagePath, {}))
		return false;

	_winBackground.setTexture(_winTexture);
	_winBackground.setColor(color);*/
	return true;
}

bool BoxML::LoadPauseBackground(const std::string& imagePath, sf::Color color)
{
	if (!_pauseTexture.loadFromFile(imagePath, {}))
		return false;

	_pauseBackground.setTexture(_pauseTexture);
	_pauseBackground.setColor(color);
	return true;
}

void BoxML::Step(void)
{
	if (RenderState() != WorldRenderState::Running)
		return;

	if (_frameTimer.getElapsedTime().asMilliseconds() < _frameRefreshRate)
		return;

	DispatchDestroyBody();

	for (auto it = _objs.begin(); it != _objs.end(); ++it)
		(*it)->updatePosition(_timer.getElapsedTime().asSeconds());

	_world.Step(_timeStep, _velocityIterations, _positionIterations);

	_frameTimer.restart();	
}

void BoxML::Render(sf::RenderWindow& mainWnd)
{
	bfObject* obj;
	const WorldRenderState state = RenderState();

	//mainWnd.clear();

	if (state == WorldRenderState::Win)
	{
		mainWnd.draw((_winBackground));
		if (_levelMusic.getStatus() == sf::SoundSource::Playing) {
			_levelMusic.stop();
		}
	}
	
	else
	{
		mainWnd.draw(_background);
		mainWnd.draw(_background2);

		for (auto it = _objs.begin(); it != _objs.end(); ++it)
		{
			obj = *it;
			if (!obj)
				continue;

			obj->setSfPosition(meterToPixel(obj->getB2Position()));

			if (_player && _player->Body() == obj->Body())
			{
				ApplyRotation(obj);
			}


			mainWnd.draw(*obj->Drawable());
		}

		if (_previewObject)
		{
			_previewObject->setSfPosition(meterToPixel(_previewObject->getB2Position()));

			if (_previewObject->Body())
				ApplyRotation(_previewObject, _previewObject->Body()->GetAngle());

			else
				ApplyRotation(_previewObject, _previewRotation);

			mainWnd.draw(*_previewObject->Drawable());
		}

		if (state == WorldRenderState::Paused)
			mainWnd.draw((_pauseBackground));
	}

	//mainWnd.display();
}


void BoxML::OnBeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (!fixtureA || !fixtureB|| fixtureA->IsSensor() || fixtureB->IsSensor())
		return;

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (!bodyA || !bodyB || !addContactObjects(bodyA, bodyB))
		return;

	if (isObject(ObjectCategory::Player, fixtureA))
		OnPlayerContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Player, fixtureB))
		OnPlayerContact(fixtureB, fixtureA);

	if (isObject(ObjectCategory::Monster, fixtureA))
		OnMonsterContact(fixtureA, fixtureB);
	else if (isObject(ObjectCategory::Monster, fixtureB))
		OnMonsterContact(fixtureB, fixtureA);
}

void BoxML::OnEndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (!fixtureA || !fixtureB || fixtureA->IsSensor() || fixtureB->IsSensor())
		return;

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (!bodyA || !bodyB)
		return;

	removeContactObjects(bodyA, bodyB);
}

void BoxML::OnPostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void BoxML::OnPlayerContact(b2Fixture* player, b2Fixture* object)
{
	if (!player || !object)
		return;

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
		_wallSound.play();
		OnPlayerWallContact(player, object, objCategory);
	}

	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Monster))
	{
		StopLevelMusic();
		_loseSound.play();
		SetRenderState(WorldRenderState::Lose);
	}

	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Key))
		OnKeyContact(object, player);

	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Gate))
		OnDoorContact(object, player);
}

void BoxML::OnPlayerWallContact(b2Fixture* player, b2Fixture* wall, uint16 objCategory)
{
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * PI);

	b2Body* playerBody = player->GetBody();
	b2Vec2 currentVelocity = playerBody->GetLinearVelocity();
	float currentSpeed = currentVelocity.Length();
	if (currentSpeed < 0.1f)
		currentSpeed = PLAYER_SPEED;

	float randomAngle = angleDist(randGenerator);
	b2Vec2 randomDir(cos(randomAngle), sin(randomAngle));

	if (isObject((ObjectCategory)objCategory, ObjectCategory::Wall_Vertical))
	{
		if (currentVelocity.x > 0 && randomDir.x > 0 || currentVelocity.x < 0 && randomDir.x < 0)
			randomDir.x = -randomDir.x;

		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::Wall_Horizontal))
	{
		if (currentVelocity.y > 0 && randomDir.y > 0 || currentVelocity.y < 0 && randomDir.y < 0)
			randomDir.y = -randomDir.y;

		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::SpeedWall_Vertical))
	{
		if (currentVelocity.x > 0 && randomDir.x > 0 || currentVelocity.x < 0 && randomDir.x < 0)
			randomDir.x = -randomDir.x;

		_player->setBoostMultiplier(WALL_SPEED_INC_MULTIPLIER);
		_player->setSpeedBoostTimer(_timer.getElapsedTime().asSeconds() + WALL_SPEED_INC_TIME);
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}
	else if (isObject((ObjectCategory)objCategory, ObjectCategory::SpeedWall_Horizontal))
	{
		if (currentVelocity.y > 0 && randomDir.y > 0 || currentVelocity.y < 0 && randomDir.y < 0)
			randomDir.y = -randomDir.y;

		_player->setBoostMultiplier(WALL_SPEED_INC_MULTIPLIER);
		_player->setSpeedBoostTimer(_timer.getElapsedTime().asSeconds() + WALL_SPEED_INC_TIME);
		playerBody->SetLinearVelocity(currentSpeed * randomDir);
	}

	else if (isObject((ObjectCategory)objCategory, ObjectCategory::SpeedWall))
	{
		_player->setBoostMultiplier(WALL_SPEED_INC_MULTIPLIER);
		_player->setSpeedBoostTimer(_timer.getElapsedTime().asSeconds() + WALL_SPEED_INC_TIME);
		playerBody->SetLinearVelocity(currentVelocity);
	}
}

void BoxML::OnMonsterContact(b2Fixture* monster, b2Fixture* object)
{
	if (!monster || !object)
		return;

	b2Body* monsterBody = monster->GetBody();
	bfMonster* monsterObj = findObjectByBody<bfMonster>(monsterBody);

	if (!monsterBody || !monsterObj)
		return;

	uint16 objCategory = object->GetFilterData().categoryBits;
	printf(__FUNCTION__": %d\n", objCategory);

	if (isObject((ObjectCategory)objCategory,
		(ObjectCategory)((uint16)ObjectCategory::Player
			)))
		return;

	monsterObj->setMovePattern(monsterObj->toggleMovementPattern());
}

void BoxML::OnKeyContact(b2Fixture* key, b2Fixture* object)
{
	KeypickUp.play();
	bfKey* keyObj = findObjectByBody<bfKey>(key->GetBody());
	if (!keyObj)
		return;

	RemoveObject(keyObj);

	bfDoor* doorObj = findObjectByBody<bfDoor>(ObjectCategory::Gate);
	if (!doorObj)
		return;

	doorObj->loadSpriteSheet("Assets/Enviroment/Gate_Open.png", 202, 298, 0, 0, 1, 1, 0, 0, true);
}

void BoxML::OnDoorContact(b2Fixture* door, b2Fixture* object)
{
	bfKey* keyObj = findObjectByBody<bfKey>(ObjectCategory::Key);
	if (keyObj)
		return;
	StopLevelMusic();
	StartWinMusic();

	SetRenderState(WorldRenderState::Win);
}

WorldRenderState BoxML::RenderState(void) const
{
	return _state;
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
	if (!fixture)
		return false;
	return isObject(category, (ObjectCategory)fixture->GetFilterData().categoryBits);
}

bool BoxML::inContactObjects(b2Body* bodyA, b2Body* bodyB)
{
	for (auto it = _objsInContact.begin(); it != _objsInContact.end(); ++it)
	{
		if (
			it->first == bodyA && it->second == bodyB
			|| it->first == bodyB && it->second == bodyA
			)
			return true;
	}

	return false;
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
			if (_previewObject->Body())
				_objsToDelete.push_back(_previewObject->Body());

			delete _previewObject;
			_previewObject = nullptr;
		}
	}
}
void BoxML::UpdatePreviewObject(const sf::Vector2f& pixelMousePos)
{

	bool limitReached = false;

	if (_currentPreviewType == ObjectCategory::Wall && _placedWall != nullptr) limitReached = true;
	if (_currentPreviewType == ObjectCategory::SpeedWall && _placedSpeedWall != nullptr) limitReached = true;
	if (_currentPreviewType == ObjectCategory::Monster && _placedMonster != nullptr) limitReached = true;

	if (limitReached) {
		if (_previewObject) {
			if (_previewObject->Body()) _objsToDelete.push_back(_previewObject->Body());
			delete _previewObject;
			_previewObject = nullptr;
		}
		return;
	}

	b2Vec2 mouseMeters = pixelToMeter(pixelMousePos);

	if (_previewObject == nullptr)
	{
		const sf::Vector2f objectSize(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT);

		switch (_currentPreviewType)
		{
		case ObjectCategory::Wall:

			_previewObject = CreateWall(b2_staticBody, mouseMeters, objectSize, 0.0f, 0.0f,
				(uint16)ObjectCategory::Wall, 0, false, true, false, 3);
			break;

		case ObjectCategory::SpeedWall:
		
			_previewObject = CreateWall(b2_staticBody, mouseMeters, objectSize, 0.0f, 0.0f,
				(uint16)ObjectCategory::SpeedWall, 0, false, true, false, 4);
			break;

		case ObjectCategory::Monster:
			_previewObject = CreateMonster(b2_dynamicBody, mouseMeters, { 80.0f, 80.0f }, 0.0f, 0.0f, 1);
			RemoveObject(_previewObject);
			_objsToDelete.pop_back();
			break;
		}

		if (_previewObject && _previewObject->Body())
		{
			_previewObject->Body()->SetGravityScale(0.0f);
			b2Fixture* fixture = _previewObject->Body()->GetFixtureList();
			if (fixture) fixture->SetSensor(true);
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

bool IsFixtureColliding(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();

	for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;

		if (!c->IsTouching())
			continue;

		if (c->GetFixtureA() == fixture || c->GetFixtureB() == fixture)
			return true;
	}
	return false;
}

void BoxML::PlacePreviewObject()
{
	if (!_previewObject) return;

	bfObject* placedObj = _previewObject;
	b2Body* body = placedObj->Body();

	if (body)
	{
		b2Fixture* fixture = body->GetFixtureList();
		if (fixture)
		{
			if (IsFixtureColliding(fixture))
				return;

			fixture->SetSensor(false);
		}

		if (_currentPreviewType == ObjectCategory::Monster) {
			body->SetType(b2_dynamicBody);
			body->SetGravityScale(1.0f);
		}
		else {
			body->SetType(b2_staticBody);
		}

		AddObject(placedObj);
	}

	if (_currentPreviewType == ObjectCategory::Wall) _placedWall = placedObj;
	else if (_currentPreviewType == ObjectCategory::SpeedWall) _placedSpeedWall = placedObj;
	else if (_currentPreviewType == ObjectCategory::Monster) _placedMonster = (bfMonster*)placedObj;

	_previewObject = nullptr;
	_previewRotation = 0.0f;
}
void BoxML::HandleRightClick(const sf::Vector2f& pixelMousePos)
{
	b2Vec2 mouseMeters = pixelToMeter(pixelMousePos);

	bfObject* objectsToCheck[] = { _placedWall, _placedSpeedWall, _placedMonster };
	for (bfObject* obj : objectsToCheck) {
		if (obj && obj->Body()) {
			for (b2Fixture* f = obj->Body()->GetFixtureList(); f; f = f->GetNext()) {
				if (f->TestPoint(mouseMeters))
				{
					bool remove = false;

				
					if (obj == _placedWall)
					{
						_placedWall = nullptr;
						remove = true;
					}

					if (obj == _placedSpeedWall)
					{
						_placedSpeedWall = nullptr;
						remove = true;
					}

					if (obj == _placedMonster)
					{
						_placedMonster = nullptr;
						remove = true;
					}

					if (remove)
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
