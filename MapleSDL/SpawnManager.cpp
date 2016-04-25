#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "Entity.h"
#include "SpawnManager.h"
#include "Game.h"

void SpawnManager::ManagePool(Uint32 tick) {
	this->lastSpawn = (tick/1000) - lastSpawnIndex;
	if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn) {
		this->lastSpawnIndex = (tick/1000);
		this->spawned.insert(this->spawned.end(), this->MobList->at("mush"));
		this->spawned.at(this->spawned.size() - 1).SetPositionY(210);
	}
}