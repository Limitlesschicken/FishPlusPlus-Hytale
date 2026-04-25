/*
* Copyright (c) FishPlusPlus.
*/
#pragma once


#include <filesystem>
#include <vector>

#include "../Math/Matrix4x4.h"
#include "../Math/Vector2.h"

#include "InputSystem.h"
#include "../sdk/Hytale/Camera.h"
#include "../sdk/Hytale/CameraModule.h"
#include "../sdk/Hytale/GameInstance.h"
#include "../sdk/Hytale/DefaultMovementController.h"
#include "../sdk/Hytale/App.h"
#include "../sdk/Hytale/Entity.h"

struct SimpleTime
{
    int hour;
    int minute;
    int second;
    int millisecond;
};

namespace Util {
    inline App* app;
    Entity* getLocalPlayer();
    GameInstance* getGameInstance();
    DefaultMovementController* GetMovementController();
    Camera* getCamera();
    CameraModule* getCameraModule();
    inline Matrix4x4 viewProjMat;
    inline Matrix4x4 orthoProjMat;
	inline bool orthoProjMatInitialized = false;
    inline int frameCount = 0;

    inline float cursorPosX = 0;
    inline float cursorPosY = 0;
    inline bool console_allocated = false;
   
    double GetTime();
    bool IsValidPtr(void* ptr);
    bool IsValidPtr(uint64_t ptr);
    uint64_t RelativeVirtualAddress(uint64_t address, int opcode_size = 0x3, int opcode_length = 0x7);
    uint64_t PatternScan(const char* signature);

    bool WorldToScreen(Vector3 pos, Vector2& out);
    Matrix4x4 getViewProjMat();
    SimpleTime HoursToTime(float hours);
    uint64_t BuildTicksFromHours(float hours);
    const char* GetKeyName(SDL_Scancode key);
    bool ShouldInteractWithGame();
    bool isFullyInitialized();
    void allocate_console();
    void free_console();
    void log(const char* fmt, ...);
    std::string string_format(const std::string fmt, ...);

    HytaleString* ObjectToString(void* object);
}