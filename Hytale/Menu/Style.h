#pragma once

#include "../Math/Color.h"
#include "../Math/Vector2.h"

namespace Style {
	constexpr Color headerColor(255, 100, 100, 255);
	constexpr float headerHeight = 30.0f;
	constexpr Vector2 headerPadding(14.0f, 8.0f);

	constexpr Color tabBgColor(50.0f, 50.0f, 50.0f, 230);
	constexpr Color tabOutlineColor(255, 100, 100, 255);

	constexpr Vector2 moduleNamePadding(8, 8);
	constexpr float featureHeight = 30.0f;
	constexpr Color moduleHoverColor(100, 100, 100, 180);
	constexpr Color moduleTextActiveColor(255, 100, 100, 255);

}