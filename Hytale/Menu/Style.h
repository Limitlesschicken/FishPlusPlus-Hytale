/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Math/Color.h"
#include "../Math/Vector2.h"

namespace Style {
	constexpr Color headerColor(255.0f, 100.0f, 100.0f, 255.0f);
	constexpr float headerHeight = 30.0f;
	constexpr Vector2 headerPadding(14.0f, 8.0f);

	constexpr Color tabBgColor(50.0f, 50.0f, 50.0f, 230.0f);
	constexpr Color tabOutlineColor(255.0f, 100.0f, 100.0f, 255.0f);

	constexpr Vector2 moduleNamePadding(8.0f, 8.0f);
	constexpr float featureHeight = 30.0f;
	constexpr Color moduleHoverColor(100.0f, 100.0f, 100.0f, 180.0f);
	constexpr Color moduleTextActiveColor(255.0f, 100.0f, 100.0f, 255.0f);

	constexpr Vector2 settingsNamePadding(12.0f, 8.0f);

	constexpr Color recursiveColor(255.0f, 100.0f, 100.0f, 200.0f);

	constexpr Color sliderColor(150.0f, 150.0f, 150.0f, 180.0f);
}