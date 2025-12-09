#pragma once
#include "bfObject.h"
#include "WorldContactListener.h"
#include "ObjectCategory.h"
#include <list>
#include <SFML/Graphics.hpp> // Required for sf::RenderWindow and sf::Event
#include<SFML/Audio.hpp>

class BoxML
{
private:
	unsigned short _screenWidth, _screenHeight, _screenPixelPerUnit;
	float _timeStep;
	int32 _frameRefreshRate;
	int32 _velocityIterations;
	int32 _positionIterations;

	sf::Clock _timer, _frameTimer;
	b2Vec2 _gravity;
	b2World _world;
	WorldContactListener _contactListener;
	std::list<bfObject*> _objs;

	class bfPlayer* _player;

	sf::SoundBuffer _loseBuffer;
	sf::SoundBuffer _winBuffer;

	sf::Sound _loseSound;
	sf::Sound _winSound;
	sf::Music _levelMusic;
	bool _hasLost;




	bfObject* _previewObject = nullptr;
	ObjectCategory _currentPreviewType = ObjectCategory::Wall;
	float _previewRotation;

	bfObject* _placedWall = nullptr;
	bfObject* _placedSpeedWall = nullptr;
	bfObject* _placedMonster = nullptr;

public:
	BoxML(unsigned short screenWidth, unsigned short screenHeight, unsigned short screenPixelPerUnit, float timeStep, int32 velocityIterations, int32 positionIterations);
	~BoxML();

	void CreateWorld();
	void LoadPositions();

	void HandleInput(sf::RenderWindow& window, sf::Event& event);


	void HandleKeyPress(sf::Keyboard::Key key);
	void UpdatePreviewObject(const sf::Vector2f& pixelMousePos);
	void PlacePreviewObject();
	void HandleRightClick(const sf::Vector2f& pixelMousePos);

private:
	void AddObject(bfObject* obj);
	bool RemoveObject(bfObject* obj);
	void ClearObjects();

public:

	class bfCircle* CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0);
	class bfRectangle* CreateRectangle(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0, bool addToWorld = true);
	class bfPlayer* CreatePlayer(const b2BodyType bodyType, const b2Vec2 position, float radius, float density = 0.01f, float friction = 0.3f);
	class bfMonster* CreateMonster(const b2BodyType bodyType, const b2Vec2 position, float radius, float density = 0.01f, float friction = 0.3f);
	class bfWall* CreateWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0, bool addToWorld = true);

	void Step();
	void Render(sf::RenderWindow& mainWnd);


	void OnBeginContact(b2Contact* contact);
	void OnEndContact(b2Contact* contact);
	void OnPostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void OnPlayerContact(b2Fixture* player, b2Fixture* object);
	void OnPlayerWallContact(b2Fixture* player, b2Fixture* wall, uint16 objCategory);
	void OnMonsterContact(b2Fixture* monster, b2Fixture* object);


	b2Vec2 pixelToMeter(const sf::Vector2f pixel) const;
	sf::Vector2f meterToPixel(const b2Vec2 meter) const;
	b2Vec2 centerAround(const b2Vec2 size, const sf::Vector2f targetPosition, const sf::Vector2f targetSize) const;
	bool isObject(const ObjectCategory category, const ObjectCategory object) const;
	bool isObject(const ObjectCategory category, b2Fixture* fixture) const;

	template<typename T>
	T* findObjectByBody(b2Body* body);
};