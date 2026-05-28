/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Math/Matrix4x4.h"
#include "RenderStructs.h"

struct GraphicsDevice {
	char pad[0x468];
	uint32_t drawCallCount;         // 0x468 - incremented per draw call
	uint32_t totalIndicesDrawn;     // 0x46C - accumulated index count
};

struct GraphicsDeviceContainer {
	char pad_0000[0x10];
	GraphicsDevice* graphicsDevice;
};

struct GPUProgramStore {
	char pad[0x74];
	int uniformBufferId;       // 0x74 - GL uniform buffer ID
	char pad_078[0xC];       // 0x78
	uint32_t shaderProgramId;       // 0x84 - GL shader program ID
	char pad_0x88[0x4];       // 0x88
	int unkValue; // 0x8C
	int unkValue2;
	int unkValue3;
	int unkValue4;
	int unkValue5;
};

struct GPUProgramStoreContainer {
	char pad_0000[0x30];
	GPUProgramStore* GPUProgramStore;
};


//Important that this size stays 0x88 to keep the stride for array correct because its not a pointer
struct EntityDrawTask {
	EntityDrawData* drawData;            // 0x0
	char pad_0018[0x10];
	uint16_t entityLocalId;              // 0x18
	char pad_002A[0x6A];				// 0x2A
	uint32_t AnimationId;              // 0x94
};

struct ArrayHeaderSize {
	char pad[0x8];
	int count;
};

struct EntityDrawData {
	char pad_0000[0x0C];
	int animationOffset;                 // 0x0C
	char pad_0010[0x28];
	Array<int>* AnimationData;                 // 0x38
	Array<uint16_t>* AnimationSizeData;             // 0x40
	char pad_0048[0xC0];
	Array<int>* drawCallArray;                 // 0x108
	Array<int>* vertexArrayArray;              // 0x110
};



struct SceneRenderer {
	char pad_0000[0x30];                    // 0x000

	GraphicsDeviceContainer* graphicsDevice;           // 0x030 -> contains GraphicsDevice at +0x10

	GPUProgramStoreContainer* contextContainer;		// 0x038 -> contains GPUProgramStore at +0x30

	char pad_0040[48]; //0x0040

	Array<EntityDrawTask>* EntityDrawTask1;
	Array<EntityDrawTask>* EntityDrawTask2;
	
	char pad_0080[312]; //0x0080
	uint32_t entityDrawCount; //0x01B8
	char pad_01BC[356]; //0x01BC
	Matrix4x4 MPV;
};