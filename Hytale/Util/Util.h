/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once


#include <filesystem>
#include <vector>

#include "../Math/Matrix4x4.h"
#include "../Math/Vector2.h"

#include "InputSystem.h"
#include "../sdk/Camera.h"
#include "../sdk/CameraModule.h"
#include "../sdk/GameInstance.h"
#include "../sdk/DefaultMovementController.h"
#include "../sdk/App.h"
#include "../sdk/Entity.h"

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
    Camera* getCamera();
    CameraModule* getCameraModule();
    inline Matrix4x4 viewProjMat;
    inline DefaultMovementController* dmc;

    inline float cursorPosX = 0;
    inline float cursorPosY = 0;
   
    double GetTime();
    bool IsValidPtr(void* ptr);

    bool WorldToScreen(Vector3 pos, Vector2& out);
    void ResetGlobals();
    Matrix4x4 getViewProjMat();
    std::filesystem::path GetDirectory();
    SimpleTime HoursToTime(float hours);
    __int64 BuildTicksFromHours(float hours);
    const char* GetKeyName(SDL_Scancode key);
    bool ShouldInteractWithGame();
}