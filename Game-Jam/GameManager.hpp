#pragma once
#include "bfPlayer.h"
#include "WorldContactListener.h"
class GameManager
{
	bfPlayer* player;
	WorldContactListener* contactListener;
	
	public:
		GameManager(bfPlayer* p, WorldContactListener* cl) : player(p),contactListener(cl) {}
		void update()
		{
				
		}
};