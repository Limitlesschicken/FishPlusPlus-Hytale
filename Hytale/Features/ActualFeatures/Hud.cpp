/*
 * Copyright (c) FishPlusPlus.
 */
#include "Hud.h"

void Hud::OnActivate() {
	this->setActive(false);
	Menu::SetMainComponent(Menu::hudTabComponent.get());
}