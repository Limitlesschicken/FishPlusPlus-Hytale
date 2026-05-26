/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <cstdint>

// Forward declarations
struct SceneContext;
struct RenderStats;
struct EntityDrawData;
struct EntityDrawTask;

// Render statistics tracking
struct RenderStats {
    char pad_0000[0x438];           // 0x000
    uint32_t drawCallCount;         // 0x438 - incremented per draw call
    uint32_t totalIndicesDrawn;     // 0x43C - accumulated index count
};

// Scene context containing shader and buffer information
struct SceneContext {
    char pad_0000[0x78];            // 0x000
    uint32_t uniformBufferId;       // 0x078 - GL uniform buffer ID
    char pad_007C[0x8];             // 0x07C
    uint32_t shaderProgramId;       // 0x084 - GL shader program ID
};

struct GLVertexArray {
    uint32_t InternalId;
};

struct GLBuffer {
    uint32_t InternalId;
};

struct EntityDrawTask {
    Vector4 BlockLightColor;
    Vector3 BottomTint;
    Vector3 TopTint;
    float InvModelHeight;
    Matrix4x4 ModelMatrix;
    GLVertexArray VertexArray;
    int DataCount;
    GLBuffer AnimationData;
    uint32_t AnimationDataOffset;
    uint16_t AnimationDataSize;
    float ModelVFXAnimationProgress;
    int ModelVFXId;
    float UseDithering;
    uint16_t EntityLocalId;
};

// Entity list structure
struct EntityList {
    char pad_0000[0x8];                 // 0x000
    uint32_t count;                     // 0x008 - number of entities in list
    char pad_000C[0x4];                 // 0x00C
    EntityDrawTask entities[1];         // 0x010 - variable length array (0x90 stride)
};

// Container structure pointed to by SceneRenderer+0x38
struct SceneContextContainer {
    char pad_0000[0x30];                // 0x000
    SceneContext* sceneContext;         // 0x030 - pointer to scene context
};

// Occlusion culling filter table
struct OcclusionFilterTable {
    char pad_0000[0x8];                 // 0x000
    uint32_t count;                     // 0x008 - number of entries
    char pad_000C[0x4];                 // 0x00C
    uint32_t VisibleOccludees[1];       // 0x010 - variable length array (1 = visible)
};

// Uniform manager structure
struct UniformManagerVTable {
    char pad_0000[0x348];               // 0x000
    uint64_t setShaderUniform;          // 0x348
};

struct UniformManager {
    char pad_0000[0x10];                // 0x000
    UniformManagerVTable* vtable;       // 0x010
};

// Buffer manager structure
struct BufferManagerVTable {
    char pad_0000[0x278];               // 0x000
	uint64_t bindUniformBufferRange;    // 0x278
};

struct BufferManager {
    char pad_0000[0x8];                 // 0x000
    BufferManagerVTable* vtable;        // 0x008
};
