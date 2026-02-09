#pragma once
#include "AppInGame.h"
#include "Input.h"

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
	AppStage Stage; //0x000C
	char pad_0010[56]; //0x0010
	Input* Input; //0x0048
	char pad_0050[32]; //0x0050
	void* AppStartup ; //0x0070
	void* AppMainMenu ; //0x0078
	void* AppInGameLoading ; //0x0080
	AppInGame* appInGame; //0x0088
	char pad_0090[1456]; //0x0090
};