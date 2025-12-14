#include "StartupVideo.h"
#include "def.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#ifndef QUALITY_INTRO
#define FRAME_PATH           "Assets/intro_video/ezgif-frame-"
#define FRAME_FILE_EXTENSION ".jpg"
#define FRAME_COUNT          196
#define FRAME_DURATION       0.04f
#define MUSIC_PATH           "Assets/intro_video/sound.wav"

#else
#define FRAME_PATH           "Assets/intro_video2/ezgif-frame-"
#define FRAME_FILE_EXTENSION ".png"
#define FRAME_COUNT          196
#define FRAME_DURATION       0.04f
#define MUSIC_PATH           "Assets/intro_video2/sound.wav"

#endif

StartupVideo::StartupVideo(sf::Vector2u windowSize) : windowSize(windowSize)
{
}

StartupVideo::~StartupVideo(void)
{
}

bool StartupVideo::Load(void)
{
	_musicLoaded = LoadMusic();
	if (!_musicLoaded)
		return false;

	_framesLoaded = LoadTextures();
	if (!_framesLoaded)
		return false;

	return true;
}

bool StartupVideo::LoadMusic(void)
{
	if (!_music.openFromFile(MUSIC_PATH))
		return false;

	_music.setLoop(false);
	return true;
}

bool StartupVideo::LoadTextures(void)
{
	backgroundFrames.clear();
	backgroundFrames.reserve(FRAME_COUNT);

	for (int i = 1; i <= FRAME_COUNT; ++i)
	{
		sf::Texture t;
		std::stringstream ss;

		// Constructs path: Assets/frog_frames/frame_000.png ... frame_054.png
		ss << FRAME_PATH
			<< std::setw(3) << std::setfill('0') << i << FRAME_FILE_EXTENSION;

		if (!t.loadFromFile(ss.str()))
			return false;

		t.setSmooth(true);
		backgroundFrames.push_back(t);
	}

	if (backgroundFrames.empty())
		return false;

	return true;
}

void StartupVideo::SetupSprites(void)
{
	// --- Background Setup ---
	if (backgroundFrames.empty())
		return;

	unsigned int frameWidth = backgroundFrames[0].getSize().x;
	unsigned int frameHeight = backgroundFrames[0].getSize().y;

	// SAFETY CHECK 1 & 2: Prevent division by zero if the first frame is empty.
	if (frameWidth > 0 && frameHeight > 0)
	{
		backgroundSprite.setTexture(backgroundFrames[0]);

		// Calculate scaling to cover the screen
		float scaleX = (float)windowSize.x / frameWidth;
		float scaleY = (float)windowSize.y / frameHeight;
		float finalScale = std::max(scaleX, scaleY);

		backgroundSprite.setScale(finalScale, finalScale);

		// Center the background
		backgroundSprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
		backgroundSprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
	}
	else {
		std::cout << "WARNING: Background frame size is zero! Cannot display animation.\n";
	}

	// --- Animation Settings ---
	currentFrameIndex = 0;
	frameDuration = FRAME_DURATION;
	animationClock.restart();
}

void StartupVideo::StartMusic(void)
{
	if (!_musicLoaded)
		return;

	_music.play();
}

void StartupVideo::StopMusic(void)
{
	if (!_musicLoaded)
		return;

	_music.stop();
}

void StartupVideo::PauseMusic(void)
{
	if (!_musicLoaded)
		return;

	_music.pause();
}

bool StartupVideo::Start(void)
{
	if (!_framesLoaded || !_musicLoaded)
		return false;

	_framesEnded = false;

	SetupSprites();
	StartMusic();
}

void StartupVideo::Draw(sf::RenderWindow& window)
{
	if (!backgroundFrames.empty() && animationClock.getElapsedTime().asSeconds() > frameDuration)
	{
		if (currentFrameIndex + 1 == backgroundFrames.size())
			_framesEnded = true;

		currentFrameIndex = (currentFrameIndex + 1) % backgroundFrames.size();

		backgroundSprite.setTexture(backgroundFrames[currentFrameIndex], true);
		animationClock.restart();
	}

	window.draw(backgroundSprite);
}

bool StartupVideo::Ended(void) const
{
	return _framesEnded;
}
