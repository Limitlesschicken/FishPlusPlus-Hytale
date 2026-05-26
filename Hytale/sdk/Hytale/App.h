/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "AppInGame.h"
#include "Input.h"
#include "Engine.h"
#include "AppMainMenu.h"

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
	char pad_0050[72];   //0x0050
	void* AppStartup ; //0x0070
	AppMainMenu* AppMainMenu ; //0x0098
	void* AppInGameLoading ; //0x00A0
	AppInGame* appInGame; //0x00B0
};