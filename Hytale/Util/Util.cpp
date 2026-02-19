/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "core.h"
#include "Util.h"

#include "../sdk/Entity.h"

void Util::ResetGlobals() {
	dmc = nullptr;
}

bool Util::IsValidPtr(void* ptr) {
	if (!ptr)
		return false;

	if (ptr < (void*) 0x10000000)
		return false;

	if (ptr > (void*) 0x7FFFFFFFFFFF)
		return false;

	return true;
}

GameInstance* Util::getGameInstance() {
	ValidPtr(app);
	ValidPtr(app->appInGame);
	return Util::app->appInGame->gameInstance;
}

Entity* Util::getLocalPlayer() {
	GameInstance* gameInstance = getGameInstance();
	ValidPtr(gameInstance);

	return gameInstance->Player;
}

Camera* Util::getCamera() {
	GameInstance* gameInstance = getGameInstance();
	ValidPtr(gameInstance);

	StructBeforeCam* structBeforeCam = gameInstance->Camera;
	ValidPtr(structBeforeCam);

	return structBeforeCam->Camera;
}

CameraModule* Util::getCameraModule() {
	GameInstance* gameInstance = getGameInstance();
	ValidPtr(gameInstance);

	return gameInstance->CameraModule;
}

double Util::GetTime() {
	static LARGE_INTEGER freq;
	static bool initialized = false;
	if (!initialized) {
		QueryPerformanceFrequency(&freq);
		initialized = true;
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (double)now.QuadPart / (double)freq.QuadPart;
}