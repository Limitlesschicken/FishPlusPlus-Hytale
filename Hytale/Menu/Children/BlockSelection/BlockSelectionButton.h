/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <Menu/Component.h>

#include <memory>

#include "../SettingButton.h"
#include "Features/Setting.h"
#include "Features/Settings/BlockSelection/BlockSelection.h"
#include "BlockSelectionWindow.h"

class BlockSelectionButton : public SettingButton {
public:
	BlockSelectionButton(Setting<std::vector<BlockSelection>>* setting);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

	std::unique_ptr<BlockSelectionScreen> screen;
};

