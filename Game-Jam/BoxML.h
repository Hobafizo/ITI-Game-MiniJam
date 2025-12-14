#pragma once
#include "bfObject.h"
#include "GameData.h"
#include "WorldContactListener.h"
#include "ObjectCategory.h"
#include <list>
#include <SFML/Graphics.hpp> // Required for sf::RenderWindow and sf::Event
#include <SFML/Audio.hpp>

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
	std::list<std::pair<b2Body*, b2Body*>> _objsInContact;
	std::list<b2Body*> _objsToDelete;
	std::list<bfObject*> _objs;

	WorldRenderState _state;
	sf::Texture _backgroundTexture, _backgroundTexture2, _winTexture, _pauseTexture;
	sf::Sprite _background, _background2, _winBackground, _pauseBackground;

	class bfPlayer* _player;

	sf::SoundBuffer _loseBuffer;

	sf::Sound _loseSound;
	sf::Music _winSound;
	sf::Music _levelMusic;
	sf::SoundBuffer KeypickUpBuffer;
	sf::Sound KeypickUp;
public:
	sf::Music _LoserMusic;
private:
	sf::SoundBuffer _wallBuffer;
	sf::Sound _wallSound;
	bool _hasLost;

	bfObject* _previewObject = nullptr;
	ObjectCategory _currentPreviewType = ObjectCategory::None;
	float _previewRotation;

	bfObject* _placedWall = nullptr;
	bfObject* _placedSpeedWall = nullptr;
	bfObject* _placedMonster = nullptr;

	static BoxML* _instance;

public:
	BoxML(unsigned short screenWidth, unsigned short screenHeight, unsigned short screenPixelPerUnit, float timeStep, int32 velocityIterations, int32 positionIterations);
	~BoxML();

	void PrepareWorld();
	void CreateWorld();
	void LoadPositions();

	void HandleInput(sf::RenderWindow& window, sf::Event& event);
	void HandleKeyPress(sf::Keyboard::Key key);
	void HandleRightClick(const sf::Vector2f& pixelMousePos);
	void UpdatePreviewObject(const sf::Vector2f& pixelMousePos);
	void PlacePreviewObject();

private:
	void AddObject(bfObject* obj);
	bool RemoveObject(bfObject* obj);
public:
	void ClearObjects();
private:
	bool addContactObjects(b2Body* bodyA, b2Body* bodyB);
	bool removeContactObjects(b2Body* bodyA, b2Body* bodyB);
	void DispatchDestroyBody();

public:
	void SetRenderState(const WorldRenderState state);
	void SetPlayer(bfPlayer* player);

	class bfCircle* CreateCircle(const b2BodyType bodyType, const b2Vec2 position, float radius, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0);
	class bfRectangle* CreateRectangle(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0, bool addToWorld = true);
	class bfPlayer* CreatePlayer(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, bool loadSprite = true);
	class bfMonster* CreateMonster(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, unsigned char spirteIndex = 0);
	class bfWall* CreateWall(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, uint16 categoryBits = 0, uint16 maskBits = 0, bool addToWorld = true, bool loadSprite = true, bool invisible = false, unsigned char spriteIndex = 0);
	class bfKey* CreateKey(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, bool loadSprite = true);
	class bfDoor* CreateDoor(const b2BodyType bodyType, const b2Vec2 position, const sf::Vector2f size, float density = 0.01f, float friction = 0.3f, bool loadSprite = true);

	bool LoadBackground(const std::string& imagePath, sf::Color color = sf::Color(255, 255, 255, 255));
	bool LoadBackground2(const std::string& imagePath, sf::Color color = sf::Color(255, 255, 255, 255));
	bool LoadWinBackground(const std::string& imagePath, sf::Color color = sf::Color(255, 255, 255, 255));
	bool LoadPauseBackground(const std::string& imagePath, sf::Color color = sf::Color(255, 255, 255, 255));
	void StartLevelMusic();
	void StopLevelMusic();
	void StartWinMusic();
	void StartLoserMusic();
	void StopWinMusic();
	void StopLoserMusic();
	void Step();
	void Render(sf::RenderWindow& mainWnd);

	void OnBeginContact(b2Contact* contact);
	void OnEndContact(b2Contact* contact);
	void OnPostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void OnPlayerContact(b2Fixture* player, b2Fixture* object);
	void OnPlayerWallContact(b2Fixture* player, b2Fixture* wall, uint16 objCategory);
	void OnMonsterContact(b2Fixture* monster, b2Fixture* object);
	void OnKeyContact(b2Fixture* key, b2Fixture* object);
	void OnDoorContact(b2Fixture* door, b2Fixture* object);

	// Utilities
	WorldRenderState RenderState() const;
	sf::Vector2u Resolution() const;
	b2Vec2 pixelToMeter(const sf::Vector2f pixel) const;
	sf::Vector2f meterToPixel(const b2Vec2 meter) const;
	b2Vec2 centerAround(const b2Vec2 size, const sf::Vector2f targetPosition, const sf::Vector2f targetSize) const;
	bool isObject(const ObjectCategory category, const ObjectCategory object) const;
	bool isObject(const ObjectCategory category, b2Fixture* fixture) const;
	bool inContactObjects(b2Body* bodyA, b2Body* bodyB);

	template<typename T>
	T* findObjectByBody(b2Body* body);

	template<typename T>
	T* findObjectByBody(const ObjectCategory category);

public:
	static void setInstance(BoxML* world);
	static BoxML* Instance();
};