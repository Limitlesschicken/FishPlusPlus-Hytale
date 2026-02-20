/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "AppInGame.h"
#include "Input.h"
#include "Engine.h"

enum AppStage {
	Initial,
	Startup,
	MainMenu,
	GameLoading,
	InGame,
	Disconnection,
	Exited
};

struct App {
	char pad_0000[12]; //0x0000
	int32_t Stage; //0x000C
	Engine* Engine; //0x0010
	char pad_0018[48]; //0x0018
	Input* Input; //0x0048
	char pad_0050[32]; //0x0050
	void* AppStartup ; //0x0070
	void* AppMainMenu ; //0x0078
	void* AppInGameLoading ; //0x0080
	AppInGame* appInGame; //0x0088
	char pad_0090[1456]; //0x0090
};