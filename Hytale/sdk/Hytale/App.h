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
	void* AuthManager; //0x0018
	void* FontManager; //0x0020
	char pad_0028[0x20]; //0x0028
	Input* Input; //0x0048
	char pad_0050[0x70];   //0x0050
	void* AppStartup ; //0x00C0
	AppMainMenu* AppMainMenu ; //0x00C8
	void* AppInGameLoading ; //0x00D0
	AppInGame* appInGame; //0x00D8
};