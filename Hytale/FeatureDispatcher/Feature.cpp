/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "Feature.h"

#include "../Renderer/Renderer3D.h"
#include "Settings/KeybindSetting.h"

Feature::Feature(std::string name) {
	m_name = name;
	m_category = "";
	enabled = false;
	this->RegisterSetting<KeybindSetting>("Keybind", SDL_SCANCODE_UNKNOWN, this);
}

void Feature::OnTick() {}
void Feature::PlayerMove(MoveCycleEvent& event) {}
void Feature::OnRender3D(Render3DEvent& event) {};
void Feature::OnRender2D(Render2DEvent& event) {};

bool Feature::CanExecute() { return true; }
void Feature::onEnable() {}
void Feature::onDisable() {}


void Feature::setCategory(std::string category) {
	m_category = category;
}
