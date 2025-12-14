#pragma	once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class StartupVideo
{
private:
	sf::Vector2u windowSize;
	std::vector<sf::Texture> backgroundFrames;
	sf::Sprite backgroundSprite;
	sf::Clock animationClock;
	float frameDuration;
	int currentFrameIndex;
	bool _framesLoaded = false;
	bool _framesEnded = false;

	sf::Music _music;
	bool _musicLoaded = false;

public:
	StartupVideo(sf::Vector2u windowSize);
	~StartupVideo();
	
	bool Load();

private:
	bool LoadMusic();
	bool LoadTextures();
	void SetupSprites();

	void StartMusic();
	void StopMusic();
	void PauseMusic();

public:
	bool Start();
	void Draw(sf::RenderWindow& window);

	bool Ended() const;
};