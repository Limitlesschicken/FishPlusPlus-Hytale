#include "Util.h"

#include "../sdk/Entity.h"



void Util::ResetGlobals() {
	dmc = nullptr;
}

GameInstance* Util::getGameInstance() {
	if (app == nullptr)
		return nullptr;
	if (app->appInGame == nullptr)
		return nullptr;
	return Util::app->appInGame->gameInstance;
}

Entity* Util::getLocalPlayer() {
	GameInstance* gameInstance = getGameInstance();
	if (gameInstance == nullptr)
		return nullptr;
	return gameInstance->Player;
}

Camera* Util::getCamera() {
	if (app == nullptr)
		return nullptr;
	if (app->appInGame == nullptr)
		return nullptr;
	if (app->appInGame->gameInstance == nullptr)
		return nullptr;
	if (app->appInGame->gameInstance->Camera == nullptr)
		return nullptr;
	return Util::app->appInGame->gameInstance->Camera->Camera;
}

double Util::GetTime()
{
	static LARGE_INTEGER freq;
	static bool initialized = false;
	if (!initialized)
	{
		QueryPerformanceFrequency(&freq);
		initialized = true;
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (double)now.QuadPart / (double)freq.QuadPart;
}

std::vector<Entity*> Util::getEntities() {
	GameInstance* gameInstance = getGameInstance();
	if (gameInstance == nullptr)
		return {};

	EntityStoreModule* entityStoreModule = gameInstance->entityStoreModule;
	int entityCount = entityStoreModule->entityCount;
	uintptr_t entityArray = entityStoreModule->entityArray;

	std::vector<Entity*> entities;
	for (int i = 0; i < entityCount; i++) {
		uintptr_t entity = *(uintptr_t*)(entityArray + 0x10 + i * 8);
		if (entity) {
			entities.push_back((Entity*)entity);
		}
	}
	
	
	return entities;
}