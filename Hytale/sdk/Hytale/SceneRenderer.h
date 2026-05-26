/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Math/Matrix4x4.h"
#include "RenderStructs.h"


struct SceneRenderer {
	void* mt;
	void* RenderTargetStore;
	void* RenderTarget;

	char pad_20[0x50];

	Array<EntityDrawTask> EntityDrawTask1;
	Array<EntityDrawTask> EntityDrawTask2;
	
	
	char pad_80[0x2A8];

	Matrix4x4 MPV;
};