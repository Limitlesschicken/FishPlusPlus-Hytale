#pragma once

#include <memory>

#include "Font.h"

namespace Fonts {
	inline std::unique_ptr<Font> Figtree;

	void initFonts();
}