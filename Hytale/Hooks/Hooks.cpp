/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "Hooks.h"
#include "Util/AntiCrash.h"
#include "Util/TrampolineUnwind.h"

// Helper macros for creating hooks
// These macros simplify the process of creating hooks by combining pattern scanning and hook creation into a single step. They also log the addresses found for easier debugging.
#define CREATE_HOOK(name) \
if (MH_CreateHook((LPVOID)name##Address, &hk##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    Util::log("Failed to hook %s\n", #name);\
    return false;\
}\
TrampolineUnwind::Register(reinterpret_cast<void*>(o##name), reinterpret_cast<void*>(name##Address), 64);

#define CREATE_SIG_HOOK(name, pattern) \
std::uintptr_t name##Address = Util::PatternScan(pattern);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX\n", #name, name##Address, gameBase, (name##Address - gameBase));\
if (MH_CreateHook((LPVOID)name##Address, &hk##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    Util::log("Failed to hook %s\n", #name);\
    return false;\
}\
TrampolineUnwind::Register(reinterpret_cast<void*>(o##name), reinterpret_cast<void*>(name##Address), 64);

#define CREATE_SIG_HOOK_BY_REF(name, pattern) \
std::uintptr_t name##Address = Util::RelativeVirtualAddress(Util::PatternScan(pattern), 1, 5);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX\n", #name, name##Address, gameBase, (name##Address - gameBase));\
if (MH_CreateHook((LPVOID)name##Address, &hk##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    Util::log("Failed to hook %s\n", #name);\
    return false;\
}\
TrampolineUnwind::Register(reinterpret_cast<void*>(o##name), reinterpret_cast<void*>(name##Address), 64);

/*
* Creates and registers all hooks
*/
bool Hooks::CreateHooks() {
    Util::log("Creating Hooks\n");

    if (MH_Initialize() != MH_OK) {
        Util::log("Failed to initialize MinHook");
        return false;
    }

    std::uintptr_t WglSwapBuffersAddress = reinterpret_cast<uint64_t>(GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers"));
    ValidPtrBool(WglSwapBuffersAddress);

    CREATE_HOOK(WglSwapBuffers);
    CREATE_SIG_HOOK(FrameIterator_IsValid, "48 83 79 ? ? 0F 95 C0 C3 CC CC CC CC CC CC CC 40 53");
    CREATE_SIG_HOOK(WeatherUpdate, "41 57 41 56 41 55 41 54 57 56 55 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 44 0F 29 8C 24 ? ? ? ? 0F 57 E4 0F 29 64 24 ? 0F 29 64 24 ? 48 B8");
    CREATE_SIG_HOOK_BY_REF(DoMoveCycle, "E8 ? ? ? ? FF CE 75 ? 48 8B 4B");
    CREATE_SIG_HOOK_BY_REF(HandleScreenShotting, "E8 ? ? ? ? 4C 8B 7D ? 49 8B 8F ? ? ? ? 39 09");
    CREATE_SIG_HOOK_BY_REF(OnUserInput, "E8 ? ? ? ? 49 8B 47 ? C6 80");
    CREATE_SIG_HOOK_BY_REF(OnChat, "E8 ? ? ? ? 48 8B 4D ? 48 8B 89 ? ? ? ? 48 8B 89");
    CREATE_SIG_HOOK_BY_REF(DrawEntityCharactersAndItems, "E8 ? ? ? ? 48 8B 4B ? 48 8B 49 ? BA ? ? ? ? 39 09 E8 ? ? ? ? 48 8B 85");
    CREATE_SIG_HOOK_BY_REF(DrawScene, "E8 ? ? ? ? 80 7B ? ? 75 ? 48 89 5D");

    AntiCrash::Install();
    MH_EnableHook(MH_ALL_HOOKS);

    Util::log("All Hooks created and registered successfully\n");
    return true;
}
