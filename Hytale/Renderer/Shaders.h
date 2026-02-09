#pragma once

#include <memory>

#include "Shader.h"


namespace Shaders {
	inline std::unique_ptr<Shader> posColor = nullptr;
	inline std::unique_ptr<Shader> text = nullptr;

	void initShaders();
}