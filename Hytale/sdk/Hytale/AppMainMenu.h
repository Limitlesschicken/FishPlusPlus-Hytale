/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

enum MainMenuPage {
	Home,
	Servers,
	Minigames,
	Adventure,
	WorldOptions,
	MyAvatar,
	Settings,
	SharedSinglePlayer
};

struct AppMainMenu {

	char pad[245];
	MainMenuPage CurrentPage;
};