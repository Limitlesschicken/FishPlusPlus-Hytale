#pragma once

#include "Entity.h"
#include "Camera.h"
#include "Time.h"
#include "EntityStoreModule.h"

class GameInstance
{
public:
    char pad_0000[0x10];
    Entity* Player;
    char pad_0018[0xE8];
    Time* Time;
    char pad_0108[0x8];
    StructBeforeCam* Camera;

    char pad_0118[0x20];
    EntityStoreModule* entityStoreModule;

    char pad_0140[0x88];
};