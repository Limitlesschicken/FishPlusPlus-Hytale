/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <memory>

#include "Shader.h"


namespace Shaders {
	inline std::unique_ptr<Shader> posColor = nullptr;
	inline std::unique_ptr<Shader> text = nullptr;
	inline std::unique_ptr<Shader> alphaChecker = nullptr;
	inline std::unique_ptr<Shader> hue = nullptr;

	void initShaders();
}